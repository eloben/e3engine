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

// Created 28-Sep-2012 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Plane.h
This file defines a geometry plane class.
*/

#ifndef E_PLANE_H
#define E_PLANE_H

#include "Vector3.h"

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Plane

Please note that this class has the following usage contract: 

1. Floating point types are expected to be used with this class: F32, D64.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class Plane
{
public:
  Plane();	
  Plane(T a, T b, T c, T d);
  Plane(const Vector3<T>& normal, T distance);	
  Plane(const Vector3<T>& normal, const Vector3<T>& pointInPlane);		
  Plane(const Vector3<T>& pointInPlaneA, const Vector3<T>& pointInPlaneB, const Vector3<T>& pointInPlaneC);

  // Accessors
  T                 GetDistance() const;
  T                 GetDistanceToPoint(const Vector3<T>& point) const;
  Vector3<T>        GetProjectedPoint(const Vector3<T>& point) const;
  const Vector3<T>& GetNormal() const;
  void              SetDistance(T v);
  void              SetNormal(const Vector3<T>& v);

  // Methods
  void              Normalize();

private:
  Vector3<T>        mNormal;
  T			            mDistance;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Plane initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline Plane<T>::Plane()
  : mDistance(0)
{}

// Implicit plane equation constructor: Ax + By + Cz + D = 0
template <typename T>  
inline Plane<T>::Plane(T a, T b, T c, T d)
  : mNormal(a, b, c)
  , mDistance(d)
{}

// Normal and distance to plane constructor
template <typename T> 
inline Plane<T>::Plane(const Vector3<T>& normal, T distance)
  : mNormal(normal.x, normal.y, normal.z)
  , mDistance(d)
{}

// Normal and point in plane constructor
template <typename T>
inline Plane<T>::Plane(const Vector3<T>& normal, const Vector3<T>& pointInPlane)
  : mNormal(normal.x, normal.y, normal.z)
  , mDistance(0)
{
  mDistance = -Vector3<T>::DotProduct(mNormal, Vector3<T>(pointInPlane.x, pointInPlane.y, pointInPlane.z));
}

// 3 plane points constructor
template <typename T>  
inline Plane<T>::Plane(const Vector3<T>& pointInPlaneA, const Vector3<T>& pointInPlaneB, const Vector3<T>& pointInPlaneC)
  : mDistance(0)
{
  Vector3<T> v1(
    pointInPlaneA.x - pointInPlaneB.x,
    pointInPlaneA.y - pointInPlaneB.y,
    pointInPlaneA.z - pointInPlaneB.z);
  Vector3<T> v2>(
    pointInPlaneC.x - pointInPlaneB.x,
    pointInPlaneC.y - pointInPlaneB.y,
    pointInPlaneC.z - pointInPlaneB.z);

  mNormal = Vector3<T>::Cross(v1, v2);
  mNormal.Normalize();
  mDistance = -Vector3<T>::Dot(mNormal, Vector3<T>(pointInPlaneB.x, pointInPlaneB.y, pointInPlaneB.z)); 
}

/*----------------------------------------------------------------------------------------------------------------------
Plane accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline T Plane<T>::GetDistance() const
{
  return mDistance;
}

template <typename T>
inline T Plane<T>::GetDistanceToPoint(const Vector3<T>& point) const
{
  return Vector3<T>::Dot(mNormal, Vector3<T>(point.x, point.y, point.z)) + mDistance;
}

template <typename T>
inline Vector3<T> Plane<T>::GetProjectedPoint(const Vector3<T>& point) const
{
  return (Vector3<T>(point.x, point.y, point.z) - mNormal * GetDistanceToPoint(point));
}

template <typename T>
inline const Vector3<T>& Plane<T>::GetNormal() const
{
  return mNormal;
}

template <typename T>
inline void Plane<T>::SetDistance(T v)
{
  mDistance = v;
}

template <typename T>
inline void Plane<T>::SetNormal(const Vector3<T>& v)
{
  mNormal = v;
}

/*----------------------------------------------------------------------------------------------------------------------
Plane methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline void Plane<T>::Normalize()
{
  T length = mNormal.GetLength();
  E_ASSERT_MSG(!IsEqual(length, static_cast<T>(0)), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  if (IsEqual(length, static_cast<T>(1))) return;

  T invLength = static_cast<T>(1) / length;
  mNormal *= invLength;
  mDistance *= invLength;
}
}

/*----------------------------------------------------------------------------------------------------------------------
Plane types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Plane<F32> Planef;
typedef Math::Plane<D64> Planed;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Planef)
E_DECLARE_POD(E::Planed)

#endif
