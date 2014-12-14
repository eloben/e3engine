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

// Created 12-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Distance.h
This file defines distance related utility functions.
*/

#ifndef E3_DISTANCE_H
#define E3_DISTANCE_H

#include "Math.h"

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Math (Distance)

Please note that Math utility functions have the following usage contract: 

1. Floating point types are expected to be used with all the functions: F32, D64.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
Vector3<T> ClosestPointToSegment(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1);

template <typename T>
T DistancePointLine(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1);

template <typename T>
T DistancePointSegment(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1);

template <class T>
T DistancePointTriangle(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1, const Vector3<T>& x2, Vector3<T>& closestPoint);

template <class T>
T DistancePointTriangleSquared(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1, const Vector3<T>& x2, Vector3<T>& closestPoint);

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
ClosestPointToSegment

Given a point p calculate the closest point on the segment between x0 and x1.
http://www.allegro.cc/forums/thread/589720
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
inline Vector3<T> ClosestPointToSegment(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1)
{
  const Vector3<T> v = x1 - x0;
  const Vector3<T> w = p - x0;

  T c1 = Vector3<T>::Dot(w, v);
  if (c1 <= static_cast<T>(0))
    return x0;

  T c2 = v.GetLengthSquared();
  if (c1 >= c2)
    return x1;

  const T b = c1 / c2;
  Vector3<T> Pb = x0 + b * v;
  return Pb;
}

/*----------------------------------------------------------------------------------------------------------------------
ClosestPointToSegment

Given a point p calculate the minimum distance to the line defined by x0 and x1.
http://www.allegro.cc/forums/thread/589720
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
inline T DistancePointLine(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1)
{
  Vector3<T> x1 = p - x0;
  Vector3<T> x2 = x1 - x0;
  return std::sqrt(Vector3<T>::Cross(x1,x2).GetLengthSquared() / x2.GetLengthSquared());
}

/*----------------------------------------------------------------------------------------------------------------------
ClosestPointToSegment

Given a point p calculate the minimum distance to the segment between x0 and x1.
http://www.allegro.cc/forums/thread/589720
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
inline T DistancePointSegment(const Vector3<T>& p, const Vector3<T>& x0, const Vector3<T>& x1)
{
  Vector3<T> closestPoint = ClosestPointToSegment(p, x0, x1);
  return (p - closestPoint).Module();
}

/*----------------------------------------------------------------------------------------------------------------------
DistancePointTriangle

Given a point p calculate the minimum distance to a triangle defined by x0, x1 and x2.
http://www.geometrictools.com/LibFoundation/Distance/Distance.html
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
inline T DistancePointTriangle(const Vector3<T>& point, const Vector3<T>& x0, const Vector3<T>& x1, const Vector3<T>& x2, Vector3<T>& closestPoint)
{
  return Math::Sqrt(DistancePointTriangleSquared(point, x0, x1, x2, closestPoint));
}

/*----------------------------------------------------------------------------------------------------------------------
DistancePointTriangle

Given a point p calculate the minimum distance squared to a triangle defined by x0, x1 and x2.
http://www.geometrictools.com/LibFoundation/Distance/Distance.html
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
T DistancePointTriangleSquared(const Vector3<T>& point, const Vector3<T>& x0, const Vector3<T>& x1, const Vector3<T>& x2, Vector3<T>& closestPoint)
{
  Vector3<T> diff = x0 - point;
  Vector3<T> edge0 = x1 - x0;
  Vector3<T> edge1 = x2 - x0;
  T a00 = edge0.GetLengthSquared();
  T a01 = Vector3<T>::Dot(edge0, edge1);
  T a11 = edge1.GetLengthSquared();
  T b0 = Vector3<T>::Dot(diff, edge0);
  T b1 = Vector3<T>::Dot(diff, edge1);
  T c = diff.GetLengthSquared();
  T det = std::abs(a00*a11 - a01*a01);
  T s = a01*b1 - a11*b0;
  T t = a01*b0 - a00*b1;
  T sqrDistance;

  if (s + t <= det)
  {
    if (s < static_cast<T>(0))
    {
      if (t < static_cast<T>(0))  // region 4
      {
        if (b0 < static_cast<T>(0))
        {
          t = static_cast<T>(0);
          if (-b0 >= a00)
          {
            s = static_cast<T>(1);
            sqrDistance = a00 + static_cast<T>(2) * b0 + c;
          }
          else
          {
            s = -b0 / a00;
            sqrDistance = b0 * s + c;
          }
        }
        else
        {
          s = static_cast<T>(0);
          if (b1 >= static_cast<T>(0))
          {
            t = static_cast<T>(0);
            sqrDistance = c;
          }
          else if (-b1 >= a11)
          {
            t = static_cast<T>(1);
            sqrDistance = a11 + static_cast<T>(2) * b1 + c;
          }
          else
          {
            t = -b1 / a11;
            sqrDistance = b1 * t + c;
          }
        }
      }
      else  // region 3
      {
        s = static_cast<T>(0);
        if (b1 >= static_cast<T>(0))
        {
          t = static_cast<T>(0);
          sqrDistance = c;
        }
        else if (-b1 >= a11)
        {
          t = static_cast<T>(1);
          sqrDistance = a11 + static_cast<T>(2) * b1 + c;
        }
        else
        {
          t = -b1 / a11;
          sqrDistance = b1 * t + c;
        }
      }
    }
    else if (t < static_cast<T>(0))  // region 5
    {
      t = static_cast<T>(0);
      if (b0 >= static_cast<T>(0))
      {
        s = static_cast<T>(0);
        sqrDistance = c;
      }
      else if (-b0 >= a00)
      {
        s = static_cast<T>(1);
        sqrDistance = a00 + static_cast<T>(2) * b0 + c;
      }
      else
      {
        s = -b0 / a00;
        sqrDistance = b0*s+c;
      }
    }
    else  // region 0
    {
      // minimum at interior point
      T invDet = (static_cast<T>(1))/det;
      s *= invDet;
      t *= invDet;
      sqrDistance = s * (a00 * s + a01 * t + static_cast<T>(2) * b0) +
        t * (a01 * s + a11 * t + static_cast<T>(2) * b1) + c;
    }
  }
  else
  {
    T tmp0, tmp1, numer, denom;

    if (s < static_cast<T>(0))  // region 2
    {
      tmp0 = a01 + b0;
      tmp1 = a11 + b1;
      if (tmp1 > tmp0)
      {
        numer = tmp1 - tmp0;
        denom = a00-2.0f*a01+a11;
        if (numer >= denom)
        {
          s = static_cast<T>(1);
          t = static_cast<T>(0);
          sqrDistance = a00 + static_cast<T>(2) * b0 + c;
        }
        else
        {
          s = numer / denom;
          t = static_cast<T>(1) - s;
          sqrDistance = s * (a00 * s + a01 * t + static_cast<T>(2) * b0) +
            t * (a01 * s + a11 * t + static_cast<T>(2) * b1) + c;
        }
      }
      else
      {
        s = static_cast<T>(0);
        if (tmp1 <= static_cast<T>(0))
        {
          t = static_cast<T>(1);
          sqrDistance = a11 + (static_cast<T>(2)) * b1 + c;
        }
        else if (b1 >= static_cast<T>(0))
        {
          t = static_cast<T>(0);
          sqrDistance = c;
        }
        else
        {
          t = -b1 / a11;
          sqrDistance = b1 * t + c;
        }
      }
    }
    else if (t < static_cast<T>(0))  // region 6
    {
      tmp0 = a01 + b1;
      tmp1 = a00 + b0;
      if (tmp1 > tmp0)
      {
        numer = tmp1 - tmp0;
        denom = a00 - static_cast<T>(2) * a01 + a11;
        if (numer >= denom)
        {
          t = static_cast<T>(1);
          s = static_cast<T>(0);
          sqrDistance = a11 + static_cast<T>(2) * b1 + c;
        }
        else
        {
          t = numer / denom;
          s = static_cast<T>(1) - t;
          sqrDistance = s * (a00 * s + a01 * t + static_cast<T>(2) * b0) +
            t * (a01 * s + a11 * t + static_cast<T>(2) * b1) + c;
        }
      }
      else
      {
        t = static_cast<T>(0);
        if (tmp1 <= static_cast<T>(0))
        {
          s = static_cast<T>(1);
          sqrDistance = a00 + static_cast<T>(2) * b0 + c;
        }
        else if (b0 >= static_cast<T>(0))
        {
          s = static_cast<T>(0);
          sqrDistance = c;
        }
        else
        {
          s = -b0 / a00;
          sqrDistance = b0 * s + c;
        }
      }
    }
    else  // region 1
    {
      numer = a11 + b1 - a01 - b0;
      if (numer <= static_cast<T>(0))
      {
        s = static_cast<T>(0);
        t = static_cast<T>(1);
        sqrDistance = a11 + static_cast<T>(2) * b1 + c;
      }
      else
      {
        denom = a00-2.0f*a01+a11;
        if (numer >= denom)
        {
          s = static_cast<T>(1);
          t = static_cast<T>(0);
          sqrDistance = a00 + static_cast<T>(2) * b0 + c;
        }
        else
        {
          s = numer/denom;
          t = static_cast<T>(1) - s;
          sqrDistance = s * (a00 * s + a01 * t + static_cast<T>(2) * b0) +
            t * (a01 * s + a11 * t + static_cast<T>(2) * b1) + c;
        }
      }
    }
  }

  // account for numerical round-off error
  if (sqrDistance < static_cast<T>(0))
  {
    sqrDistance = static_cast<T>(0);
  }

  closestPoint = x0 + s * edge0 + t * edge1;

  return sqrDistance;
}
}
}

#endif
