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

/** @file Stack.h
This file defines the Stack class. Stack is implemented on top of List (sequential container)
*/

#ifndef E3_STACK_H
#define E3_STACK_H

#include "List.h"

namespace E
{
namespace Containers
{
/*----------------------------------------------------------------------------------------------------------------------
Stack

Please note that this class has the following usage contract: 

1. GetTop will E_ASSERT_MSG on empty stack.
2. On resize array elements are always default initialized (inherited from list).
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class Stack
{
public:
  Stack();
  explicit Stack(size_t size);
  ~Stack();

  // Accessors
  const Memory::IAllocator* GetAllocator() const;
  size_t                    GetCount() const;
  size_t                    GetGranularity() const;
  U8                        GetGrowthPercentage() const;
  size_t                    GetSize() const;
  const T&                  GetTop() const;
  T&                        GetTop();
  bool                      HasValue(const T& value) const;
  bool                      IsEmpty() const;
  void                      SetAllocator(Memory::IAllocator* p);

  // Methods                
  void                      Clear();
  void                      Compact();
  void                      EnsureSize(size_t size);
  void                      Pop(size_t count = 1);
  void                      Push(const T& value);
  void                      Push(const T* pData, size_t count);
  void                      Reserve(size_t size);
  void                      Resize(size_t size);
  void                      Trim(size_t size);

private:
  List<T>                   mList;

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Stack initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
inline Stack<T>::Stack()
{
}

template <typename T>
inline Stack<T>::Stack(size_t size)
  : mList(size)
{
}

template <typename T>
inline Stack<T>::~Stack()
{
}

/*----------------------------------------------------------------------------------------------------------------------
Stack accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline const Memory::IAllocator* Stack<T>::GetAllocator() const
{
  return mList.GetAllocator();
}

template <typename T>
inline size_t Stack<T>::GetCount() const
{
  return mList.GetCount();
}

template <typename T>
inline size_t Stack<T>::GetGranularity() const
{
  return mList.GetGranularity();
}

template <typename T>
inline U8 Stack<T>::GetGrowthPercentage() const
{
  return mList.GetGrowthPercentage();
}

template <typename T>
inline size_t Stack<T>::GetSize() const
{
  return mList.GetSize();
}

template <typename T>
inline const T& Stack<T>::GetTop() const
{
  E_ASSERT_MSG(mList.GetCount(), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return mList[mList.GetCount() - 1];
}

template <typename T>
inline T& Stack<T>::GetTop()
{
  E_ASSERT_MSG(mList.GetCount(), E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  return mList[mList.GetCount() - 1];
}

template <typename T>
inline bool Stack<T>::HasValue(const T& value) const
{
  return mList.HasValue(value);
}

template <typename T>
inline bool Stack<T>::IsEmpty() const
{
  return mList.IsEmpty();
}

template <typename T>
inline void Stack<T>::SetAllocator(Memory::IAllocator* p)
{
  mList.SetAllocator(p);
}

/*----------------------------------------------------------------------------------------------------------------------
Stack methods
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
inline void Stack<T>::Clear()
{
  mList.Clear();
}

template <typename T>
inline void Stack<T>::Compact()
{
  mList.Compact();
}

template <typename T>
inline void Stack<T>::EnsureSize(size_t size)
{
  mList.EnsureSize(size);
}

template <typename T>
inline void Stack<T>::Pop(size_t count)
{
  mList.PopBack(count);
}

template <typename T>
inline void Stack<T>::Push(const T& value)
{
  mList.PushBack(value);
}

template <typename T>
inline void Stack<T>::Push(const T* pData, size_t count)
{
  mList.PushBack(pData, count);
}

template <typename T>
inline void Stack<T>::Reserve(size_t size)
{
  mList.Reserve(size);
}

template <typename T>
inline void Stack<T>::Resize(size_t size)
{
  mList.Resize(size);
}

template <typename T>
inline void Stack<T>::Trim(size_t size)
{
  mList.Trim(size);
}
}
}

#endif
