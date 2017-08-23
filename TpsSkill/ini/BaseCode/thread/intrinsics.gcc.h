//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _TORQUE_PLATFORM_PLATFORMINTRINSICS_GCC_H_
#define _TORQUE_PLATFORM_PLATFORMINTRINSICS_GCC_H_





// Fetch-And-Add
//
// NOTE: These do not return the pre-add value because
// not all platforms (damn you OSX) can do that.
//
inline void dFetchAndAdd( volatile uint32& ref, uint32 val )
{
      __sync_fetch_and_add(&ref, val );

}

inline void dFetchAndAdd( volatile int32& ref, int32 val )
{
      __sync_fetch_and_add( &ref, val );

}

// Compare-And-Swap

inline bool dCompareAndSwap( volatile uint32& ref, uint32 oldVal, uint32 newVal )
{
      return ( __sync_val_compare_and_swap( &ref, oldVal, newVal ) == oldVal );

}

inline bool dCompareAndSwap( volatile uint64& ref, uint64 oldVal, uint64 newVal )
{
      return ( __sync_val_compare_and_swap( &ref, oldVal, newVal ) == oldVal );

}

/// Performs an atomic read operation.
inline uint32 dAtomicRead( volatile uint32 &ref )
{
      return __sync_fetch_and_add( ( volatile long* ) &ref, 0 );

}

#endif // _TORQUE_PLATFORM_PLATFORMINTRINSICS_GCC_H_
