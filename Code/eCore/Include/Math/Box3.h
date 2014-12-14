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

// Created 34-Jan-3014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Box3.h
This file defines the Box3 class. Box3 defines an axis aligned bounding box in 3D space.
*/

#ifndef E3_BOX3_H
#define E3_BOX3_H

#include "Vector3.h"
#include "Matrix4.h"
#include "Math.h"
#include <Assertion/Assert.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Box3

Please note that this class has the following usage contract:

1. Floating point types are expected to be used with this class: F32, D64.
2. SetPoints count must be greater than 0.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class Box3
{
public:
  Box3();
  Box3(const Vector3<T>& min, const Vector3<T>& max);

  // Accessors
  bool        AddBox(const Box3& other);
  bool        AddPoint(const Vector3<T>& point);
  Vector3<T>	GetBackBottomLeft() const;
  Vector3<T>	GetBackBottomRight() const;
  Vector3<T>	GetBackTopLeft() const;
  Vector3<T>	GetBackTopRight() const;
  Vector3<T>	GetFrontBottomLeft() const;
  Vector3<T>	GetFrontBottomRight() const;
  Vector3<T>	GetFrontTopLeft() const;
  Vector3<T>	GetFrontTopRight() const;
  Vector3<T>	GetMax() const;
  Vector3<T>	GetMin() const;
  bool		    IsContained(const Vector3<T>& point) const;
  bool		    IsOverlapped(const Box3& box) const;
  void        SetPoints(const Vector3<T>* pPoints, U32 count);

  // Methods
  void        Transform(const Matrix4<T>& matrix);

private:
  Vector3<T>	mCenter;
  Vector3<T>	mExtents;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Box3 initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline Box3<T>::Box3()
  : mExtents(-1)
{ 
}

template <class T>
inline Box3<T>::Box3(const Vector3<T>& min, const Vector3<T>& max)
{	
  E_ASSERT_MSG(min.x <= max. x && min.y <= max.y && min.z <= max.z);
  mCenter   = (max + min) * static_cast<T>(0.5);
  mExtents  = (max - min) * static_cast<T>(0.5);
}

/*----------------------------------------------------------------------------------------------------------------------
Box3 accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline bool Box3<T>::AddBox(const Box3& other)
{
  // If other is empty
  if (other.mExtends.x != -1)
  {
    // If this is empty
    if (mExtends.x == -1)
    {
      *this = other;
      return true;
    }
    else
    {
      Vector3<T> max = mCenter + mExtents;
      Vector3<T> min = mCenter - mExtents;      

      Vector3<T> newMax = Vector3<T>::Max(max, other.mCenter + other.mExtends);
      Vector3<T> newMin = Vector3<T>::Max(min, other.mCenter - other.mExtends);

      mCenter   = (newMax + newMin) * static_cast<T>(0.5);
      mExtents  = (newMax - newMin) * static_cast<T>(0.5);

      return (newMax != max || newMin != min);
    }
  }

  return false;
}

// Returns true if a new point is added to the sphere or false if the added point is already contained.
template <typename T>
inline bool Box3<T>::AddPoint(const Vector3<T>& point)
{
  if (mExtents.x == -1 )
  {
    mCenter = point;
    mExtents.SetZero();

    return true;
  }
  else
  {
    Vector3<T> max = mCenter + mExtents;
    Vector3<T> min = mCenter - mExtents;      

    Vector3<T> newMax = Vector3<T>::Max(max, point);
    Vector3<T> newMin = Vector3<T>::Max(min, point);

    mCenter   = (newMax + newMin) * static_cast<T>(0.5);
    mExtents  = (newMax - newMin) * static_cast<T>(0.5);

    return (newMax != max || newMin != min);
  }
}

template <class T>
inline Vector3<T> Box3<T>::GetBackBottomLeft() const
{ 
  return Vector3<T>(mCenter.x - mExtents.x, mCenter.y - mExtents.y, mCenter.z + mExtents.z);
}

template <class T>
inline Vector3<T> Box3<T>::GetBackBottomRight() const
{ 
  return Vector3<T>(mCenter.x + mExtents.x, mCenter.y - mExtents.y, mCenter.z + mExtents.z);
}

template <class T>
inline Vector3<T> Box3<T>::GetBackTopLeft() const
{ 
  return Vector3<T>(mCenter.x - mExtents.x, mCenter.y + mExtents.y, mCenter.z + mExtents.z);
}

template <class T>
inline Vector3<T> Box3<T>::GetBackTopRight() const	
{ 
  return mCenter + mExtents;
}

template <class T>
inline Vector3<T> Box3<T>::GetFrontBottomLeft() const
{ 
  return mCenter - mExtents;
}

template <class T>
inline Vector3<T> Box3<T>::GetFrontBottomRight() const
{ 
  return Vector3<T>(mCenter.x + mExtents.x, mCenter.y - mExtents.y, mCenter.z - mExtents.z);
}

template <class T>
inline Vector3<T> Box3<T>::GetFrontTopLeft() const
{ 
  return Vector3<T>(mCenter.x - mExtents.x, mCenter.y + mExtents.y, mCenter.z - mExtents.z);
}

template <class T>
inline Vector3<T> Box3<T>::GetFrontTopRight() const	
{ 
  return Vector3<T>(mCenter.x + mExtents.x, mCenter.y + mExtents.y, mCenter.z - mExtents.z);
}

template <class T>
inline Vector3<T> Box3<T>::GetMax() const	
{ 
  return GetBackTopRight();
}

template <class T>
inline Vector3<T> Box3<T>::GetMin() const	
{ 
  return GetFrontBottomLeft();
}

template <class T>
inline bool Box3<T>::IsContained(const Vector3<T>& point) const
{
  Vector3<T> distance = point - mCenter;
  return (Math::Abs(distance.x) <= mExtents.x && Math::Abs(distance.y) <= mExtents.y && Math::Abs(distance.z) <= mExtents.z);
}

template <class T>
inline bool Box3<T>::IsOverlapped(const Box3& box) const
{
  Vector3<T> distance = box.mCenter - mCenter; 
  Vector3<T> extends  = box.mExtents + mExtents;
  return (Math::Abs(distance.x) <= extends.x && Math::Abs(distance.y) <= extends.y && Math::Abs(distance.z) <= extends.z);
}

template <typename T>
inline void Box3<T>::SetPoints(const Vector3<T>* pPoints, U32 count)
{
  Vector3<T> max, min;
  max = min = pPoints[0];

  for (U32 i = 1; i < count; ++i)
  {
    // Recalculate max min
    if (point.x > max.x)	max.x = point.x;
    if (point.y > max.y)	max.y = point.y;
    if (point.z > max.z)	max.z = point.z;

    if (point.x < min.x)	min.x = point.x;
    if (point.y < min.y)	min.y = point.y;
    if (point.z < min.z)	min.z = point.z;
  }

  mCenter   = (max + min) * static_cast<T>(0.5);
  mExtents  = (max - min) * static_cast<T>(0.5);
}

/*----------------------------------------------------------------------------------------------------------------------
Box3 methods
----------------------------------------------------------------------------------------------------------------------*/

void Transform(const Matrix4<T>& matrix)
{
  Box3<T> result;
  result.PushPoint(matrix.Transform(GetBackBottomLeft()));
  result.PushPoint(matrix.Transform(GetBackBottomRigth()));
  result.PushPoint(matrix.Transform(GetBackTopLeft()));
  result.PushPoint(matrix.Transform(GetBackTopRightLeft()));
  result.PushPoint(matrix.Transform(GetFrontBottomLeft()));
  result.PushPoint(matrix.Transform(GetFrontBottomRigth()));
  result.PushPoint(matrix.Transform(GetFrontTopLeft()));
  result.PushPoint(matrix.Transform(GetFrontTopRightLeft()));
  (*this) = result;
}
}

/*----------------------------------------------------------------------------------------------------------------------
Box3 types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Box3<F32> Box3f;
typedef Math::Box3<D64> Box3d;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Box3f)
E_DECLARE_POD(E::Box3d)

#endif


	
