/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 01-Mar-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Comparison.h
This file defines utility functions for numerical comparison including floating point values and utility structures for 
numerical limit handling.

Half implementation based on: Fast Half Float Conversions by Jeroen van der Zijp (September 2010).
*/

#ifndef E_COMPARISON_H
#define E_COMPARISON_H

#include "Math.h"
#include <Base.h>
#include <limits>

/*----------------------------------------------------------------------------------------------------------------------
Math comparison assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_MATH_A_EQUAL_B                       "The value of %s (%d) must equal the value of %s (%d)"
#define E_ASSERT_MSG_MATH_A_SMALLER_B                     "The value of %s (%d) must be smaller than the value of %s (%d)"
#define E_ASSERT_MSG_MATH_A_SMALLER_EQUAL_B               "The value of %s (%d) cannot exceed value of %s (%d)"
#define E_ASSERT_MSG_MATH_EQUAL_TO_ONE_VALUE              "Value must be one"
#define E_ASSERT_MSG_MATH_EQUAL_TO_ZERO_VALUE             "Value must be zero"
#define E_ASSERT_MSG_MATH_GREATER_OR_EQUAL_TO_ONE_VALUE   "Value must be greater or equal to one"
#define E_ASSERT_MSG_MATH_GREATER_OR_EQUAL_TO_ZERO_VALUE  "Value must be greater or equal to zero"
#define E_ASSERT_MSG_MATH_GREATER_THAN_ONE_VALUE          "Value must be greater than one"
#define E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE         "Value must be greater than zero"
#define E_ASSERT_MSG_MATH_NON_ZERO_VALUE                  "Value cannot be zero"
#define E_ASSERT_MSG_MATH_POWER_OF_TWO_VALUE              "Value must be a power of two"
#define E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE             "Value is out of bounds"
#define E_ASSERT_MSG_MATH_NON_EXISTANT_VALUE              "Value does not exist"
#define E_STATIC_ASSERT_MSG_MATH_IEEE_754_F32_VALUE       "Floating point value must comply with IEEE 754 standard"

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
NumericLimits
This struct defines numeric limits for different types
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct NumericLimits
{
  static T Epsilon() { return std::numeric_limits<T>::epsilon(); }
  static T Max() { return std::numeric_limits<T>::max(); }
  static T Min() { return std::numeric_limits<T>::min(); }
};

/*----------------------------------------------------------------------------------------------------------------------
Epsilon
This class defines an epsilon value to use to compare floating point numbers
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct Epsilon
{
  static T Get() { return static_cast<T>(0); }
};

/*----------------------------------------------------------------------------------------------------------------------
Epsilon specialization
----------------------------------------------------------------------------------------------------------------------*/
template <>
struct Math::Epsilon<F32> { static F32 Get() { return 1e-5f; } };
template <>
struct Math::Epsilon<D64> { static D64 Get() { return 1e-7; } };

/*----------------------------------------------------------------------------------------------------------------------
Math (comparison)

Please note that these methods have the following contract:

1. ClampToSign returns 1 or -1 depending on whether the passed value is positive or negative.
2. IsEqual performs an absolute comparison. It compares the value difference with an epsilon value independently of
each of the values scale. Note: this function needs to be defined as it is using a template default argument.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
inline T    CeilMultiple(T v, T multiple) { return (v % multiple == 0) ? v : NextMultiple(v, multiple); }

template <typename T>
T           CeilPowerOf2(T v);

template <typename T>
inline T    Clamp(T x, T low, T high) { return (x > high) ? high : ((x < low) ? low : x); }

template <typename T>
inline T    ClampToSign(T x) { return x >= (T)0 ? (T)1 : (T)(-1); }

/*----------------------------------------------------------------------------------------------------------------------
Half

Please note that these methods have the following contract:

1. Half performs a IEEE 754 conformant floating point conversion from  32- bit to 16-bit.
2. Half assumes U32 to be same size as F32.
----------------------------------------------------------------------------------------------------------------------*/
U16         Half(F32 value);

template<typename T>
inline bool IsEqual(T a, T b, T epsilon = Epsilon<T>::Get()) { return Math::Abs(a - b) <= epsilon; }

template <typename T>
inline bool IsInRange(T x, T low, T high) { return (x > high) ? false : ((x < low) ? false : true); }

template <typename T>
inline bool IsPower2(T x) { return x && !(x & (x - 1)); }

/*----------------------------------------------------------------------------------------------------------------------
IsRelativelyEqual

This function performs a relative comparison. The greatest of both values is scaled with the epsilon value to perform 
the comparison. This way for example:

F32 a = 99.9f;
F32 b = 100.0f;

E_ASSERT_MSG(!Math::IsEqual(a, b, 1e-3f));
E_ASSERT_MSG(Math::IsRelativelyEqual(a, b, 1e-3f));

This implementation is based in The art of computer programming by Knuth. Note: this function needs to be defined as it 
is using a template default argument.
----------------------------------------------------------------------------------------------------------------------*/
template<typename T>
inline bool IsRelativelyEqual(T a, T b, T epsilon = Epsilon<T>::Get()) { return Abs(a - b) <= Max(Abs(a), Abs(b)) * epsilon; }

template<typename T>
inline bool IsLess(T a, T b, T epsilon = Epsilon<T>::Get()) { return (b - a) > epsilon; }

template <typename T>
inline T    Max(T x, T y) { return x > y ? x : y; }

template <typename T>
inline T    Min(T x, T y) { return x < y ? x : y; }

template <typename T>
inline T    NextMultiple(T v, T multiple) { return v + (multiple - (v % multiple)); }

template <typename T>
inline T    Saturate(T x) { return Clamp(x, 0.0f, 1.0f); }

template <typename T>
inline void Swap(T& a, T& b, T epsilon = Epsilon<T>::Get()) { if (IsEqual(a, b, epsilon)) return; T tmp = a; a = b; b = tmp; }

/*----------------------------------------------------------------------------------------------------------------------
Math methods
----------------------------------------------------------------------------------------------------------------------*/

inline U16 Half(F32 value)
{
  static_assert(std::numeric_limits<F32>::is_iec559, E_STATIC_ASSERT_MSG_MATH_IEEE_754_F32_VALUE);
  static const U16 kBaseTable[512] = { 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 
    0x0200, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00, 0x2000, 0x2400, 0x2800, 0x2C00, 0x3000, 0x3400, 0x3800, 0x3C00, 
    0x4000, 0x4400, 0x4800, 0x4C00, 0x5000, 0x5400, 0x5800, 0x5C00, 0x6000, 0x6400, 0x6800, 0x6C00, 0x7000, 0x7400, 0x7800, 0x7C00, 
    0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
    0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
    0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
    0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
    0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
    0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
    0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100, 
    0x8200, 0x8400, 0x8800, 0x8C00, 0x9000, 0x9400, 0x9800, 0x9C00, 0xA000, 0xA400, 0xA800, 0xAC00, 0xB000, 0xB400, 0xB800, 0xBC00, 
    0xC000, 0xC400, 0xC800, 0xCC00, 0xD000, 0xD400, 0xD800, 0xDC00, 0xE000, 0xE400, 0xE800, 0xEC00, 0xF000, 0xF400, 0xF800, 0xFC00, 
    0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
    0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
    0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
    0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
    0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
    0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 
    0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00 };
    
  static const unsigned char kShiftTable[512] = { 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13 };
    
  U32 bits;// = *reinterpret_cast<U32*>(&value);  // Violates strict aliasing!
  std::memcpy(&bits, &value, sizeof(F32));
  return (kBaseTable[bits >> 23] + static_cast<U16>((bits & 0x7FFFFF) >> kShiftTable[bits >> 23]));
}

/*----------------------------------------------------------------------------------------------------------------------
Math methods specializations
----------------------------------------------------------------------------------------------------------------------*/

template<>
inline U32 CeilPowerOf2(U32 v)
{
  U32 pow2 = v;
  pow2--;
  pow2 = (pow2 >> 1) | pow2;
  pow2 = (pow2 >> 2) | pow2;
  pow2 = (pow2 >> 4) | pow2;
  pow2 = (pow2 >> 8) | pow2;
  pow2 = (pow2 >> 16) | pow2;
	
  return ++pow2;
}

template<>
inline U64 CeilPowerOf2(U64 v)
{
  U64 pow2 = v;
  pow2--;
  pow2 = (pow2 >> 1) | pow2;
  pow2 = (pow2 >> 2) | pow2;
  pow2 = (pow2 >> 4) | pow2;
  pow2 = (pow2 >> 8) | pow2;
  pow2 = (pow2 >> 16) | pow2;
  pow2 = (pow2 >> 32) | pow2;

  return ++pow2;
}
}
}

#endif
