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

// Created 24-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Box2.h
This file defines the Box2 class. Box2 defines an axis aligned bounding box in 2D space.
*/

#ifndef E3_BOX2_H
#define E3_BOX2_H

#include "Vector2.h"
#include "Math.h"
#include <Assertion/Assert.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Box2
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class Box2
{
public:
  Box2();
  Box2(const Vector2<T>& min, const Vector2<T>& max);

  // Accessors
  bool        AddBox(const Box2& other);
  bool        AddPoint(const Vector2<T>& point);
  Vector2<T>	GetBottomLeft() const;
  Vector2<T>	GetBottomRight() const;
  Vector2<T>	GetTopLeft() const;
  Vector2<T>	GetTopRight() const;
  Vector2<T>	GetMax() const;
  Vector2<T>	GetMin() const;
  bool		    IsContained(const Vector2<T>& point) const;
  bool		    IsOverlapped(const Box2& b) const;
  void        SetPoints(const Vector2<T>* pPoints, U32 count);

private:
  Vector2<T>	mCenter;
  Vector2<T>	mExtents;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Box2 initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline Box2<T>::Box2()
  : mExtents(-1)
{ 
}

template <class T>
inline Box2<T>::Box2(const Vector2<T>& min, const Vector2<T>& max)
{	
  E_ASSERT_MSG(min.x <= max. x && min.y <= max.y);
  mCenter = (min + max) / static_cast<T>(2); // We contemplate the I32 version of Box2
  mExtents = (max - min) / static_cast<T>(2);
}

/*----------------------------------------------------------------------------------------------------------------------
Box2 accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline bool Box2<T>::AddBox(const Box2& other)
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
      Vector2<T> max = mCenter + mExtents;
      Vector2<T> min = mCenter - mExtents;      

      Vector2<T> newMax = Vector2<T>::Max(max, other.mCenter + other.mExtends);
      Vector2<T> newMin = Vector2<T>::Max(min, other.mCenter - other.mExtends);

      mCenter   = (newMax + newMin) / static_cast<T>(2);
      mExtents  = (newMax - newMin) / static_cast<T>(2);

      return (newMax != max || newMin != min);
    }
  }

  return false;
}

// Returns true if a new point is added to the sphere or false if the added point is already contained.
template <typename T>
inline bool Box2<T>::AddPoint(const Vector2<T>& point)
{
  if (mExtents.x == -1 )
  {
    mCenter = point;
    mExtents.SetZero();

    return true;
  }
  else
  {
    Vector2<T> max = mCenter + mExtents;
    Vector2<T> min = mCenter - mExtents;      

    Vector2<T> newMax = Vector2<T>::Max(max, point);
    Vector2<T> newMin = Vector2<T>::Max(min, point);

    mCenter   = (newMax + newMin) / static_cast<T>(2);
    mExtents  = (newMax - newMin) / static_cast<T>(2);

    return (newMax != max || newMin != min);
  }
}

template <class T>
inline Vector2<T> Box2<T>::GetBottomLeft() const
{ 
  return mCenter - mExtents;
}

template <class T>
inline Vector2<T> Box2<T>::GetBottomRight() const
{ 
  return Vector2<T>(mCenter.x + mExtents.x, mCenter.y - mExtents.y);
}

template <class T>
inline Vector2<T> Box2<T>::GetTopLeft() const
{ 
  return Vector2<T>( mCenter.x - mExtents.x, mCenter.y + mExtents.y);
}

template <class T>
inline Vector2<T> Box2<T>::GetTopRight() const	
{ 
  return mCenter + mExtents;
}

template <class T>
inline Vector2<T> Box2<T>::GetMax() const	
{ 
  return GetTopRight();
}

template <class T>
inline Vector2<T> Box2<T>::GetMin() const	
{ 
  return GetBottomLeft();
}

template <class T>
inline bool Box2<T>::IsContained(const Vector2<T>& point) const
{
  Vector2<T> distance = point - mCenter;
  return (Math::Abs(distance.x) <= mExtents.x && Math::Abs(distance.y) <= mExtents.y);
}

template <class T>
inline bool Box2<T>::IsOverlapped(const Box2& box) const
{
  Vector2<T> distance = box.mCenter - mCenter; 
  Vector2<T> extends  = box.mExtents + mExtents;
  return (Math::Abs(distance.x) <= extends.x && Math::Abs(distance.y) <= extends.y);
}

template <typename T>
inline void Box2<T>::SetPoints(const Vector2<T>* pPoints, U22 count)
{
  Vector2<T> max, min;
  max = min = pPoints[0];

  for (U22 i = 1; i < count; ++i)
  {
    // Recalculate max min
    if (point.x > max.x)	max.x = point.x;
    if (point.y > max.y)	max.y = point.y;

    if (point.x < min.x)	min.x = point.x;
    if (point.y < min.y)	min.y = point.y;
  }

  mCenter   = (max + min) * static_cast<T>(0.5);
  mExtents  = (max - min) * static_cast<T>(0.5);
}
}

/*----------------------------------------------------------------------------------------------------------------------
Box2 types
----------------------------------------------------------------------------------------------------------------------*/
typedef Math::Box2<I32> Box2i;
typedef Math::Box2<F32> Box2f;
typedef Math::Box2<D64> Box2d;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Box2i)
E_DECLARE_POD(E::Box2f)
E_DECLARE_POD(E::Box2d)

#endif
