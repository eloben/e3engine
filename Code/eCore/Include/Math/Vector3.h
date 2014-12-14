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

// Created 29-Sep-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Vector3.h
This file defines the Vector3 class.
*/

#ifndef E_VECTOR3_H
#define E_VECTOR3_H

#include "Comparison.h"
#include <Assertion/Assert.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Vector3

Please note that this class has the following usage contract:

1. Floating point types are expected to be used with this class: F32, D64.
2. GetLength and Normalize are not supported on the I32 version.
3. MaxMin count must be greater than 0.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct Vector3
{
  Vector3();
  Vector3(T scalar);
  Vector3(T x, T y, T z);

  // Operators
  const T&        operator[](const size_t i) const;
  T&              operator[](const size_t i);
  bool            operator==(const Vector3& other) const;
  bool            operator==(T scalar) const;
  bool            operator!=(const Vector3& other) const;
  bool            operator!=(T scalar) const;
  Vector3         operator+(const Vector3& other) const;
  Vector3         operator+(T scalar) const;
  Vector3&        operator+=(const Vector3& other);
  Vector3&        operator+=(T scalar);
  Vector3         operator-() const;
  Vector3         operator-(const Vector3& other) const;
  Vector3         operator-(T scalar) const;
  Vector3&	      operator-=(const Vector3& other);
  Vector3&        operator-=(T scalar);
  Vector3         operator*(const Vector3& other) const;
  Vector3         operator*(T scalar) const;
  Vector3&        operator*=(const Vector3& other);
  Vector3&        operator*=(T scalar);
  Vector3         operator/(const Vector3& other) const;
  Vector3         operator/(T scalar) const;
  Vector3&        operator/=(const Vector3& other);
  Vector3&        operator/=(T scalar);
  Vector3         operator^(const Vector3& other) const;

  // Accessors
  T               GetLength() const;
  T               GetLengthSquared() const;
  bool            IsZero() const;
  void            Set(T scalar);
  void            Set(T x, T y, T z);
  void            SetZero();

  // Methods
  void            Normalize();
  void            Sign();

  // Static methods
  static Vector3  AxisX();
  static Vector3  AxisY();
  static Vector3  AxisZ();
  static Vector3  Cross(const Vector3& a, const Vector3& b);
  static T        Dot(const Vector3& a, const Vector3& b);
  static Vector3  Max(const Vector3& a, const Vector3& b);
  static Vector3  Min(const Vector3& a, const Vector3& b);
  static void     MinMax(Vector3& min, Vector3& max, const Vector3<T>* pSource, const U32 count);
  static Vector3  ZeroVector();

  // Members
  T x;
  T y;
  T z;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Vector3 initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline Vector3<T>::Vector3()
  : x(0)
  , y(0)
  , z(0)
{
}

template <typename T>
inline Vector3<T>::Vector3(T scalar)
  : x(scalar)
  , y(scalar)
  , z(scalar)
{
}

template <typename T>
inline Vector3<T>::Vector3(T x, T y, T z)
  : x(x)
  , y(y)
  , z(z)
{
}

/*----------------------------------------------------------------------------------------------------------------------
Vector3 operators
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline const T& Vector3<T>::operator[](const size_t i) const
{ 
  E_ASSERT_MSG(i < 3, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <typename T>
inline T& Vector3<T>::operator[](const size_t i)
{ 
  E_ASSERT_MSG(i < 3, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <typename T>
inline bool Vector3<T>::operator==(const Vector3& other) const
{ 
  return IsEqual(other.x, x) && IsEqual(other.y, y) && IsEqual(other.z, z);
}

template <typename T>
inline bool Vector3<T>::operator==(T scalar) const
{ 
  return IsEqual(scalar, x) && IsEqual(scalar, y) && IsEqual(scalar, z);
}

template <typename T>
inline bool Vector3<T>::operator!=(const Vector3& other) const
{ 
  return !((*this) == other);
}

template <typename T>
inline bool Vector3<T>::operator!=(T scalar) const
{ 
  return !((*this) == scalar);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator+(const Vector3& other) const
{
  return Vector3(x + other.x, y + other.y, z + other.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator+(T scalar) const		
{
  return Vector3(x + scalar, y + scalar, z + scalar);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3& other)		
{ 
  x += other.x;
  y += other.y;
  z += other.z; 
  return (*this);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator+=(T scalar)	
{
  x += scalar; 
  y += scalar; 
  z += scalar; 
  return (*this);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-() const	
{
  return Vector3(-x, -y, -z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3& other) const	
{
  return Vector3(x - other.x, y - other.y, z - other.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-(T scalar) const	
{
  return Vector3(x - scalar, y - scalar, z - scalar);
}

template <typename T>
inline Vector3<T>&	Vector3<T>::operator-=(const Vector3& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return (*this);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator-=(T scalar)
{
  x -= scalar;
  y -= scalar;
  z -= scalar; 
  return (*this);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator*(const Vector3& other) const	
{
  return Vector3(x * other.x, y * other.y, z * other.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator*(T scalar) const	
{
  return Vector3(x * scalar, y * scalar, z * scalar);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator*=(const Vector3& other)
{
  x *= other.x;
  y *= other.y;
  z *= other.z;
  return (*this);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator*=(T scalar)	
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return (*this);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator/(const Vector3& other) const	
{
  E_ASSERT_MSG(
    !IsEqual(other.x, static_cast<T>(0)) && 
    !IsEqual(other.y, static_cast<T>(0)) && 
    !IsEqual(other.z, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return Vector3(x / other.x, y / other.y, z / other.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator/(T scalar) const		
{
  E_ASSERT_MSG(!IsEqual(scalar, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return Vector3(x / scalar, y / scalar, z / scalar);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator/=(const Vector3& other)
{
  E_ASSERT_MSG(
    !IsEqual(other.x, static_cast<T>(0)) && 
    !IsEqual(other.y, static_cast<T>(0)) && 
    !IsEqual(other.z, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  x /= other.x;
  y /= other.y;
  z /= other.z;
  return (*this);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator/=(T scalar)
{
  E_ASSERT_MSG(!IsEqual(scalar, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  x /= scalar;
  y /= scalar;
  z /= scalar;
  return (*this);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator^(const Vector3& other) const	
{
  return Vector3<T>::Cross(*this, other);
}

/*----------------------------------------------------------------------------------------------------------------------
Vector3 accessors
----------------------------------------------------------------------------------------------------------------------*/

// Take note that integer version won't compile:
// sqrt works with long, floats and doubles hence it can not be used with integers. Length and Normalize won't work
template <typename T>
inline T Vector3<T>::GetLength() const	
{ 
  return Math::Sqrt(GetLengthSquared()); 
}

template <typename T>
inline T Vector3<T>::GetLengthSquared() const	
{ 
  return x * x + y * y + z * z; 
}

template <typename T>
inline bool Vector3<T>::IsZero() const
{
  return !x && !y && !z;
}

template <typename T>
inline void Vector3<T>::Set(T scalar)
{
  x = scalar;
  y = scalar;
  z = scalar;
}

template <typename T>
inline void Vector3<T>::Set(T x, T y, T z)
{
  (*this).x = x;
  (*this).y = y;
  (*this).z = z;
}

template <typename T>
inline void Vector3<T>::SetZero()
{
  x = 0;
  y = 0;
  z = 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Vector3 methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline void Vector3<T>::Normalize()								
{			
  T length = GetLength();
  E_ASSERT_MSG(!IsEqual(length, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  if (IsEqual(length, static_cast<T>(1))) 
    return;

  (*this) *= static_cast<T>(1) / length;
}

template <typename T>
inline void Vector3<T>::Sign()								
{			
  x = -x;
  y = -y;
  z = -z;
}

/*----------------------------------------------------------------------------------------------------------------------
Vector3 static methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline Vector3<T> Vector3<T>::AxisX()
{
  static Vector3<T> sX(1, 0, 0);
  return sX;
}

template <typename T>
inline Vector3<T> Vector3<T>::AxisY()
{
  static Vector3<T> sY(0, 1, 0);
  return sY;
}

template <typename T>
inline Vector3<T> Vector3<T>::AxisZ()
{
  static Vector3<T> sZ(0, 0, 1);
  return sZ;
}

template <typename T>
inline Vector3<T> Vector3<T>::Cross(const Vector3& a, const Vector3& b)	
{
  return Vector3<T>(a.y * b.z - a.z * b.y,	a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template <typename T>
inline T Vector3<T>::Dot(const Vector3& a, const Vector3& b)		
{ 
  return (a.x * b.x + a.y * b.y + a.z * b.z); 
}

template <typename T>
inline Vector3<T> Vector3<T>::Max(const Vector3& a, const Vector3& b)
{
  return Vector3<T>(
    a.x > b.x ? a.x : b.x, 
    a.y > b.y ? a.y : b.y, 
    a.z > b.z ? a.z : b.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::Min(const Vector3& a, const Vector3& b)
{
  return Vector3<T>(
    a.x < b.x ? a.x : b.x, 
    a.y < b.y ? a.y : b.y, 
    a.z < b.z ? a.z : b.z);
}

template <typename T>
inline void Vector3<T>::MinMax(Vector3& min, Vector3& max, const Vector3<T>* pSource, const U32 count)
{
  E_ASSERT_MSG(count > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);
  min = max = pSource[0];
  for (U32 i = 1; i < count; ++i)
  {
    if (pSource[i].x > max.x) max.x = pSource[i].x;
    if (pSource[i].y > max.y) max.y = pSource[i].y;
    if (pSource[i].z > max.z) max.z = pSource[i].z;

    if (pSource[i].x < min.x) min.x = pSource[i].x;
    if (pSource[i].y < min.y) min.y = pSource[i].y;
    if (pSource[i].z < min.z) min.z = pSource[i].z;
  }
}

template <typename T>
inline Vector3<T> Vector3<T>::ZeroVector()
{
  static Vector3<T> sZeroVector;
  return sZeroVector;
}
}

/*----------------------------------------------------------------------------------------------------------------------
Vector3 types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Vector3<F32> Vector3f;
typedef Math::Vector3<D64> Vector3d;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Vector3f)
E_DECLARE_POD(E::Vector3d)

#endif
