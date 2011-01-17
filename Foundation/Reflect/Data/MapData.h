#pragma once

#include "Foundation/String.h"
#include "Foundation/Container/Map.h"
#include "Foundation/Math/Matrix4.h"
#include "Foundation/Reflect/Registry.h"
#include "Foundation/Reflect/Data/SimpleData.h"
#include "Foundation/Reflect/Data/ContainerData.h"

namespace Helium
{
    namespace Reflect
    {
        class FOUNDATION_API MapData : public ContainerData
        {
        public:
            REFLECT_DECLARE_ABSTRACT( MapData, ContainerData );

            typedef Pair< ConstDataPtr, DataPtr > ValueType;
            typedef DynArray< ValueType > A_ValueType;

            typedef Pair< ConstDataPtr, ConstDataPtr > ConstValueType;
            typedef DynArray< ConstValueType > A_ConstValueType;

            virtual const Class* GetKeyClass() const = 0;
            virtual const Class* GetValueClass() const = 0;
            virtual void GetItems( A_ValueType& items ) = 0;
            virtual void GetItems( A_ConstValueType& items ) const = 0;
            virtual DataPtr GetItem( const Data* key ) = 0;
            virtual ConstDataPtr GetItem( const Data* key ) const = 0;
            virtual void SetItem( const Data* key, const Data* value ) = 0;
            virtual void RemoveItem( const Data* key ) = 0;
        };

        template< typename KeyT, typename ValueT, typename EqualKeyT = Equals< KeyT >, typename AllocatorT = DefaultAllocator >
        class FOUNDATION_API SimpleMapData : public MapData
        {
        public:
            typedef Map< KeyT, ValueT, EqualKeyT, AllocatorT > DataType;
            Data::Pointer< DataType > m_Data;

            typedef SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT > MapDataT;
            REFLECT_DECLARE_OBJECT( MapDataT, MapData );

            SimpleMapData();
            virtual ~SimpleMapData();

            virtual void ConnectData( Helium::HybridPtr< void > data ) HELIUM_OVERRIDE;

            virtual size_t GetSize() const HELIUM_OVERRIDE;
            virtual void Clear() HELIUM_OVERRIDE;

            virtual const Class* GetKeyClass() const HELIUM_OVERRIDE;
            virtual const Class* GetValueClass() const HELIUM_OVERRIDE;
            virtual void GetItems( A_ValueType& items ) HELIUM_OVERRIDE;
            virtual void GetItems( A_ConstValueType& items ) const HELIUM_OVERRIDE;
            virtual DataPtr GetItem( const Data* key ) HELIUM_OVERRIDE;
            virtual ConstDataPtr GetItem( const Data* key ) const HELIUM_OVERRIDE;
            virtual void SetItem( const Data* key, const Data* value ) HELIUM_OVERRIDE;
            virtual void RemoveItem( const Data* key ) HELIUM_OVERRIDE;

            virtual bool Set( const Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
            virtual bool Equals( const Object* object ) const HELIUM_OVERRIDE;

            virtual void Serialize( Archive& archive ) const HELIUM_OVERRIDE;
            virtual void Deserialize( Archive& archive ) HELIUM_OVERRIDE;

            virtual tostream& operator>>( tostream& stream ) const HELIUM_OVERRIDE;
            virtual tistream& operator<<( tistream& stream ) HELIUM_OVERRIDE;
        };

        typedef SimpleMapData< String, String > StringStringMapData;
        typedef SimpleMapData< String, bool > StringBoolMapData;
        typedef SimpleMapData< String, uint32_t > StringUInt32MapData;
        typedef SimpleMapData< String, int32_t > StringInt32MapData;

        typedef SimpleMapData< uint32_t, String > UInt32StringMapData;
        typedef SimpleMapData< uint32_t, uint32_t > UInt32UInt32MapData;
        typedef SimpleMapData< uint32_t, int32_t > UInt32Int32MapData;
        typedef SimpleMapData< uint32_t, uint64_t > UInt32UInt64MapData;

        typedef SimpleMapData< int32_t, String > Int32StringMapData;
        typedef SimpleMapData< int32_t, uint32_t > Int32UInt32MapData;
        typedef SimpleMapData< int32_t, int32_t > Int32Int32MapData;
        typedef SimpleMapData< int32_t, uint64_t > Int32UInt64MapData;

        typedef SimpleMapData< uint64_t, String > UInt64StringMapData;
        typedef SimpleMapData< uint64_t, uint32_t > UInt64UInt32MapData;
        typedef SimpleMapData< uint64_t, uint64_t > UInt64UInt64MapData;
        typedef SimpleMapData< uint64_t, Matrix4 > UInt64Matrix4MapData;

        typedef SimpleMapData< Helium::GUID, uint32_t > GUIDUInt32MapData;
        typedef SimpleMapData< Helium::GUID, Matrix4 > GUIDMatrix4MapData;
        typedef SimpleMapData< Helium::TUID, uint32_t > TUIDUInt32MapData;
        typedef SimpleMapData< Helium::TUID, Matrix4 > TUIDMatrix4MapData;
    }
}
