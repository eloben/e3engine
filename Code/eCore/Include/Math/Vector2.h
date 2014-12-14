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

// Created 25-Sep-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Vector2.h
This file defines the Vector2 class.
*/

#ifndef E_VECTOR2_H
#define E_VECTOR2_H

#include "Comparison.h"
#include <Assertion/Assert.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Vector2

Please note that this class has the following usage contract:

1. Signed types are expected to be used with this class: I32, F32, D64.
2. GetLength and Normalize are not supported on the I32 version.
3. MaxMin count must be greater than 0.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct Vector2
{
  Vector2();
  Vector2(T scalar);
  Vector2(T x, T y);

  // Operators
  const T&        operator[](const size_t i) const;
  T&              operator[](const size_t i);
  bool            operator==(const Vector2& other) const;
  bool            operator==(T scalar) const;
  bool            operator!=(const Vector2& other) const;
  bool            operator!=(T scalar) const;
  Vector2         operator+(const Vector2& other) const;
  Vector2         operator+(T scalar) const;
  Vector2&        operator+=(const Vector2& other);
  Vector2&        operator+=(T scalar);
  Vector2         operator-() const;
  Vector2         operator-(const Vector2& other) const;
  Vector2         operator-(T scalar) const;
  Vector2&	      operator-=(const Vector2& other);
  Vector2&        operator-=(T scalar);
  Vector2         operator*(const Vector2& other) const;
  Vector2         operator*(T scalar) const;
  Vector2&        operator*=(const Vector2& other);
  Vector2&        operator*=(T scalar);
  Vector2         operator/(const Vector2& other) const;
  Vector2         operator/(T scalar) const;
  Vector2&        operator/=(const Vector2& other);
  Vector2&        operator/=(T scalar);
  T               operator^(const Vector2& other) const;

  // Accessors
  T               GetLength() const;
  T               GetLengthSquared() const;
  bool            IsZero() const;
  void            Set(T scalar);
  void            Set(T x, T y);
  void            SetZero();

  // Methods
  void            Normalize();
  void            Sign();

  // Static methods
  static Vector2  AxisX();
  static Vector2  AxisY();
  static T        Cross(const Vector2& a, const Vector2& b);
  static T        Dot(const Vector2& a, const Vector2& b);
  static Vector2  Max(const Vector2& a, const Vector2& b);
  static Vector2  Min(const Vector2& a, const Vector2& b);
  static void     MinMax(Vector2& min, Vector2& max, const Vector2* pSource, const U32 count);
  static Vector2  ZeroVector();

  // Members
  T x;
  T y;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Vector2 initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
inline Vector2<T>::Vector2()
  : x(0)
  , y(0)
{
}

template <typename T>
inline Vector2<T>::Vector2(T scalar)
  : x(scalar)
  , y(scalar)
{
}

template <typename T>
inline Vector2<T>::Vector2(T x, T y)
  : x(x)
  , y(y)
{
}

/*----------------------------------------------------------------------------------------------------------------------
Vector2 operators
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline const T& Vector2<T>::operator[](const size_t i) const
{ 
  E_ASSERT_MSG(i < 2, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <typename T>
inline T& Vector2<T>::operator[](const size_t i)
{ 
  E_ASSERT_MSG(i < 2, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <typename T>
inline bool Vector2<T>::operator==(const Vector2& other) const
{ 
  return IsEqual(other.x, x) && IsEqual(other.y, y);
}

template <typename T>
inline bool Vector2<T>::operator==(T scalar) const
{ 
  return IsEqual(scalar, x) && IsEqual(scalar, y);
}

template <typename T>
inline bool Vector2<T>::operator!=(const Vector2& other) const
{ 
  return !((*this) == other);
}

template <typename T>
inline bool Vector2<T>::operator!=(T scalar) const
{ 
  return !((*this) == scalar);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator+(const Vector2& other) const
{
  return Vector2(x + other.x, y + other.y);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator+(T scalar) const		
{
  return Vector2(x + scalar, y + scalar);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2& other)		
{ 
  x += other.x; 
  y += other.y; 
  return (*this);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator+=(T scalar)	
{
  x += scalar; 
  y += scalar; 
  return (*this);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator-() const	
{
  return Vector2(-x, -y);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator-(const Vector2& other) const	
{
  return Vector2(x - other.x, y - other.y);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator-(T scalar) const	
{
  return Vector2(x - scalar, y - scalar);
}

template <typename T>
inline Vector2<T>&	Vector2<T>::operator-=(const Vector2& other)
{
  x -= other.x;
  y -= other.y;
  return (*this);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator-=(T scalar)
{
  x -= scalar;
  y -= scalar;
  return (*this);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator*(const Vector2& other) const	
{
  return Vector2(x * other.x, y * other.y);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator*(T scalar) const	
{
  return Vector2(x * scalar, y * scalar);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator*=(const Vector2& other)
{
  x *= other.x;
  y *= other.y;
  return (*this);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator*=(T scalar)	
{
  x *= scalar;
  y *= scalar;
  return (*this);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator/(const Vector2& other) const	
{
  E_ASSERT_MSG(!IsEqual(other.x, static_cast<T>(0)) && !IsEqual(other.y, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return Vector2(x / other.x, y / other.y);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator/(T scalar) const		
{
  E_ASSERT_MSG(!IsEqual(scalar, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return Vector2(x / scalar, y / scalar);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator/=(const Vector2& other)
{
  E_ASSERT_MSG(!IsEqual(other.x, static_cast<T>(0)) && !IsEqual(other.y, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  x /= other.x;
  y /= other.y; 
  return (*this);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator/=(T scalar)
{
  E_ASSERT_MSG(!IsEqual(scalar, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  x /= scalar;
  y /= scalar; 
  return (*this);
}

template <typename T>
inline T Vector2<T>::operator^(const Vector2& other) const	
{
  return Vector2<T>::Cross(*this, other);
}

/*----------------------------------------------------------------------------------------------------------------------
Vector2 accessors
----------------------------------------------------------------------------------------------------------------------*/

// Take note that integer version won't compile:
// sqrt works with long, floats and doubles hence it can not be used with integers. Length and Normalize won't work
template <typename T>
inline T Vector2<T>::GetLength() const	
{ 
  return Math::Sqrt(GetLengthSquared()); 
}

template <typename T>
inline T Vector2<T>::GetLengthSquared() const	
{ 
  return x * x + y * y; 
}

template <typename T>
inline bool Vector2<T>::IsZero() const
{
  return !x && !y;
}

template <typename T>
inline void Vector2<T>::Set(T scalar)
{
  x = scalar;
  y = scalar;
}

template <typename T>
inline void Vector2<T>::Set(T x, T y)
{
  (*this).x = x;
  (*this).y = y;
}

template <typename T>
inline void Vector2<T>::SetZero()
{
  x = 0; 
  y = 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Vector2 methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline void Vector2<T>::Normalize()								
{			
  T length = GetLength();
  E_ASSERT_MSG(!IsEqual(length, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  if (IsEqual(length, static_cast<T>(1))) 
    return;

  (*this) *= static_cast<T>(1) / length;
}

template <typename T>
inline void Vector2<T>::Sign()								
{			
  x = -x;
  y = -y;
}

/*----------------------------------------------------------------------------------------------------------------------
Vector2 static methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline Vector2<T> Vector2<T>::AxisX()
{
  static Vector2<T> sX(1, 0);
  return sX;
}

template <typename T>
inline Vector2<T> Vector2<T>::AxisY()
{
  static Vector2<T> sY(0, 1);
  return sY;
}

// Note about Cross Product:
// Computing the cross product of two 2D vectors we obtain a new 3D vector. Its only component z is exactly its module.
// The cross product between two Vector2 is an optimization of the well known Vector2 cross product where both
// input vectors have zero z component. The result of that operation will always be a vector with zero x and y
// components and a z component equal to the module.
// This method returns a positive module if the rotation from vec1 towards vec2 is counter-clockwise. It returns
// a negative module otherwise. Thus CrossProduct(a,b) is equal to -CrossProduct(b,a).
template <typename T>
inline T Vector2<T>::Cross(const Vector2& a, const Vector2& b)	
{
  return a.x * b.y - a.y * b.x;
}

template <typename T>
inline T Vector2<T>::Dot(const Vector2& a, const Vector2& b)		
{ 
  return (a.x * b.x + a.y * b.y); 
}

template <typename T>
inline Vector2<T> Vector2<T>::Max(const Vector2& a, const Vector2& b)
{
  return Vector2<T>(
    a.x > b.x ? a.x : b.x, 
    a.y > b.y ? a.y : b.y);
}

template <typename T>
inline Vector2<T> Vector2<T>::Min(const Vector2& a, const Vector2& b)
{
  return Vector2<T>(
    a.x < b.x ? a.x : b.x, 
    a.y < b.y ? a.y : b.y);
}

template <typename T>
inline void Vector2<T>::MinMax(Vector2& min, Vector2& max, const Vector2<T>* pSource, const U32 count)
{
  E_ASSERT_MSG(count > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);
  min = max = pSource[0];
  for (U32 i = 1; i < count; ++i)
  {
    if (pSource[i].x > max.x) max.x = pSource[i].x;
    if (pSource[i].y > max.y) max.y = pSource[i].y;

    if (pSource[i].x < min.x) min.x = pSource[i].x;
    if (pSource[i].y < min.y) min.y = pSource[i].y;
  }
}

template <typename T>
inline Vector2<T> Vector2<T>::ZeroVector()
{
  static Vector2<T> sZeroVector;
  return sZeroVector;
}
}

/*----------------------------------------------------------------------------------------------------------------------
Vector2 types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Vector2<I32> Vector2i;
typedef Math::Vector2<F32> Vector2f;
typedef Math::Vector2<D64> Vector2d;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Vector2i)
E_DECLARE_POD(E::Vector2f)
E_DECLARE_POD(E::Vector2d)

#endif

