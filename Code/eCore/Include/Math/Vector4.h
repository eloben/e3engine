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

// Created 09-Aug-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Vector4.h
This file defines the Vector4 class.
*/

#ifndef E_VECTOR4_H
#define E_VECTOR4_H

#include "Comparison.h"
#include <Assertion/Assert.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Vector4

Please note that this class has the following usage contract:

1. Floating point types are expected to be used with this class: F32, D64.
2. GetLength and Normalize are not supported on the I32 version.
3. MaxMin count must be greater than 0.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct Vector4
{
  Vector4();
  Vector4(T scalar);
  Vector4(T x, T y, T z, T w);

  // Operators
  const T&        operator[](const size_t i) const;
  T&              operator[](const size_t i);
  bool            operator==(const Vector4& other) const;
  bool            operator==(T scalar) const;
  bool            operator!=(const Vector4& other) const;
  bool            operator!=(T scalar) const;
  Vector4         operator+(const Vector4& other) const;
  Vector4         operator+(T scalar) const;
  Vector4&        operator+=(const Vector4& other);
  Vector4&        operator+=(T scalar);
  Vector4         operator-() const;
  Vector4         operator-(const Vector4& other) const;
  Vector4         operator-(T scalar) const;
  Vector4&	      operator-=(const Vector4& other);
  Vector4&        operator-=(T scalar);
  Vector4         operator*(const Vector4& other) const;
  Vector4         operator*(T scalar) const;
  Vector4&        operator*=(const Vector4& other);
  Vector4&        operator*=(T scalar);
  Vector4         operator/(const Vector4& other) const;
  Vector4         operator/(T scalar) const;
  Vector4&        operator/=(const Vector4& other);
  Vector4&        operator/=(T scalar);

  // Accessors
  T               GetLength() const;
  T               GetLengthSquared() const;
  bool            IsZero() const;
  void            Set(T scalar);
  void            Set(T x, T y, T z, T w);
  void            SetZero();

  // Methods
  void            Normalize();
  void            Sign();

  // Static methods
  static Vector4  AxisX();
  static Vector4  AxisY();
  static Vector4  AxisZ();
  static Vector4  AxisW();
  static T        Dot(const Vector4& a, const Vector4& b);
  static Vector4  Max(const Vector4& a, const Vector4& b);
  static Vector4  Min(const Vector4& a, const Vector4& b);
  static void     MinMax(Vector4& min, Vector4& max, const Vector4<T>* pSource, const U32 count);
  static Vector4  ZeroVector();

  // Members
  T x;
  T y;
  T z;
  T w;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Vector4 initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
inline Vector4<T>::Vector4()
  : x(0)
  , y(0)
  , z(0)
  , w(0)
{
}

template <typename T>
inline Vector4<T>::Vector4(T scalar)
  : x(scalar)
  , y(scalar)
  , z(scalar)
  , w(scalar)
{
}

template <typename T>
inline Vector4<T>::Vector4(T x, T y, T z, T w)
  : x(x)
  , y(y)
  , z(z)
  , w(w)
{
}

/*----------------------------------------------------------------------------------------------------------------------
Vector4 operators
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline const T& Vector4<T>::operator[](const size_t i) const
{ 
  E_ASSERT_MSG(i < 4, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <typename T>
inline T& Vector4<T>::operator[](const size_t i)
{ 
  E_ASSERT_MSG(i < 4, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <typename T>
inline bool Vector4<T>::operator==(const Vector4& other) const
{ 
  return IsEqual(other.x, x) && IsEqual(other.y, y) && IsEqual(other.z, z) && IsEqual(other.w, w);
}

template <typename T>
inline bool Vector4<T>::operator==(T scalar) const
{ 
  return IsEqual(scalar, x) && IsEqual(scalar, y) && IsEqual(scalar, z) && IsEqual(scalar, w);
}

template <typename T>
inline bool Vector4<T>::operator!=(const Vector4& other) const
{ 
  return !((*this) == other);
}

template <typename T>
inline bool Vector4<T>::operator!=(T scalar) const
{ 
  return !((*this) == scalar);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator+(const Vector4& other) const
{
  return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator+(T scalar) const		
{
  return Vector4(x + scalar, y + scalar, z + scalar, w + scalar);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector4& other)		
{ 
  x += other.x;
  y += other.y;
  z += other.z; 
  w += other.w; 
  return (*this);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator+=(T scalar)	
{
  x += scalar; 
  y += scalar; 
  z += scalar; 
  w += scalar; 
  return (*this);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator-() const	
{
  return Vector4(-x, -y, -z, -w);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator-(const Vector4& other) const	
{
  return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator-(T scalar) const	
{
  return Vector4(x - scalar, y - scalar, z - scalar, w - scalar);
}

template <typename T>
inline Vector4<T>&	Vector4<T>::operator-=(const Vector4& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;
  return (*this);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator-=(T scalar)
{
  x -= scalar;
  y -= scalar;
  z -= scalar;
  w -= scalar; 
  return (*this);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator*(const Vector4& other) const	
{
  return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator*(T scalar) const	
{
  return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator*=(const Vector4& other)
{
  x *= other.x;
  y *= other.y;
  z *= other.z;
  w *= other.w;
  return (*this);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator*=(T scalar)	
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  w *= scalar;
  return (*this);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator/(const Vector4& other) const	
{
  E_ASSERT_MSG(
    !IsEqual(other.x, static_cast<T>(0)) && 
    !IsEqual(other.y, static_cast<T>(0)) && 
    !IsEqual(other.z, static_cast<T>(0)) &&
    !IsEqual(other.w, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator/(T scalar) const		
{
  E_ASSERT_MSG(!IsEqual(scalar, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator/=(const Vector4& other)
{
  E_ASSERT_MSG(
    !IsEqual(other.x, static_cast<T>(0)) && 
    !IsEqual(other.y, static_cast<T>(0)) && 
    !IsEqual(other.z, static_cast<T>(0)) &&
    !IsEqual(other.w, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  x /= other.x;
  y /= other.y;
  z /= other.z;
  w /= other.w;
  return (*this);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator/=(T scalar)
{
  E_ASSERT_MSG(!IsEqual(scalar, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  x /= scalar;
  y /= scalar;
  z /= scalar;
  w /= scalar;
  return (*this);
}

/*----------------------------------------------------------------------------------------------------------------------
Vector4 accessors
----------------------------------------------------------------------------------------------------------------------*/

// Take note that integer version won't compile:
// sqrt works with long, floats and doubles hence it can not be used with integers. Length and Normalize won't work
template <typename T>
inline T Vector4<T>::GetLength() const	
{ 
  return Math::Sqrt(GetLengthSquared()); 
}

template <typename T>
inline T Vector4<T>::GetLengthSquared() const	
{ 
  return x * x + y * y + z * z + w * w; 
}

template <typename T>
inline bool Vector4<T>::IsZero() const
{
  return !x && !y && !z && !w;
}

template <typename T>
inline void Vector4<T>::Set(T scalar)
{
  x = scalar;
  y = scalar;
  z = scalar;
  w = scalar;
}

template <typename T>
inline void Vector4<T>::Set(T x, T y, T z, T w)
{
  (*this).x = x;
  (*this).y = y;
  (*this).z = z;
  (*this).w = w;
}

template <typename T>
inline void Vector4<T>::SetZero()
{
  x = 0;
  y = 0;
  z = 0;
  w = 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Vector4 methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline void Vector4<T>::Normalize()								
{			
  T length = GetLength();
  E_ASSERT_MSG(!IsEqual(length, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  if (IsEqual(length, static_cast<T>(1))) 
    return;

  (*this) *= static_cast<T>(1) / length;
}

template <typename T>
inline void Vector4<T>::Sign()								
{			
  x = -x;
  y = -y;
  z = -z;
  w = -w;
}

/*----------------------------------------------------------------------------------------------------------------------
Vector4 static methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline Vector4<T> Vector4<T>::AxisX()
{
  static Vector4<T> sX(1, 0, 0, 0);
  return sX;
}

template <typename T>
inline Vector4<T> Vector4<T>::AxisY()
{
  static Vector4<T> sY(0, 1, 0, 0);
  return sY;
}

template <typename T>
inline Vector4<T> Vector4<T>::AxisZ()
{
  static Vector4<T> sZ(0, 0, 1, 0);
  return sZ;
}

template <typename T>
inline Vector4<T> Vector4<T>::AxisW()
{
  static Vector4<T> sW(0, 0, 0, 1);
  return sW;
}

template <typename T>
inline T Vector4<T>::Dot(const Vector4& a, const Vector4& b)		
{ 
  return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

template <typename T>
inline Vector4<T> Vector4<T>::Max(const Vector4& a, const Vector4& b)
{
  return Vector4<T>(
    a.x > b.x ? a.x : b.x, 
    a.y > b.y ? a.y : b.y, 
    a.z > b.z ? a.z : b.z,
    a.w > b.w ? a.w : b.w);
}

template <typename T>
inline Vector4<T> Vector4<T>::Min(const Vector4& a, const Vector4& b)
{
  return Vector4<T>(
    a.x < b.x ? a.x : b.x, 
    a.y < b.y ? a.y : b.y, 
    a.z < b.z ? a.z : b.z,
    a.w < b.w ? a.w : b.w);
}

template <typename T>
inline void Vector4<T>::MinMax(Vector4& min, Vector4& max, const Vector4<T>* pSource, const U32 count)
{
  E_ASSERT_MSG(count > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);
  min = max = pSource[0];
  for (U32 i = 1; i < count; ++i)
  {
    if (pSource[i].x > max.x) max.x = pSource[i].x;
    if (pSource[i].y > max.y) max.y = pSource[i].y;
    if (pSource[i].z > max.z) max.z = pSource[i].z;
    if (pSource[i].w > max.w) max.w = pSource[i].w;

    if (pSource[i].x < min.x) min.x = pSource[i].x;
    if (pSource[i].y < min.y) min.y = pSource[i].y;
    if (pSource[i].z < min.z) min.z = pSource[i].z;
    if (pSource[i].w < min.w) min.w = pSource[i].w;
  }
}

template <typename T>
inline Vector4<T> Vector4<T>::ZeroVector()
{
  static Vector4<T> sZeroVector;
  return sZeroVector;
}
}

/*----------------------------------------------------------------------------------------------------------------------
Vector4 types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Vector4<F32> Vector4f;
typedef Math::Vector4<D64> Vector4d;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Vector4f)
E_DECLARE_POD(E::Vector4d)

#endif
