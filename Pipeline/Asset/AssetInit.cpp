#include "AssetInit.h"

#include "Pipeline/Asset/Components/ArtFileComponent.h"
#include "Pipeline/Asset/AssetType.h"
#include "Pipeline/Asset/AssetClass.h"
#include "Pipeline/Asset/AssetFile.h"
#include "Pipeline/Asset/AssetFolder.h"
#include "Pipeline/Asset/AssetTemplate.h"
#include "Pipeline/Asset/AssetVersion.h"
#include "Pipeline/Asset/Components/ColorMapComponent.h"
#include "Pipeline/Asset/Components/DependenciesComponent.h"
#include "Pipeline/Asset/Components/DetailMapComponent.h"
#include "Pipeline/Asset/Classes/Entity.h"
#include "Pipeline/Asset/Classes/EntityAsset.h"
#include "Pipeline/Asset/Manifests/EntityManifest.h"
#include "Pipeline/Asset/Components/ExpensiveMapComponent.h"
#include "Pipeline/Asset/ExporterJob.h"
#include "Pipeline/Asset/Classes/SceneAsset.h"
#include "Pipeline/Asset/Manifests/ManifestVersion.h"
#include "Pipeline/Asset/Components/NormalMapComponent.h"
#include "Pipeline/Asset/Components/StandardColorMapComponent.h"
#include "Pipeline/Asset/Components/StandardDetailMapComponent.h"
#include "Pipeline/Asset/Components/StandardExpensiveMapComponent.h"
#include "Pipeline/Asset/Components/StandardNormalMapComponent.h"
#include "Pipeline/Asset/Classes/StandardShaderAsset.h"
#include "Pipeline/Asset/Components/TextureMapComponent.h"
#include "Pipeline/Asset/Manifests/SceneManifest.h"

#include "Pipeline/Component/ComponentInit.h"
#include "Pipeline/Component/ComponentCategories.h"
#include "Foundation/InitializerStack.h"
#include "Pipeline/Content/ContentInit.h"
#include "Foundation/Reflect/Registry.h"

#include "Foundation/InitializerStack.h"

using namespace Reflect;

#define ASSET_BEGIN_REGISTER_ENGINE_TYPES                                       \
  Nocturnal::Insert<M_AssetTypeInfo>::Result et_inserted;

#define ASSET_REGISTER_ENGINETYPE( __AssetTypeName )                           \
  et_inserted = g_AssetTypeInfos.insert( M_AssetTypeInfo::value_type( AssetTypes::__AssetTypeName, AssetTypeInfo( #__AssetTypeName, #__AssetTypeName"Builder.dll" ) ) );

#define _ASSET_REGISTER_ENGINETYPE_SET_MEMBER( __MemberName, __Value )          \
  if ( et_inserted.second ) et_inserted.first->second.__MemberName = __Value;

#define ASSET_REGISTER_ENGINETYPE_BUILDERDLL( __BuilderDLL )                    \
  _ASSET_REGISTER_ENGINETYPE_SET_MEMBER( m_BuilderDLL, __BuilderDLL );

#define ASSET_REGISTER_ENGINETYPE_ICONFILENAME( __IconFilename )                \
  _ASSET_REGISTER_ENGINETYPE_SET_MEMBER( m_IconFilename, __IconFilename );

#define ASSET_REGISTER_ENGINETYPE_TYPECOLOR_ARGB(a,r,g,b)                       \
  _ASSET_REGISTER_ENGINETYPE_SET_MEMBER( m_TypeColor, ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))) );

#define _ASSET_REGISTER_ENGINETYPE_ENTITYCLASS                                  \
  if ( et_inserted.second && et_inserted.first->second.m_EntityAsset == NULL )  \
  et_inserted.first->second.m_EntityAsset = new EntityAsset();

#define ASSET_REGISTER_ENGINETYPE_ENTITYCLASS_ATTRIB( __Attribute )             \
  _ASSET_REGISTER_ENGINETYPE_ENTITYCLASS                                        \
  if ( et_inserted.second && et_inserted.first->second.m_EntityAsset != NULL )  \
  et_inserted.first->second.m_EntityAsset->SetComponent( new __Attribute() );

using namespace Asset;

std::vector< i32 > Asset::g_AssetClassTypes;

i32 g_AssetInitCount = 0;

Nocturnal::InitializerStack g_AssetInitializerStack;

void Asset::Initialize()
{
  if ( ++g_AssetInitCount == 1 )
  {
    g_AssetInitializerStack.Push( Reflect::Initialize, Reflect::Cleanup );
    g_AssetInitializerStack.Push( Component::Initialize, Component::Cleanup );
    g_AssetInitializerStack.Push( Content::Initialize, Content::Cleanup );

    //
    // Numbered asset conversion
    //

    g_AssetInitializerStack.Push( Reflect::RegisterClass<ExporterJob>( "ExporterJob" ) );


    //
    // Enums
    //

    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<RunTimeFilters::RunTimeFilter>( &RunTimeFilters::RunTimeFilterEnumerateEnumeration, "RunTimeFilter" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<WrapModes::WrapMode>( &WrapModes::WrapModeEnumerateEnumeration, "WrapMode" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<AlphaTypes::AlphaType>( &AlphaTypes::AlphaTypeEnumerateEnumeration, "AlphaType" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<WetSurfaceTypes::WetSurfaceType>( &WetSurfaceTypes::WetSurfaceTypeEnumerateEnumeration, "WetSurfaceType" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<MipGenFilterTypes::MipGenFilterType>( &MipGenFilterTypes::MipGenFilterTypeEnumerateEnumeration, "MipGenFilterType" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<PostMipFilterTypes::PostMipFilterType>( &PostMipFilterTypes::PostMipFilterTypeEnumerateEnumeration, "PostMipFilterType" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<ReductionRatios::ReductionRatio>( &ReductionRatios::ReductionRatioEnumerateEnumeration, "ReductionRatio" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<ColorTexFormats::ColorTexFormat>( &ColorTexFormats::ColorTexFormatEnumerateEnumeration, "ColorTexFormat" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<NormalTexFormats::NormalTexFormat>( &NormalTexFormats::NormalTexFormatEnumerateEnumeration, "NormalTexFormat" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<GlossParaIncanTexFormats::GlossParaIncanTexFormat>( &GlossParaIncanTexFormats::GlossParaIncanTexFormatEnumerateEnumeration, "GlossParaIncanTexFormat" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<CubeSpecTypeFormats::CubeSpecTypeFormat>( &CubeSpecTypeFormats::CubeSpecTypeFormatEnumerateEnumeration, "CubeSpecTypeFormat" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<DetailTexFormats::DetailTexFormat>( &DetailTexFormats::DetailTexFormatEnumerateEnumeration, "DetailTexFormat" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<Asset::AssetType>( &AssetTypes::AssetTypeEnumerateEnumeration, "AssetType" ) );

    //
    // Basic Types
    //

    g_AssetInitializerStack.Push( Reflect::RegisterClass<AssetTemplate>( "AssetTemplate" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<AssetVersion>( "AssetVersion" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<AssetClass>( "AssetClass" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<AssetFile>( "AssetFile" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<AssetFolder>( "AssetFolder" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<ShaderAsset>( "ShaderAsset" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<FileBackedComponent>( "FileBackedComponent" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<Entity>( "Entity" ) );

    g_AssetInitializerStack.Push( Reflect::RegisterClass<ManifestVersion>( "ManifestVersion" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<AssetManifest>( "AssetManifest" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<EntityManifest>( "EntityManifest" ) ); Reflect::Registry::GetInstance()->AliasType( Reflect::GetClass< EntityManifest >(), TXT( "AssetManifest" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<SceneManifest>( "SceneManifest" ) );


    //
    // Misc
    //

    // texture
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<TextureWrapModes::TextureWrapMode>( &TextureWrapModes::TextureWrapModeEnumerateEnumeration, "TextureWrapMode" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<TextureColorFormats::TextureColorFormat>( &TextureColorFormats::TextureColorFormatEnumerateEnumeration, "TextureColorFormat" ) );

    //
    // Asset Attributes
    //

    g_AssetInitializerStack.Push( Reflect::RegisterClass<DependenciesComponent>( "DependenciesComponent" ) );
    Component::ComponentCategories::GetInstance()->Categorize( new DependenciesComponent );

    g_AssetInitializerStack.Push( Reflect::RegisterClass<TextureMapComponent>( "TextureMapComponent" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<ColorMapComponent>( "ColorMapComponent" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<NormalMapComponent>( "NormalMapComponent" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<ExpensiveMapComponent>( "ExpensiveMapComponent" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<DetailMapComponent>( "DetailMapComponent" ) );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<StandardColorMapComponent>( "StandardColorMapComponent" ) );
    Component::ComponentCategories::GetInstance()->Categorize( new StandardColorMapComponent );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<StandardNormalMapComponent>( "StandardNormalMapComponent" ) );
    Component::ComponentCategories::GetInstance()->Categorize( new StandardNormalMapComponent );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<StandardExpensiveMapComponent>( "StandardExpensiveMapComponent" ) );
    Component::ComponentCategories::GetInstance()->Categorize( new StandardExpensiveMapComponent );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<StandardDetailMapComponent>( "StandardDetailMapComponent" ) );
    Component::ComponentCategories::GetInstance()->Categorize( new StandardDetailMapComponent );

    //
    // Attribute Sets
    //

    // appearance
    g_AssetInitializerStack.Push( Reflect::RegisterClass<ArtFileComponent>( "ArtFileComponent" ) );
    Component::ComponentCategories::GetInstance()->Categorize( new ArtFileComponent );

    //
    // Asset classes
    //

    g_AssetInitializerStack.Push( Reflect::RegisterClass<EntityAsset>( "EntityAsset" ) );
    g_AssetClassTypes.push_back( Reflect::GetType<EntityAsset>() );
    g_AssetInitializerStack.Push( Reflect::RegisterClass<SceneAsset>( "SceneAsset" ) );
    g_AssetClassTypes.push_back( Reflect::GetType<SceneAsset>() );

    // Shaders
    g_AssetInitializerStack.Push( Reflect::RegisterClass<StandardShaderAsset>( "StandardShaderAsset" ) );
    g_AssetClassTypes.push_back( Reflect::GetType<StandardShaderAsset>() );


    /////////////////////////////////////////////////////////////
    // Support for engine types

    // we build up entity classes that consist of all the attributes
    // necessary for the given type.  these are then used in the
    // Classify function to determine what kind of engine type the
    // entity class maps to.  (ie: an entity class maps to the
    // smallest superset)

    /*
    ASSET_BEGIN_REGISTER_ENGINE_TYPES;
    {
      // Null == -1
      g_AssetTypeInfos.insert( M_AssetTypeInfo::value_type( AssetTypes::Null, Asset::GetAssetTypeInfo( AssetTypes::Null ) ) );

      // Level
      ASSET_REGISTER_ENGINETYPE( Level );
      ASSET_REGISTER_ENGINETYPE_ICONFILENAME( "enginetype_level_16.png" );
      ASSET_REGISTER_ENGINETYPE_TYPECOLOR_ARGB( 0xff, 142, 234, 251 );

      // Shader
      ASSET_REGISTER_ENGINETYPE( Shader );
      ASSET_REGISTER_ENGINETYPE_ICONFILENAME( "enginetype_shader_16.png" );
      ASSET_REGISTER_ENGINETYPE_TYPECOLOR_ARGB( 0xff, 57, 143, 202 );

      // TexturePack
      ASSET_REGISTER_ENGINETYPE( TexturePack );
      ASSET_REGISTER_ENGINETYPE_BUILDERDLL( "TexturePackBuilder.dll" );
      ASSET_REGISTER_ENGINETYPE_ICONFILENAME( "enginetype_texturepack_16.png" );
      ASSET_REGISTER_ENGINETYPE_TYPECOLOR_ARGB( 0xff, 164, 93, 163 );
    }


    //
    // Enums
    //
    g_AssetInitializerStack.Push( Reflect::RegisterEnumeration<Asset::AssetTypes::AssetType>( &Asset::AssetTypes::AssetTypeEnumerateEnumeration, "AssetType" ) );
*/
    // Above is for supporting engine types
    ///////////////////////////////////////////////////////////////
  }
}

void Asset::Cleanup()
{
  if ( --g_AssetInitCount == 0 )
  {
    g_AssetClassTypes.clear();

    g_AssetInitializerStack.Cleanup();
  }
}
