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

// Created 23-Sep-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file WeakPtr.h
This file defines an non-intrusive smart pointer which implements weak reference counting. This implementation is 
inspired by C++11 Standard Library weak_ptr.
*/

#ifndef E3_WEAK_POINTER_H
#define E3_WEAK_POINTER_H

#include <WeakPtr.h>

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
WeakPtr

1. WeakPtr holds a weak reference to a pointer owned by a SharedPtr
2. IsValid tells whether or not the referenced pointer owned by a SharePtr is valid (has a count greater than zero) or
has been already released by the owner SharedPtr
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = U32>
class WeakPtr
{
public:
  WeakPtr();
  WeakPtr(const WeakPtr& other);
  template <class U>
  WeakPtr(const WeakPtr<U, CounterType>& other);
  template <class U, class DeleterClass>
  WeakPtr(const SharedPtr<U, CounterType, DeleterClass>& other);
  ~WeakPtr();

  // Operators
  WeakPtr&                    operator=(const WeakPtr& other);
  template <class U>
  WeakPtr&                    operator=(const WeakPtr<U, CounterType>& other);
  template <class U, class DeleterClass>
  WeakPtr&                    operator=(const SharedPtr<U, CounterType, DeleterClass>& other);
  T&                          operator*() const;
  T*                          operator->() const;
  bool                        operator==(const WeakPtr& other) const;
  bool                        operator==(const T* ptr) const;
  bool                        operator!=(const WeakPtr& other) const;
  bool                        operator!=(const T* ptr) const;  

  // Accessors
  bool                        IsValid() const;

  // Methods
  void                        Reset();

private:
  // Allow private member access to copy constructor & assignment operator method versions which require static_cast conversion. 
  template <class U, typename CounterType>
  friend class WeakPtr;

  T*                          mpPtr;
  SharedCounter<CounterType>* mpCounter;

  void                        Swap(WeakPtr& other);
};
}

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
WeakPtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline WeakPtr<T, CounterType>::WeakPtr()
  : mpPtr(nullptr)
  , mpCounter(nullptr) {}

template <class T, typename CounterType>
inline WeakPtr<T, CounterType>::WeakPtr(const WeakPtr& other)
  : mpPtr(other.mpPtr)
  , mpCounter(other.mpCounter)
{
  if (mpCounter) ++mpCounter->weakCount;
}

template <class T, typename CounterType>
template <class U>
inline WeakPtr<T, CounterType>::WeakPtr(const WeakPtr<U, CounterType>& other)
  : mpPtr(static_cast<T*>(other.mpPtr))
  , mpCounter(other.mpCounter)
{
  if (mpCounter) ++mpCounter->weakCount;
}

template <class T, typename CounterType>
template <class U, class DeleterClass>
inline WeakPtr<T, CounterType>::WeakPtr(const SharedPtr<U, CounterType, DeleterClass>& other)
  : mpPtr(static_cast<T*>(other.mpPtr))
  , mpCounter(other.mpCounter)
{
  if (mpCounter) ++mpCounter->weakCount;
}

template <class T, typename CounterType>
inline WeakPtr<T, CounterType>::~WeakPtr()
{
  Reset();
}

/*----------------------------------------------------------------------------------------------------------------------
WeakPtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline WeakPtr<T, CounterType>& WeakPtr<T, CounterType>::operator=(const WeakPtr& other)
{
  WeakPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U>
inline WeakPtr<T, CounterType>& WeakPtr<T, CounterType>::operator=(const WeakPtr<U, CounterType>& other)
{
  WeakPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U, class DeleterClass>
inline WeakPtr<T, CounterType>& WeakPtr<T, CounterType>::operator=(const SharedPtr<U, CounterType, DeleterClass>& other)
{
  WeakPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
inline T& WeakPtr<T, CounterType>::operator*() const
{
  E_ASSERT_PTR(mpPtr);
  return *mpPtr;
}

template <class T, typename CounterType>
inline T* WeakPtr<T, CounterType>::operator->() const
{
  E_ASSERT_PTR(mpPtr);
  return mpPtr;
}

template <class T, typename CounterType>
inline bool WeakPtr<T, CounterType>::operator==(const WeakPtr& other) const
{
  return mpPtr == other.mpPtr;
}

template <class T, typename CounterType>
inline bool WeakPtr<T, CounterType>::operator==(const T* ptr) const
{
  return mpPtr == ptr;
}

template <class T, typename CounterType>
inline bool WeakPtr<T, CounterType>::operator!=(const WeakPtr& other) const
{
  return mpPtr != other.mpPtr;
}

template <class T, typename CounterType>
inline bool WeakPtr<T, CounterType>::operator!=(const T* ptr) const
{
  return mpPtr != ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
WeakPtr accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline bool WeakPtr<T, CounterType>::IsValid() const
{
  return (mpCounter && mpCounter->count != 0);
}

/*----------------------------------------------------------------------------------------------------------------------
WeakPtr methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void WeakPtr<T, CounterType>::Reset()
{
  if (mpCounter) 
  {      
    --mpCounter->weakCount;
    if (mpCounter->weakCount == 0)
    { 
      if (mpCounter->count == 0) E_DELETE(mpCounter);
    }
    mpPtr = nullptr;
    mpCounter = nullptr;
  }
}

/*----------------------------------------------------------------------------------------------------------------------
WeakPtr private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void WeakPtr<T, CounterType>::Swap(WeakPtr& other)
{
  T* tmpPtr = mpPtr;
  SharedCounter<CounterType>* tmpCounter = mpCounter;
  mpPtr = other.mpPtr;
  mpCounter = other.mpCounter;
  other.mpPtr = tmpPtr;
  other.mpCounter = tmpCounter;
}
}

#endif
