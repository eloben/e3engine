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

// Created 29-Sep-2012 by Elías Lozada-Benavente (updated 30-Dec-2013)
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Sphere.h
This file defines a geometry sphere class. This implementation is based on the original by:
Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
*/

#ifndef E3_SPHERE_H
#define E3_SPHERE_H

#include "Vector3.h"

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Sphere

Please note that this class has the following usage contract: 

1. Floating point types are expected to be used with this class: F32, D64.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class Sphere
{
public:
  Sphere();
  Sphere(const Vector3<T>& origin, T radius);

  // Accessors
  bool              AddPoint(const Vector3<T>& point);
  const Vector3<T>& GetOrigin() const;
  T                 GetRadius() const;
  bool              IsContained(const Vector3<T>& point) const;
  void              SetOrigin(const Vector3<T>& v);
  void              SetPoints(const Vector3<T>* pPoints, U32 count);
  void              SetRadius(T v);

private:
  Vector3<T>	      mOrigin;
  T			            mRadius;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Sphere initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

// Normal and radius constructor
template <typename T>
inline Sphere<T>::Sphere()
  : mRadius(-1)
{}

// Normal and radius constructor
template <typename T>  
inline Sphere<T>::Sphere(const Vector3<T>& origin, T radius)
  : mOrigin(origin.x, origin.y, origin.z)
  , mRadius(radius)
{}

/*----------------------------------------------------------------------------------------------------------------------
Sphere accessors
----------------------------------------------------------------------------------------------------------------------*/

// Returns true if a new point is added to the sphere or false if the added point is already contained.
template <typename T>
inline bool Sphere<T>::AddPoint(const Vector3<T>& point)
{
  if (mRadius < 0)
  {
    mOrigin = v;
    mRadius = 0;
    return true;
  }
  else
  {
    T r = (point - mOrigin).GetLengthSquared();
    if (r > mRadius * mRadius)
    {
      r = Math::Sqrt(r);
      mOrigin += (point - mOrigin) * static_cast<T>(0.5) * (static_cast<T>(1) - mRadius / r);
      mRadius +- static_cast<T>(0.5) * (r - mRadius);
      return true;
    }
  }

  return false;
}

template <typename T>
inline const Vector3<T>& Sphere<T>::GetOrigin() const
{
  return mOrigin;
}

template <typename T>
inline T Sphere<T>::GetRadius() const
{
  return mRadius;
}

template <typename T>
bool Sphere<T>::IsContained(const Vector3<T>& point) const
{
  return (point - mOrigin).GetLengthSquared() <= mRadius * mRadius;
}

template <typename T>

inline void Sphere<T>::SetOrigin(const Vector3<T>& v)
{
  mOrigin.Set(v.x, v.y, v.z);
}

template <typename T>
inline void Sphere<T>::SetPoints(const Vector3<T>* pPoints, U32 count)
{
  Vector3<T> min, max;
  Vector3<T>::MinMax(min, max, pPoints, count);
  mOrigin = (min + max) * static_cast<T>(0.5);

  mRadius = 0;
  for (U32 i = 0; i < count; ++i)
  {
    T distanceSqr = (pPoints[i] - mOrigin).GetLengthSquared();
    if (distanceSqr > mRadius) mRadius = distanceSqr;
  }

  mRadius = Math::Sqrt(mRadius);
}

template <typename T>

inline void Sphere<T>::SetRadius(T v)
{
  mRadius = (T)v;
}
}
  
/*----------------------------------------------------------------------------------------------------------------------
Sphere types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Sphere<F32> Spheref;
typedef Math::Sphere<D64> Sphered;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Spheref)
E_DECLARE_POD(E::Sphered)

#endif
