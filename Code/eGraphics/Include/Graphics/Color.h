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

// Created 19-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Color.h
This file defines the Color class. Color is a simple floating point color representation that allows multiple 
conversions to other representations.
*/

#ifndef E3_COLOR_H
#define E3_COLOR_H

#include <Base.h>

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
Color

Please note that this interface has the following usage contract:

- The default constructor color is the zero color (alpha component has a zero 0 value too).
- The I32 constructor translates to U32 rbga. I32 type is only chosen to avoid warnings from predefined colors.
----------------------------------------------------------------------------------------------------------------------*/
class Color
{
public:
  enum ID
  {
    eBlack        = 0xFF000000,
    eBlue         = 0xFFFF0000,
    eCyan         = 0xFFFFFF00,
    eDarkestGrey  = 0xFF040404,
    eDarkerGrey   = 0xFF202020,
    eDarkGrey     = 0xFF404040,
    eGreen        = 0xFF00FF00,
    eGrey         = 0xFFBBBBBB,
    eMagenta      = 0xFFFF00FF,
    eRed          = 0xFF0000FF,
    eYellow       = 0xFF00FFFF,
    eWhite        = 0xFFFFFFFF,
  };

  Color();
  Color(I32 rgba);
  Color(F32 r, F32 g, F32 b, F32 a = 1.0f);

  // Operators
  F32                 operator[](const size_t i) const;
  F32&                operator[](const size_t i);
  bool                operator==(const Color& other) const;
  bool                operator!=(const Color& other) const;

  // Accessors
  const F32*          Get() const;
  U32                 GetABGR() const;
  U32                 GetARGB() const;		
  U32                 GetRGBA() const;
  void                SetABGR(U32 color);
  void                SetARGB(U32 color);
  void                SetRGBA(U32 color);
  void                Set(U8 r, U8 g, U8 b, U8 a);
  void                Set(F32 r, F32 g, F32 b, F32 a = 1.0f);

  static const Color& ZeroColor() { static const Color sColor; return sColor; }

  F32                 r;
  F32                 g;
  F32                 b;
  F32                 a;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Color initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/
inline Color::Color()
{
  r = 0.0f;
  g = 0.0f;
  b = 0.0f;
  a = 0.0f;
}

inline Color::Color(I32 color)
{
  SetRGBA(static_cast<U32>(color));
}

inline Color::Color(F32 r, F32 g, F32 b, F32 a)
{
  Set(r, g, b, a);
}

/*----------------------------------------------------------------------------------------------------------------------
Vector4 operators
----------------------------------------------------------------------------------------------------------------------*/

inline F32 Color::operator[](const size_t i) const
{ 
  E_ASSERT_MSG(i < 4, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&r + i);
}

inline F32& Color::operator[](const size_t i)
{ 
  E_ASSERT_MSG(i < 4, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&r + i);
}

inline bool Color::operator==(const Color& other) const
{ 
  return 
    Math::IsEqual(other.r, r) &&
    Math::IsEqual(other.g, g) &&
    Math::IsEqual(other.b, b) &&
    Math::IsEqual(other.a, a);
}

inline bool Color::operator!=(const Color& other) const
{ 
  return !((*this) == other);
}

/*----------------------------------------------------------------------------------------------------------------------
Color accessors
----------------------------------------------------------------------------------------------------------------------*/
inline const F32* Color::Get() const
{
  return &r;
}

inline U32 Color::GetABGR() const
{
  U32	color = 
    ((static_cast<U32>(a * 255.f) & 0xff) << 24) |
    ((static_cast<U32>(b * 255.f) & 0xff) << 16) |
    ((static_cast<U32>(g * 255.f) & 0xff) << 8) |
    ((static_cast<U32>(r * 255.f) & 0xff));
  return color;
}

inline U32 Color::GetARGB() const
{
  U32	color = 
    ((static_cast<U32>(a * 255.f) & 0xff) << 24) |
    ((static_cast<U32>(r * 255.f) & 0xff) << 16) |
    ((static_cast<U32>(g * 255.f) & 0xff) << 8) |
    ((static_cast<U32>(b * 255.f) & 0xff));
  return color;
}

inline U32 Color::GetRGBA() const
{
  U32	color = 
    ((static_cast<U32>(r * 255.f) & 0xff) << 24) |
    ((static_cast<U32>(g * 255.f) & 0xff) << 16) |
    ((static_cast<U32>(b * 255.f) & 0xff) << 8) |
    ((static_cast<U32>(a * 255.f) & 0xff));
  return color;
}

inline void Color::SetABGR(U32 color)
{
  U8* pColor = reinterpret_cast<U8*>(&color);
  Set(pColor[3], pColor[2], pColor[1], pColor[0]);
}

inline void Color::SetARGB(U32 color)
{
  U8* pColor = reinterpret_cast<U8*>(&color);
  Set(pColor[3], pColor[0], pColor[1], pColor[2]);
}

inline void Color::SetRGBA(U32 color)
{
  U8* pColor = reinterpret_cast<U8*>(&color);
  Set(pColor[0], pColor[1], pColor[2], pColor[3]);
}

inline void Color::Set(U8 r, U8 g, U8 b, U8 a)
{
  (*this).r = static_cast<F32>(r / 255.f);
  (*this).g = static_cast<F32>(g / 255.f);
  (*this).b = static_cast<F32>(b / 255.f);
  (*this).a = static_cast<F32>(a / 255.f);
}

inline void Color::Set(F32 r, F32 g, F32 b, F32 a)
{
  (*this).r = (r > 1.0f) ? 1.0f : r;
  (*this).g = (g > 1.0f) ? 1.0f : g;
  (*this).b = (b > 1.0f) ? 1.0f : b;
  (*this).a = (a > 1.0f) ? 1.0f : a;
}
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::Color)

#endif
