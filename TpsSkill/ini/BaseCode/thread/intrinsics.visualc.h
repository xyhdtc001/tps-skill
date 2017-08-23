#ifndef _TORQUE_PLATFORM_PLATFORMINTRINSICS_VISUALC_H_
#define _TORQUE_PLATFORM_PLATFORMINTRINSICS_VISUALC_H_

#include <intrin.h>


inline void dFetchAndAdd( volatile uint32& ref, uint32 val )
{  
   _InterlockedExchangeAdd( ( volatile long* ) &ref, val );
}
inline void dFetchAndAdd( volatile int32& ref, int32 val )
{
   _InterlockedExchangeAdd( ( volatile long* ) &ref, val );
}

inline bool dCompareAndSwap( volatile uint32& ref, uint32 oldVal, uint32 newVal )
{
   return ( _InterlockedCompareExchange( ( volatile long* ) &ref, newVal, oldVal ) == oldVal );
}
inline bool dCompareAndSwap( volatile uint64& ref, uint64 oldVal, uint64 newVal )
{
   return ( _InterlockedCompareExchange64( ( volatile __int64* ) &ref, newVal, oldVal ) == oldVal );
}

inline uint32 dAtomicRead( volatile uint32 &ref )
{
   return _InterlockedExchangeAdd( ( volatile long* )&ref, 0 );
}

#endif // _TORQUE_PLATFORM_PLATFORMINTRINSICS_VISUALC_H_
