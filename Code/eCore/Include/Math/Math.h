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

// Created 31-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Math.h
This file defines basic mathematic utility functions.
*/

#ifndef E3_MATH_H
#define E3_MATH_H

#include <Base.h>
#include <cmath>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Trigonometric constants
----------------------------------------------------------------------------------------------------------------------*/

static const D64 kPi		=	3.141592653589793238462;
static const D64 k2Pi		=	kPi * 2.0;
static const D64 kPiDiv2    =	kPi / 2.0;
static const D64 kPiDiv4	=	kPi / 4.0;
static const D64 kPiDiv180  =	kPi / 180.0;
static const D64 k1DivPi	=	1.0 / kPi;
static const D64 k1Div2Pi	=	1.0 / k2Pi;
static const D64 k180DivPi  =	180.0 / kPi;
static const D64 kSqrt2     =   1.414213562373095048801;
static const D64 kSqrt2Div2 =   0.707106781186547524400;

static const F32 kPif		=	3.1415927f;
static const F32 k2Pif		=	kPif * 2.f;
static const F32 kPiDiv2f	=	kPif / 2.f;
static const F32 kPiDiv4f	=	kPif / 4.f;
static const F32 kPiDiv180f	=	kPif / 180.f;
static const F32 k1DivPif	=	1.f / kPif;
static const F32 k1Div2Pif	=	1.f / k2Pif;
static const F32 k180DivPif	=	180.f / kPif;
static const F32 kSqrt2f     =   1.4142135f;
static const F32 kSqrt2Div2f =   0.7071068f;

/*----------------------------------------------------------------------------------------------------------------------
Math (Math)

Please note that these functions have the following usage contract: 

1. Floating point types are expected to be used with all the functions: F32, D64.
----------------------------------------------------------------------------------------------------------------------*/
template<typename T>
inline T Abs(T v)     { return std::abs(v); }
template<typename T>
inline T Acos(T v)    { return std::acos(v); }
template<typename T>
inline T Asin(T v)    { return std::asin(v); }
template<typename T> 
inline T Atan(T v)    { return std::atan(v); }
template<typename T>
inline T Ceil(T v)    { return std::ceil(v); }
template<typename T>
inline T Cos(T v)     { return std::cos(v); }
template<typename T>
inline T Deg(T rad)   { return rad * static_cast<T>(k180DivPi); }
template<typename T>
inline T Floor(T v)   { return std::floor(v); }
template<typename T>
inline T Frac(T v)    { return v - Floor(v); }
template<typename T>
inline T Normalize180(T deg) { deg = Normalize360(deg); return (deg > 180.0f) ? deg - 360.0f : deg; }
template<typename T>
inline T Normalize360(T deg) { deg = std::fmod(deg, 360.0f); return (deg < 0.0f) ? deg + 360.0f : deg; }
template<typename T>
inline T Rad(T deg)   { return deg * static_cast<T>(kPiDiv180); }
template<typename T>
inline T Sin(T v)     { return std::sin(v); }
template<typename T>
inline T Sqrt(T v)    { return std::sqrt(v); }
template<typename T>
inline T Tan(T v)     { return std::tan(v); }
}
}

#endif
