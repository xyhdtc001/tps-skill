#ifndef _TORQUE_PLATFORM_PLATFORMINTRINSICS_VISUALC_H_
#define _TORQUE_PLATFORM_PLATFORMINTRINSICS_VISUALC_H_

#include <sys/atomics.h>


inline void dFetchAndAdd( volatile uint32& ref, uint32 val )
{
	__sync_fetch_and_add((int* ) &ref, val);
}

inline void dFetchAndAdd( volatile int32& ref, int32 val )
{
	__sync_fetch_and_add((int* ) &ref, val);
}

// Compare-And-Swap

inline bool dCompareAndSwap( volatile uint32& ref, uint32 oldVal, uint32 newVal )
{
	return !__atomic_cmpxchg(oldVal, newVal, (int *) &ref);
}

inline bool dCompareAndSwap( volatile uint64& ref, uint64 oldVal, uint64 newVal )
{
	return !__atomic_cmpxchg(oldVal, newVal, (int *) &ref);
}

/// Performs an atomic read operation.
inline uint32 dAtomicRead( volatile uint32 &ref )
{
	return __sync_fetch_and_add((int* ) &ref, 0);
}

#endif // _TORQUE_PLATFORM_PLATFORMINTRINSICS_VISUALC_H_
