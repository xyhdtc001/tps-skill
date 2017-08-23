#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#include "basetype.h"

//------------------------------------------------------------------------------
// Endian conversions

inline uint8 endianSwap(const uint8 in_swap)
{
   return in_swap;
}

inline int8 endianSwap(const int8 in_swap)
{
   return in_swap;
}

/**
Convert the byte ordering on the U16 to and from big/little endian format.
@param in_swap Any U16
@returns swapped U16.
*/

inline uint16 endianSwap(const uint16 in_swap)
{
   return uint16(((in_swap >> 8) & 0x00ff) |
      ((in_swap << 8) & 0xff00));
}

inline int16 endianSwap(const int16 in_swap)
{
   return int16(endianSwap(uint16(in_swap)));
}

/**
Convert the byte ordering on the uint32 to and from big/little endian format.
@param in_swap Any uint32
@returns swapped uint32.
*/
inline uint32 endianSwap(const uint32 in_swap)
{
   return uint32(((in_swap >> 24) & 0x000000ff) |
      ((in_swap >>  8) & 0x0000ff00) |
      ((in_swap <<  8) & 0x00ff0000) |
      ((in_swap << 24) & 0xff000000));
}

inline int32 endianSwap(const int32 in_swap)
{
   return int32(endianSwap(uint32(in_swap)));
}

inline uint64 endianSwap(const uint64 in_swap)
{
   uint32 *inp = (uint32 *) &in_swap;
   uint64 ret = 0;
   uint32 *outp = (uint32 *) &ret;
   outp[0] = endianSwap(inp[1]);
   outp[1] = endianSwap(inp[0]);
   return ret;
}

inline int64 endianSwap(const int64 in_swap)
{
   return int64(endianSwap(uint64(in_swap)));
}

inline float32 endianSwap(const float32 in_swap)
{
   uint32 result = endianSwap(* ((uint32 *) &in_swap) );
   return * ((float32 *) &result);
}

inline float64 endianSwap(const float64 in_swap)
{
   uint64 result = endianSwap(* ((uint64 *) &in_swap) );
   return * ((float64 *) &result);
}

//------------------------------------------------------------------------------


#define TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(type) \
   inline type convertHostToLEndian(type i) { return i; } \
   inline type convertLEndianToHost(type i) { return i; } \
   inline type convertHostToBEndian(type i) { return endianSwap(i); } \
   inline type convertBEndianToHost(type i) { return endianSwap(i); }


TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(uint8)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(int8)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(uint16)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(int16)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(uint32)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(int32)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(uint64)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(int64)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(float32)
TORQUE_DECLARE_TEMPLATIZED_ENDIAN_CONV(float64)

#endif

