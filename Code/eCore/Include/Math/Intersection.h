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

// Created 13-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IntersectionUtil.h
This file defines geometric intersection utility functions.
*/

#ifndef E3_INTERSECTION_H
#define E3_INTERSECTION_H

#include "Math.h"
#include "Vector3.h"
#include "Box3.h"
#include "Sphere.h"

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Math (IntersectionUtil)

Please note that Math utility functions have the following usage contract: 

1. Floating point types are expected to be used with all the functions: F32, D64.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
bool IntersectRayBox3(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Box3<T>& box, T& outLambda)

  template <class T>
bool IntersectRayTriangle(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Vector3<T>& x0, const Vector3<T>& x1, const Vector3<T>& x2, T& outLambda, bool faceCulling = false);

template <class T>
bool IntersectRaySphere(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Vector3<T>& sphereCenter, T sphereRadius, T& outLambda);

/*----------------------------------------------------------------------------------------------------------------------
Math functions
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
IntersectRayTriangle

Performs a ray intersection test with a triangle defined by x0, x1 and x2 and retrieves the distance from the ray origin
(defined by outLambda).
http://www.cs.lth.se/home/Tomas_Akenine_Moller/code/
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
bool IntersectRayBox3(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Box3<T>& box, T& outLambda)
{
  bool inside = true;

  // Right 'R'  Left 'L' Middle 'M'
  char quadrant[3]; 
  Vector3<T> candidatePlane;
  Vector3<T> hitPoint;
  const Vector3<T>& boxMax = box.GetMax();
  const Vector3<T>& boxMin = box.GetMin();

  // Find candidate planes; this loop can be avoided if rays cast all from the eye (assume perpsective view)
  for (U32 i = 0; i < 3; ++i)
  {
    if (rayOrigin[i] < boxMin[i])
    {
      quadrant[i] = 'L';
      candidatePlane[i] = boxMin[i];
      inside = false;
    }
    else if (rayOrigin[i] > boxMax[i])
    {
      quadrant[i] = 'R';
      candidatePlane[i] = boxMax[i];
      inside = false;
    }
    else
    {
      quadrant[i] = 'M';
    }
  }

  // Ray origin inside bounding box
  if (inside)
  {
    hitPoint = rayOrigin;
    outLambda = 0;
    return true;
  }

  // Calculate distances to candidate planes
  Vector3<T> maxT;
  for (U32 i = 0; i < 3; i++)
  {
    if (quadrant[i] != 'M' && rayDirection[i] != 0)
      maxT[i] = (candidatePlane[i] - rayOrigin[i]) / rayDirection[i];
    else
      maxT[i] = -1.;
  }

  // Get largest of the maxT's for final choice of intersection
  I32 whichPlane = 0;
  for (U32 i = 1; i < 3; i++)
    if (maxT[whichPlane] < maxT[i])
      whichPlane = i;

  // Check final candidate actually inside box
  if (maxT[whichPlane] < 0)
    return false;

  for (U32 i = 0; i < 3; i++)
  {
    if (whichPlane != i)
    {
      hitPoint[i] = rayOrigin[i] + maxT[whichPlane] * rayDirection[i];
      if (hitPoint[i] < minB[i] || hitPoint[i] > boxMax[i])
        return false;
    }
    else
    {
      hitPoint[i] = candidatePlane[i];
    }
  }

  // Ray hits box
  outLambda = maxT[whichPlane];

  return true;	
}

/*----------------------------------------------------------------------------------------------------------------------
IntersectRaySphere

Performs a ray intersection test with a sphere and retrieves the distance from the ray origin (defined by outLambda).
Note that the intersection test against a sphere is understood to be always in a "outside to inside direction" fashion.
(there is only be one outLambda value). The outLambda value is only written when there is an intersection corresponding
to the collision depth from the origin of the ray to the intersection point. The outLambda value is negative when the ray 
origin is inside of the sphere.
http://www.devmaster.net/wiki/Ray-sphere_intersection
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
bool IntersectRaySphere(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Sphere<T>& sphere, T& outLambda)
{
  Vector3<T> v = rayOrigin - sphere.GetOrigin();
  T b = T(2) * Vector3<T>::Dot(v, rayDirection);
  T c = - sphere.GetRadius() * sphere.GetRadius() + Vector3<T>::Dot(v, v);
  T d = b * b - T(4) * c;

  if (d >= static_cast<T>(0))
  {
    d = Math::Sqrt(d);
    T l1 = (-b + d) / (T(2));
    T l2 = (-b - d) / (T(2));

    if (l1 >= static_cast<T>(0) || l2 >= static_cast<T>(0))
    {
      if (l1 < l2 && l1 >= static_cast<T>(0)) // The observer is inside the sphere.
      {
        outLambda = l1;
        return true;
      }
      else // The observer is outside the sphere.
      {
        outLambda = l2;
        return true;
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------------------------------------------------
IntersectRayTriangle

Performs a ray intersection test with a triangle defined by x0, x1 and x2 and retrieves the distance from the ray origin
(defined by outLambda).
http://www.cs.lth.se/home/Tomas_Akenine_Moller/code/
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
bool IntersectRayTriangle(const Vector3<T>& rayOrigin, const Vector3<T>& rayDirection, const Vector3<T>& x0, const Vector3<T>& x1, const Vector3<T>& x2, T& outLambda, bool faceCulling = false)
{
  // Find vectors for two edges sharing x0
  Vector3<T> edge0 = x1 - x0;
  Vector3<T> edge1 = x2 - x0;

  // Begin calculating determinant - also used to calculate U parameter
  Vector3<T> pvec = Vector3<T>::Cross(rayDirection, edge1);

  // If determinant is near zero, ray lies in plane of triangle
  T det = Vector3<T>::Dot(edge0, pvec);

  const T epsilon = Math::Epsilon<T>::Get();
  if (det < epsilon)
  {
    if (faceCulling)
      return false;
    else if (det > -epsilon)
      return false;
  }

  T inv_det = static_cast<T>(1) / det;

  // Calculate distance from vert0 to ray source
  Vector3<T> tvec = rayOrigin - x0;

  // Calculate U parameter and test bounds
  T u = Vector3<T>::Dot(tvec, pvec) * inv_det;
  if (u < static_cast<T>(0) || u > static_cast<T>(1))
    return false;

  // Prepare to test V parameter
  Vector3<T> qvec = Vector3<T>::Cross(tvec, edge0);

  // calculate V parameter and test bounds
  T v = Vector3<T>::Dot(rayDirection, qvec) * inv_det;
  if (v < static_cast<T>(0) || u + v > static_cast<T>(1))
    return false;

  // calculate t, ray intersects triangle
  outLambda = Vector3<T>::Dot(edge1, qvec) * inv_det;
  return true;
}
}
}

#endif
