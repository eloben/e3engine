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

// Created 15-Nov-2011 by Elías Lozada-Benavente (updated 30-Dec-2013)
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Quaternion.h
This file defines the Quaternion class.
*/

#ifndef E_QUATERNION_H
#define E_QUATERNION_H

#include "Comparison.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <Serialization/ISerializer.h>
#include <Assertion/Assert.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Quaternion

Please note that this class has the following usage contract: 

1. Floating point types are expected to be used with this class: F32, D64.
2. The two vectors version of SetRotation requires unit vectors.
3. The chosen rotation order for Euler angles is Roll Pitch Yaw (Z, X, Y) which is the same as in the DirectX SDK 
function D3DXQuaternionRotationYawPitchRoll.

Note: quaternions do not have handness although to be consistent with Matrix4 rotations, a rotation order must be 
used consistently for Euler angles.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
struct Quaternion
{
  Quaternion();	
  Quaternion(T x, T y, T z, T w);

  // Operators
  const T&          operator[](const size_t i) const;
  T&                operator[](const size_t i);
  bool              operator==(const Quaternion& other) const;
  bool              operator!=(const Quaternion& other) const;
  Quaternion        operator+(const Quaternion& other);
  Quaternion&       operator+=(const Quaternion& other);
  Quaternion        operator-() const;
  Quaternion        operator-(const Quaternion& other);
  Quaternion&       operator-=(const Quaternion& other);
  Quaternion        operator*(const Quaternion& other);
  Quaternion        operator*(T scalar) const;
  Quaternion&       operator*=(const Quaternion& other);
  Quaternion&       operator*=(const Vector3<T>& scalar);
  Quaternion&       operator*=(T scalar);
  Quaternion        operator~() const;

  // Accessors
  T                 GetAxisAndAngle(Vector3<T>& scalar) const;
  T                 GetLength() const;
  T                 GetLengthSquared() const;
  void              GetRotation(Matrix4<T>& m) const;
  bool              IsIdentity() const;
  void              Set(T x, T y, T z, T w);
  void              SetIdentity();
  void              SetRotation(const Matrix4<T>& m);
  void              SetRotation(T pitch, T yaw, T roll);
  void              SetRotation(const Vector3<T>& eulerAngles);
  void              SetRotation(T radians, const Vector3<T>& axis);
  void              SetRotation(const Vector3<T>& a, const Vector3<T>& b);

  // Methods
  void              Normalize();
  void              Regenerate();

  // Static methods
  static T          Dot(const Quaternion& q1, const Quaternion& q2);
  static Quaternion Lerp(const Quaternion& qFrom, const Quaternion& qTo, T t);
  static Vector3<T> Rotate(const Quaternion& q, const Vector3<T>& scalar);
  static Quaternion Slerp(const Quaternion& qFrom, const Quaternion& qTo, T t);

  // Members
  T x;
  T y;
  T z;
  T w;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Quaternion initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline Quaternion<T>::Quaternion()
  : x(0)
  , y(0)
  , z(0)
  , w(1)
{}

template <class T>

inline Quaternion<T>::Quaternion(T x, T y, T z, T w)
  : x(x)
  , y(y)
  , z(z)
  , w(w)
{}

/*----------------------------------------------------------------------------------------------------------------------
Quaternion operators
----------------------------------------------------------------------------------------------------------------------*/  

template <typename T>
inline const T& Quaternion<T>::operator[](const size_t i) const
{ 
  E_ASSERT_MSG(i < 4, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <typename T>
inline T& Quaternion<T>::operator[](const size_t i)
{ 
  E_ASSERT_MSG(i < 4, E_ASSERT_MSG_MATH_OUT_OF_BOUNDS_VALUE);
  return *(&x + i);
}

template <class T>
inline bool Quaternion<T>::operator==(const Quaternion& other) const
{
  return(
    IsEqual(x, other.x) &&
    IsEqual(y, other.y) &&
    IsEqual(z, other.z) &&
    IsEqual(w, other.w));
}

template <class T>
inline bool Quaternion<T>::operator!=(const Quaternion& other) const
{ 
  return !((*this) == other); 
}

template <class T>
inline Quaternion<T> Quaternion<T>::operator+(const Quaternion& other)
{
  return Quaternion<T>(x + other.x, y + other.y, z + other.z, w + other.w); 
}

template <class T>
inline Quaternion<T>& Quaternion<T>::operator+=(const Quaternion& other)
{
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;
  return (*this);
}

// Negation (additive inverse)
template <class T>
inline Quaternion<T> Quaternion<T>::operator-() const
{
  return Quaternion<T>(-x, -y, -z, -w);
}

template <class T>
inline Quaternion<T> Quaternion<T>::operator-(const Quaternion& other)
{ 
  return Quaternion<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}
template <class T>
inline Quaternion<T>& Quaternion<T>::operator-=(const Quaternion& other)
{ 
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;
  return (*this);
}

template <class T>
inline Quaternion<T> Quaternion<T>::operator*(const Quaternion& other)
{
  return Quaternion<T>(	
    y * other.z - z * other.y + w * other.x + x * other.w,
    z * other.x - x * other.z + w * other.y + y * other.w,
    x * other.y - y * other.x + w * other.z + z * other.w,
    w * other.w - x * other.x - y * other.y - z * other.z);
}

template <class T>
inline Quaternion<T> Quaternion<T>::operator*(T scalar) const
{
  return Quaternion<T>(x * scalar, y * scalar, z * scalar, w * scalar);
}

template <class T>
inline Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& other)
{
  Quaternion<T>q(
    y * other.z - z * other.y + w * other.x + x * other.w,
    z * other.x - x * other.z + w * other.y + y * other.w,
    x * other.y - y * other.x + w * other.z + z * other.w,
    w * other.w - x * other.x - y * other.y - z * other.z);
  (*this) = q;
  return (*this);
}

// Vectorial product
template <class T>
inline Quaternion<T>& Quaternion<T>::operator*=(const Vector3<T>& scalar)
{
  Quaternion<T>q(
    w * scalar.x + y * scalar.z - z * scalar.y,
    w * scalar.y + z * scalar.x - x * scalar.z,
    w * scalar.z + x * scalar.y - y * scalar.x,
    -(x * scalar.x + y * scalar.y + z * scalar.z));
  (*this) = q;
  return (*this);
}

template <class T>
inline Quaternion<T>& Quaternion<T>::operator*=(T scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  w *= scalar;
  return (*this);
}

// Conjugation (spatial inverse)
template <class T>
inline Quaternion<T> Quaternion<T>::operator~() const
{ 
  return Quaternion<T>(-x, -y, -z, w);
}

/*----------------------------------------------------------------------------------------------------------------------
Quaternion accessors
----------------------------------------------------------------------------------------------------------------------*/

// Gets the axis and the angle of the rotation represented by this quaternion.
template <class T>
inline T Quaternion<T>::GetAxisAndAngle(Vector3<T>& scalar) const
{
  T angle = 2 * Math::Acos(w);
  const T factor = static_cast<T>(1) / Math::Sqrt(static_cast<T>(1) - w * w);
  scalar.x = x * factor;
  scalar.y = y * factor;
  scalar.z = z * factor;

  return angle;
}

template <class T>
inline T Quaternion<T>::GetLength() const
{ 
  return Math::Sqrt(GetLengthSquared()); 
}

template <class T>
inline T Quaternion<T>::GetLengthSquared() const
{ 
  return x * x + y * y + z * z + w * w;
}

// Gets the equivalent rotation matrix to this quaternion.
template <class T>
inline void Quaternion<T>::GetRotation(Matrix4<T>& m) const
{
  // If m is guaranteed to be a unit quaternion, s will always
  // be 2. In that case, this calculation can be optimized out.
  const T s = static_cast<T>(2);

  // Pre-calculate coordinate products
  const T xx = x * x * s;
  const T yy = y * y * s;
  const T zz = z * z * s;
  const T xy = x * y * s;
  const T xz = x * z * s;
  const T yz = y * z * s;
  const T wx = w * x * s;
  const T wy = w * y * s;
  const T wz = w * z * s;

  // Calculate 3x3 matrix from orthonormal basis
  m[0] = static_cast<T>(1) - (yy + zz);
  m[1] = xy + wz;
  m[2] = xz - wy;

  m[4] = xy - wz;
  m[5] = static_cast<T>(1) - (xx + zz);
  m[6] = yz + wx;

  m[8]  = xz + wy;
  m[9]  = yz - wx;
  m[10] = static_cast<T>(1) - (xx + yy);
}

template <typename T>
inline bool Quaternion<T>::IsIdentity() const
{
  return(
    IsEqual(x, static_cast<T>(0)) &&
    IsEqual(y, static_cast<T>(0)) &&
    IsEqual(z, static_cast<T>(0)) &&
    IsEqual(w, static_cast<T>(1)));
}

template <typename T>
inline void Quaternion<T>::Set(T x, T y, T z, T w)
{
  (*this).x = x;
  (*this).y = y;
  (*this).z = z;
  (*this).w = w;
}

template <class T>
inline void Quaternion<T>::SetIdentity()
{
  x = 0; 
  y = 0; 
  z = 0; 
  w = 1;
}

// Sets a quaternion from a rotation matrix (translation part is ignored)
template <class T>
inline void Quaternion<T>::SetRotation(const Matrix4<T>& m)
{
  T trace = m[0] + m[5] + m[10];

  if (IsEqual(trace, static_cast<T>(0)))
  {
    T s = Math::Sqrt(trace + static_cast<T>(1));
    w = static_cast<T>(0.5) * s;
    s = static_cast<T>(0.5) / s;
    x = (m[6] - m[9]) * s;
    y = (m[8] - m[2]) * s;
    z = (m[1] - m[4]) * s;
  }
  else
  {
    // |w| <= 1/2
    U32 i = 0;
    if (m[5] > m[0])
      i = 1;
    if (m[10] > m(i, i))
      i = 2;
    U32 j = (i+1) % 3;
    U32 k = (j+1) % 3;

    T s = Math::Sqrt(m(i, i) - m(j, j) - m(k, k) + static_cast<T>(1));
    T* pQ[3] = {&x, &y, &z};
    *pQ[i]  = static_cast<T>(0.5) * s;
    s       = static_cast<T>(0.5) / s;
    *pQ[j]  = (m(i, j) + m(j, i)) * s;
    *pQ[k]  = (m(i, k) + m(k, i)) * s;
    w       = (m(j, k) - m(k, j)) * s;
  }
}

template <class T>
inline void Quaternion<T>::SetRotation(T pitch, T yaw, T roll)
{
  const T hx = static_cast<T> (0.5 * pitch);
  const T hy = static_cast<T> (0.5 * yaw);
  const T hz = static_cast<T> (0.5 * roll);
  const T sinR = Math::Sin(hz);
  const T cosR = Math::Cos(hz);
  const T sinY = Math::Sin(hy);
  const T cosY = Math::Cos(hy);
  const T sinP = Math::Sin(hx);
  const T cosP = Math::Cos(hx);
  /*
  // Pitch Yaw Roll
  x = sinP * cosY * cosR + cosP * sinY * sinR;
  y = cosP * sinY * cosR - sinP * cosY * sinR;
  z = cosP * cosY * sinR + sinP * sinY * cosR;
  w = cosP * cosY * cosR - sinP * sinY * sinR;
  */
  x = sinP * cosY * cosR + cosP * sinY * sinR;
  y = cosP * sinY * cosR - sinP * cosY * sinR;
  z = cosP * cosY * sinR - sinP * sinY * cosR;
  w = cosP * cosY * cosR + sinP * sinY * sinR;    
}

template <class T>
inline void Quaternion<T>::SetRotation(const Vector3<T>& eulerAngles)
{
  SetRotation(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

// Sets a rotation of a certain angle in radians around an axis
template <class T>
inline void Quaternion<T>::SetRotation(T radians, const Vector3<T>& axis)
{
  const T halfa = radians * static_cast<T>(0.5);
  const T s = Math::Sin(halfa);
  x = axis.x * s;
  y = axis.y * s;
  z = axis.z * s;
  w = Math::Cos(halfa);
}

// Sets the shortest rotation between between two vectors
template <class T>
inline void Quaternion<T>::SetRotation(const Vector3<T>& a, const Vector3<T>& b)
{
  E_ASSERT_MSG(
    IsEqual(a.GetLength(), static_cast<T>(1)) &&
    IsEqual(b.GetLength(), static_cast<T>(1)), E_ASSERT_MSG_MATH_EQUAL_TO_ONE_VALUE);

  Vector3<T> axis = Vector3<T>::Cross(a, b);
  x = axis.x;
  y = axis.y;
  z = axis.z;
  w = static_cast<T>(1) + Vector3<T>::Dot(a, b);
  Normalize();
}

/*----------------------------------------------------------------------------------------------------------------------
Quaternion methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void Quaternion<T>::Normalize()
{
  T length = GetLength();
  E_ASSERT_MSG(!IsEqual(length, static_cast<T>(0)), E_ASSERT_MSG_MATH_EQUAL_TO_ZERO_VALUE);
  if (IsEqual(length, static_cast<T>(1))) 
    return;

  (*this) *= static_cast<T>(1) / length;
}

template <typename T>
inline void Quaternion<T>::Regenerate()
{
  x = IsEqual(x, static_cast<T>(0)) ? 0 : (IsEqual(x, static_cast<T>(1)) ? 1 : x);
  y = IsEqual(y, static_cast<T>(0)) ? 0 : (IsEqual(y, static_cast<T>(1)) ? 1 : y);
  z = IsEqual(z, static_cast<T>(0)) ? 0 : (IsEqual(z, static_cast<T>(1)) ? 1 : z);
  w = IsEqual(w, static_cast<T>(0)) ? 0 : (IsEqual(w, static_cast<T>(1)) ? 1 : w);
}

/*----------------------------------------------------------------------------------------------------------------------
Quaternion static methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>   
inline T Quaternion<T>::Dot(const Quaternion& q1, const Quaternion& q2)
{
  return (q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w);
}

template <class T>
inline Quaternion<T> Quaternion<T>::Lerp(const Quaternion& qFrom, const Quaternion& qTo, T t)
{
  if (t <= static_cast<T>(0))
  {
    return qFrom;
  }

  if (t >= static_cast<T>(1) || qFrom == qTo)
  {
    return qTo;
  }

  T cosOmega = Quaternion<T>::Dot(qFrom, qTo);
  T t0 = static_cast<T>(1) - t;
  T t1 = (cosOmega >= static_cast<T>(0)) ? t : -t;

  Quaternion<T> q(
    t0 * qFrom.x + t1 * qTo.x,
    t0 * qFrom.y + t1 * qTo.y,
    t0 * qFrom.z + t1 * qTo.z,
    t0 * qFrom.w + t1 * qTo.w);
  q.Normalize();

  return q;
}

template <class T>
inline Vector3<T> Quaternion<T>::Rotate(const Quaternion& q, const Vector3<T>& scalar)
{
  // nVidia SDK implementation
  Vector3<T> uv;
  Vector3<T> uuv;
  Vector3<T> qv(q.x, q.y, q.z);
  uv = Vector3<T>::Cross(qv, scalar);
  uuv = Vector3<T>::Cross(qv, uv);
  uv *= (static_cast<T>(2) * q.w);
  uuv *= static_cast<T>(2);
  return scalar + uv + uuv;
}

template <class T>
inline Quaternion<T> Quaternion<T>::Slerp(const Quaternion& qFrom, const Quaternion& qTo, T t)
{
  if (t <= static_cast<T>(0))
  {
    return qFrom;
  }

  if (t >= static_cast<T>(1) || qFrom == qTo)
  {
    return qTo;
  }

  Quaternion<T> qTemp;
  T cosOmega = Quaternion<T>::Dot(qFrom, qTo);
  if (cosOmega < static_cast<T>(0))
  {
    qTemp = -qTo;
    cosOmega = -cosOmega;
  }
  else
  {
    qTemp = qTo;
  }

  T t0, t1;
  if (IsEqual(cosOmega, static_cast<T>(1)))
  {
    // Standard case (slerp)
    T omega = acos(cosOmega);
    T invSinOmega = static_cast<T>(1) / Math::Sin(omega);
    t0 = sin((static_cast<T>(1) - t) * omega) * invSinOmega;
    t1 = sin(t * omega) * invSinOmega;
  }
  else
  {
    // Two cases:
    // 1. qFrom and qTo are very close (cosOmega ~= 1), so we can do a linear  interpolation safely.
    // 2. qFrom and qTo are almost inverse of each other (fCos ~= -1), there are an infinite number 
    //    of possible interpolations, so just use linear interpolation.
    t0 = static_cast<T>(1) - t;
    t1 = t;
  }

  return (qFrom * t0) + (qTemp * t1);
}
}

/*----------------------------------------------------------------------------------------------------------------------
Quaternion types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Quaternion<F32> Quatf;
typedef Math::Quaternion<D64> Quatd;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Quatf)
E_DECLARE_POD(E::Quatd)

#endif
