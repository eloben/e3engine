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

// Created 04-Oct-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Queue.h
This file defines the Queue class. Queue is implemented on top of DynamicArray (base dynamic sequential container)
*/

#ifndef E3_QUEUE_H
#define E3_QUEUE_H

#include "DynamicArray.h"
#include <Math/Comparison.h>

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (E_INTERNAL_SETTING_QUEUE_GROWTH_PERCENTAGE)

Please note that this macro has the following usage contract:

1. This macro defines a default growth percentage for List.
2. This is a global library setting macro which can be predefined by the user.
3. This macro value MUST be an natural greater than zero.
4. This macro is internal and CANNOT be defined outside the library as it is used as a template parameter for the 
Queue class, which is used by other library classes as an argument in some of their methods. You may redefine it at the 
beginning of Base.h instead.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef E_INTERNAL_SETTING_QUEUE_GROWTH_PERCENTAGE
#define E_INTERNAL_SETTING_QUEUE_GROWTH_PERCENTAGE 50
#endif

namespace E
{
namespace Containers
{
/*----------------------------------------------------------------------------------------------------------------------
Queue

This queue implements a "circular buffer" using a DynamicArray container. On growth the queue rearranges its head / tail indices.
Please note that this class has the following usage contract: 

1. GetFront will E_ASSERT_MSG on an empty queue.
2. On resize array elements are always default initialized.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, U8 GrowthPercentage = E_INTERNAL_SETTING_QUEUE_GROWTH_PERCENTAGE>
class Queue
{
public:
  Queue();
  explicit Queue(size_t size);
  ~Queue();

  // Accessors
  const Memory::IAllocator* GetAllocator() const;
  size_t            GetCount() const;
  const T&          GetFront() const;
  T&                GetFront();
  F32               GetGrowthPercentage() const;
  size_t            GetSize() const;
  bool              HasValue(const T& value) const;
  bool              IsEmpty() const;
  void              SetAllocator(Memory::IAllocator* p);

  // Methods        
  void              Clear();
  void              Compact();
  void              EnsureSize(size_t size);
  void              Pop(size_t count = 1);
  void              Push(const T& value);
  void              Push(const T* pData, size_t count);
  void              Reserve(size_t size);
  void              Resize(size_t size);
  void              Trim(size_t size);

private:
  DynamicArray<T>   mData;
  size_t            mCount;
  size_t            mHead;
  size_t            mTail;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Queue initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, U8 GrowthPercentage>
inline Queue<T, GrowthPercentage>::Queue()
  : mCount(0)
  , mHead(0)
  , mTail(0)
{
}

template <typename T, U8 GrowthPercentage>
inline Queue<T, GrowthPercentage>::Queue(size_t size)
  : mData(size)
  , mCount(0)
  , mHead(0)
  , mTail(0)
{
}

template <typename T, U8 GrowthPercentage>
inline Queue<T, GrowthPercentage>::~Queue()
{
}

/*----------------------------------------------------------------------------------------------------------------------
Queue accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, U8 GrowthPercentage>
inline const Memory::IAllocator* Queue<T, GrowthPercentage>::GetAllocator() const
{
  return mData.GetAllocator();
}

template <typename T, U8 GrowthPercentage>
inline size_t Queue<T, GrowthPercentage>::GetCount() const
{
  return mCount;
}

template <typename T, U8 GrowthPercentage>
inline const T& Queue<T, GrowthPercentage>::GetFront() const
{
  E_ASSERT_MSG(mCount, E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return mData[mHead];
}

template <typename T, U8 GrowthPercentage>
inline T& Queue<T, GrowthPercentage>::GetFront()
{
  E_ASSERT_MSG(mCount, E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return mData[mHead];
}

template <typename T, U8 GrowthPercentage>
inline F32 Queue<T, GrowthPercentage>::GetGrowthPercentage() const
{
  return GrowthPercentage;
}

template <typename T, U8 GrowthPercentage>
inline size_t Queue<T, GrowthPercentage>::GetSize() const
{
  return mData.GetSize();
}

template <typename T, U8 GrowthPercentage>
inline bool Queue<T, GrowthPercentage>::HasValue(const T& value) const
{
  for (size_t i = mHead; i < mCount; ++i) if (mData[(mHead + i) % mData.GetSize()] == value) return true;
  return false;
}

template <typename T, U8 GrowthPercentage>
inline bool Queue<T, GrowthPercentage>::IsEmpty() const
{
  return mCount == 0;
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::SetAllocator(Memory::IAllocator* p)
{
  mData.SetAllocator(p);
}

/*----------------------------------------------------------------------------------------------------------------------
Queue methods
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Clear()
{
  for (size_t i = mHead; i < mCount; ++i)  Memory::Destruct(&mData[(mHead + i) % mData.GetSize()]);
  mHead = 0;
  mTail = 0;
  mCount = 0;
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Compact()
{
  Resize(mCount);
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::EnsureSize(size_t size)
{
  if (mData.GetSize() < size)
  {
    Resize(size);
  }
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Pop(size_t count)
{
  if (mCount > count)
  {
    for (U32 i = 0; i < count; ++i)
    {
      Memory::Destruct(&mData[mHead]);
      mHead ++;
      if (mHead == mData.GetSize()) mHead = 0;
    }
    mCount -= count;
  }
  else
  {
    Clear();
  }
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Push(const T& value)
{
  if (mCount == mData.GetSize())
  {
    size_t growSize = static_cast<size_t>(mData.GetSize() * (GrowthPercentage + 100) / 100);
    Resize(growSize == mData.GetSize() ? growSize + 1 : growSize);
  }
  mData[mTail++] = value;
  if (mTail == mData.GetSize()) mTail = 0;
  mCount++;
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Push(const T* pData, size_t count)
{
  for (U32 i = 0; i < count; ++i) Push(pData[i]);
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Reserve(size_t size)
{
  Clear();
  mData = DynamicArray<T>(size);
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Resize(size_t size)
{
  if (size == 0)
  {
    Clear();
    mData = DynamicArray<T>();
  }
  else if (size != mData.GetSize())
  {
    mTail = 0;
    DynamicArray<T> temp(size);
    size_t copySize = Math::Min(size, mCount);
    while (mTail != copySize)
    {
      temp[mTail++] = mData[mHead++];
      if (mHead == mData.GetSize()) mHead = 0;
    }
    mHead = 0;
    if (mTail == size) mTail = 0;
    mData.Swap(temp);
  }
}

template <typename T, U8 GrowthPercentage>
inline void Queue<T, GrowthPercentage>::Trim(size_t size)
{
  mCount = size;
  mTail = mHead + mCount;
  if (mTail > mData.GetSize()) mTail -= mData.GetSize();
}
}
}

#endif
