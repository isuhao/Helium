#include "Precompile.h"
#include "DependencyLoader.h"
#include "DependencyCollection.h"

#include "Browser.h"

using namespace Luna;


///////////////////////////////////////////////////////////////////////////////
/// class DependencyLoader
///////////////////////////////////////////////////////////////////////////////
DependencyLoader::DependencyLoader( const std::string& rootDirectory, const std::string& configDirectory, DependencyCollection* collection )
: Nocturnal::ThreadMechanism( "DependencyLoader" )
, m_RootDirectory( rootDirectory )
, m_ConfigDirectory( configDirectory )
, m_Collection( collection )
{
}

///////////////////////////////////////////////////////////////////////////////
DependencyLoader::~DependencyLoader()
{
    m_AssetPaths.clear();
}

///////////////////////////////////////////////////////////////////////////////
void DependencyLoader::InitData()
{
    m_AssetPaths.clear();
}

///////////////////////////////////////////////////////////////////////////////
void DependencyLoader::ThreadProc( i32 threadID )
{
    ThreadEnter( threadID );

    if ( CheckThreadLeave( threadID ) )
    {
        return;
    }

    m_Collection->SetAssetReferences( m_AssetPaths );

    ThreadLeave( threadID );
}

///////////////////////////////////////////////////////////////////////////////
void DependencyLoader::OnEndThread( const Nocturnal::ThreadProcArgs& args )
{
    if ( !IsCurrentThread( args.m_ThreadID ) )
        return;

    m_Collection->IsLoading( false );
    m_Collection->Thaw();
}
