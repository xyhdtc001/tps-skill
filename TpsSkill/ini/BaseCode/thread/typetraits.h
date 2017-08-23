#ifndef _TYPETRAITS_H_
#define _TYPETRAITS_H_

//--------------------------------------------------------------------------
//    Construct.
//--------------------------------------------------------------------------
template< class T >
inline T* constructArrayInPlace( T* p, uint32 num )
{
	return new ( p ) T[ num ];
}

template <class T>
inline T* constructInPlace(T* p)
{
	return new ( p ) T;
}


template <class T>
inline void destructInPlace(T* p)
{
	p->~T();
}

struct _ConstructDefault
{
   template< typename T >
   static T single()
   {
      return T();
   }
   template< typename T, typename A >
   static T single( A a )
   {
      return T( a );
   }
   template< typename T, typename A, typename B >
   static T single( A a, B b )
   {
      return T( a, b );
   }
   template< typename T >
   static void array( T* ptr, uint32 num )
   {
      constructArrayInPlace< T >( ptr, num );
   }
   template< typename T, typename A >
   static void array( T* ptr, uint32 num, A a )
   {
      for( uint32 i = 0; i < num; ++ i )
         ptr[ i ] = single< T >( a );
   }
};
struct _ConstructPrim
{
   template< typename T >
   static T single()
   {
      return 0;
   }
   template< typename T, typename A >
   static T single( T a )
   {
      return a;
   }
   template< typename T >
   static void array( T* ptr, uint32 num )
   {
      memset( ptr, 0, num * sizeof( T ) );
   }
   template< typename T, typename A >
   static void array( T* ptr, uint32 num, T a )
   {
      for( uint32 i = 0; i < num; ++ i )
         ptr[ i ] = a;
   }
};
struct _ConstructPtr
{
   template< typename T >
   static T* single()
   {
      return new T;
   }
   template< typename T, typename A >
   static T* single( A a )
   {
      return new T( a );
   }
   template< typename T, typename A, typename B >
   static T* single( A a, B b )
   {
      return new T( a, b );
   }
   template< typename  T >
   static void array( T** ptr, uint32 num )
   {
      for( uint32 i = 0; i < num; ++ i )
         ptr[ i ] = single< T >();
   }
   template< typename T, typename A >
   static void array( T** ptr, uint32 num, A a )
   {
      for( uint32 i = 0; i < num; ++ i )
         ptr[ i ] = single< T >( a );
   }
};

//--------------------------------------------------------------------------
//    Destruct.
//--------------------------------------------------------------------------

struct _DestructDefault
{
   template< typename T >
   static void single( T& val )
   {
      val.~T();
   }
   template< typename T >
   static void array( T* ptr, uint32 num )
   {
      for( uint32 i = 0; i < num; ++ i )
         single< T >( ptr[ i ] );
   }
};
struct _DestructPrim
{
   template< typename T >
   static void single( T& val ) {}
   template< typename T >
   static void array( T* ptr, uint32 num ) {}
};
struct _DestructPtr
{
   template< typename T >
   static void single( T*& val )
   {
      delete val;
      val = NULL;
   }
   template< typename T >
   static void array( T* ptr, uint32 num )
   {
      for( uint32 i = 0; i < num; ++ i )
         single< T >( ptr[ i ] );
   }
};

//--------------------------------------------------------------------------
//    TypeTraits.
//--------------------------------------------------------------------------

template< typename T >
struct _TypeTraits
{
   typedef T BaseType;
   typedef const T ConstType;
   typedef _ConstructDefault Construct;
   typedef _DestructDefault Destruct;
};
template< typename T >
struct _TypeTraits< T* >
{
   typedef T BaseType;
   typedef const T ConstType;
   typedef _ConstructPtr Construct;
   typedef _DestructPtr Destruct;

   template< typename A >
   static bool isTaggedPtr( A* ptr ) { return ( uintptr_t( ptr ) & 0x1 ); }
   template< typename A >
   static A* getTaggedPtr( A* ptr ) { return ( A* ) ( uintptr_t( ptr ) | 0x1 ); }
   template< typename A >
   static A* getUntaggedPtr( A* ptr ) { return ( A* ) ( uintptr_t( ptr ) & (~0x1) ); }
};

template< typename T >
struct TypeTraits : public TypeTraits< typename T::Parent >
{
   typedef T BaseType;
   typedef const T ConstType;
};
template< typename T >
struct TypeTraits< T* > : public TypeTraits< typename T::Parent* >
{
   typedef T BaseType;
   typedef const T ConstType;
};
template< typename T >
struct TypeTraits< T* const > : public TypeTraits< typename T::Parent* >
{
   typedef T BaseType;
   typedef const T ConstType;
};
template<>
struct TypeTraits< void > : public _TypeTraits< void > {};
template<>
struct TypeTraits< void* > : public _TypeTraits< void* > {};
template<>
struct TypeTraits< void* const > : public _TypeTraits< void* > {};

// Type traits for primitive types.

template<>
struct TypeTraits< bool > : public _TypeTraits< bool >
{
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};
template<>
struct TypeTraits< int8 > : public _TypeTraits< int8 >
{
   static const int8 MIN = Int8_MIN;
   static const int8 MAX = Int8_MAX;
   static const int8 ZERO = 0;
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};
template<>
struct TypeTraits< uint8 > : public _TypeTraits< uint8 >
{
   static const uint8 MIN = 0;
   static const uint8 MAX = Uint8_MAX;
   static const uint8 ZERO = 0;
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};
template<>
struct TypeTraits< int16 > : public _TypeTraits< int16 >
{
   static const int16 MIN = Int16_MIN;
   static const int16 MAX = Int16_MAX;
   static const int16 ZERO = 0;
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};
template<>
struct TypeTraits< uint16 > : public _TypeTraits< uint16 >
{
   static const uint16 MIN = 0;
   static const uint16 MAX = Uint16_MAX;
   static const uint16 ZERO = 0;
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};
template<>
struct TypeTraits< int32 > : public _TypeTraits< int32 >
{
   static const int32 MIN = Int32_MIN;
   static const int32 MAX = Int32_MAX;
   static const int32 ZERO = 0;
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};
template<>
struct TypeTraits< uint32 > : public _TypeTraits< uint32 >
{
   static const uint32 MIN = 0;
   static const uint32 MAX = Uint32_MAX;
   static const uint32 ZERO = 0;
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};
template<>
struct TypeTraits< float32 > : public _TypeTraits< float32 >
{
   static const float32 MIN;
   static const float32 MAX;
   static const float32 ZERO;
   typedef _ConstructPrim Construct;
   typedef _DestructPrim Destruct;
};

//--------------------------------------------------------------------------
//    Utilities.
//--------------------------------------------------------------------------

template< typename T >
inline T constructSingle()
{
   typedef typename TypeTraits< T >::BaseType Type;
   typedef typename TypeTraits< T >::Construct Construct;
   return Construct::template single< Type >();
}
template< typename T, typename A >
inline T constructSingle( A a )
{
   typedef typename TypeTraits< T >::BaseType BaseType;
   typedef typename TypeTraits< T >::Construct Construct;
   return Construct::template single< BaseType >( a );
}
template< typename T, typename A, typename B >
inline T constructSingle( A a, B b )
{
   typedef typename TypeTraits< T >::BaseType BaseType;
   typedef typename TypeTraits< T >::Construct Construct;
   return Construct::template single< BaseType >( a, b );
}
template< typename T >
inline void constructArray( T* ptr, uint32 num )
{
   typedef typename TypeTraits< T >::BaseType BaseType;
   typedef typename TypeTraits< T >::Construct Construct;
   Construct::template array< BaseType >( ptr, num );
}
template< typename T, typename A >
inline void constructArray( T* ptr, uint32 num, A a )
{
   typedef typename TypeTraits< T >::BaseType BaseType;
   typedef typename TypeTraits< T >::Construct Construct;
   Construct::template array< BaseType >( ptr, num, a );
}
template< typename T >
inline void destructSingle( T& val )
{
   typedef typename TypeTraits< T >::BaseType BaseType;
   typedef typename TypeTraits< T >::Destruct Destruct;
   Destruct::template single< BaseType >( val );
}
template< typename T >
inline void destructArray( T* ptr, uint32 num )
{
   typedef typename TypeTraits< T >::BaseType BaseType;
   typedef typename TypeTraits< T >::Destruct Destruct;
   Destruct::template array< BaseType >( ptr, num );
}

template< typename T>
inline T& Deref( T& val )
{
   return val;
}
template< typename T >
inline T& Deref( T* ptr )
{
   return *ptr;
}

/// Delete a single object policy.
struct DeleteSingle
{
   template<class T>
   static void destroy(T *ptr) { delete ptr; }
};

/// Delete an array of objects policy.
struct DeleteArray
{
   template<class T>
   static void destroy(T *ptr) { delete [] ptr; }
};

///
template< typename T >
struct ValueHolder
{
   T value;
   
   ValueHolder( const T& value )
      : value( value ) {}
      
   operator T() const { return value; }
};
template<>
struct ValueHolder< void >
{
   ValueHolder() {}
};

#endif // _TYPETRAITS_H_
