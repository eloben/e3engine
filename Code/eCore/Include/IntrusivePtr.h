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

// Created 07-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file SmartPtr.h
This file defines an intrusive smart pointer. 
The IntrusivePtr implementation is based on the original by the Boost libraries available at: http://www.boost.org/. 
The IntrusiveReferenceCounter implementation is based on an article by Peter Weinert which can be found at:
http://www.drdobbs.com/cpp/a-base-class-for-intrusively-reference-c/229218807?pgno=1
*/

#ifndef E3_INTRUSIVE_POINTER_H
#define E3_INTRUSIVE_POINTER_H

#include <Memory/Allocator.h>

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
IntrusivePtr

Please note that this class has the following usage contract:

1. In order to work with IntrusivePtr the hosted class must inherit (preferred privately) from the template class
   IntrusiveReferenceCounter.
2. IntrusiveReferenceCounter uses GAllocator to deallocate itself.
3. This class can offer a certain degree of thread-safety when IntrusiveReferenceCounter is used in conjunction with 
   and Atomic counter such as:

    class Foo : public IntrusiveReferenceCounter<Foo, Atomic<U32>> 

In conjunction with Atomic IntrusiveReferenceCounter is thread-safe as long as the multi-threaded work is done with 
IntrusivePtr copies (and thus the reference count never gets down to zero). This can be easily achieved by keeping aside 
an IntrusivePtr copy (or the original) during the multi-threaded task execution and using SharedPtr copies in the thread 
tasks. To better understand the reasoning behind this requirement take the following example (commented numbers indicate 
thread execution order):

Thread 1:                                                           
IntrusivePtr<Foo> fooPtr1;     On destruction:    friend void IntrusiveReferenceCounterRemove(const DerivedType* p)
                                                  {
                                                    if (--((const IntrusiveReferenceCounter*) p)->mCounter == 0)  // 1
                                                      delete p;                                                   // 4
                                                  } 
Thread 2:                                                           
IntrusivePtr<Foo> fooPtr2;     On destruction:    friend void IntrusiveReferenceCounterRemove(const DerivedType* p)
                                                  {
                                                    if (--((const IntrusiveReferenceCounter*) p)->mCounter == 0)   // 2
                                                      delete p;                                                    // 3
                                                  }

In step 3 the resource memory would be freed so step 4 will cause an access violation (setting it to nullptr would not 
solve the problem as the invariance is broken due to the fact that only the counter is thread-safe.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class IntrusivePtr
{
public:
  explicit IntrusivePtr(T* ptr);
  IntrusivePtr(const IntrusivePtr& other);
  ~IntrusivePtr();

  // Operators
  IntrusivePtr& operator=(const IntrusivePtr& other);
  IntrusivePtr& operator=(T* ptr);
  T &           operator*() const;
  T *           operator->() const;
  bool          operator==(const T* ptr) const;    
  bool          operator!=(const T* ptr) const; 

  // Accessors
  T *           GetPtr() const;
  bool          IsUnique() const;

private:
  void          Swap(IntrusivePtr& other);

  T*            mpPtr;
};

/*----------------------------------------------------------------------------------------------------------------------
IntrusiveReferenceCounter
----------------------------------------------------------------------------------------------------------------------*/
template <class DerivedType, typename CounterType = U32, class DeleterClass = Memory::Deleter<DerivedType>>
class IntrusiveReferenceCounter
{
public:
  friend bool IntrusiveReferenceCounterIsUnique(const DerivedType* p)
  {
    return (((const IntrusiveReferenceCounter*) p)->mCounter == 1);
  }

  friend void IntrusiveReferenceCounterAdd(const DerivedType* p)
  {
    ++((const IntrusiveReferenceCounter*) p)->mCounter;
  }

  friend void IntrusiveReferenceCounterRemove(const DerivedType* p)
  {
    if (--((const IntrusiveReferenceCounter*) p)->mCounter == 0)
    {
      DeleterClass::Delete(const_cast<DerivedType*>(p));
    }
  }
protected:
  IntrusiveReferenceCounter();
  IntrusiveReferenceCounter(const IntrusiveReferenceCounter&);
  IntrusiveReferenceCounter& operator=(const IntrusiveReferenceCounter&);
  ~IntrusiveReferenceCounter();

private:
  mutable CounterType mCounter;
};

/*----------------------------------------------------------------------------------------------------------------------
IntrusivePtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
inline IntrusivePtr<T>::IntrusivePtr(T* ptr)
{
  mpPtr = ptr;
  if (mpPtr)
  {
    IntrusiveReferenceCounterAdd(mpPtr);
  }
}

template <class T>
inline IntrusivePtr<T>::IntrusivePtr(const IntrusivePtr& other)
{
  mpPtr = other.mpPtr;
  if (mpPtr)
  {
    IntrusiveReferenceCounterAdd(mpPtr);
  }
}

template <class T>
inline IntrusivePtr<T>::~IntrusivePtr()
{
  if (mpPtr)
  {
    IntrusiveReferenceCounterRemove(mpPtr);
  }
}

/*----------------------------------------------------------------------------------------------------------------------
IntrusivePtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline IntrusivePtr<T>&  IntrusivePtr<T>::operator=(const IntrusivePtr& other)
{
  IntrusivePtr<T>(other).Swap(this);
  return *this;
}

template <class T>
inline IntrusivePtr<T>& IntrusivePtr<T>::operator=(T* ptr)
{
  IntrusivePtr<T>(ptr).Swap(*this);
  return *this;
}

template <class T>
inline T& IntrusivePtr<T>::operator*() const
{
  E_ASSERT_PTR(mpPtr);
  return *mpPtr;
}

template <class T>
inline T* IntrusivePtr<T>::operator->() const
{
  E_ASSERT_PTR(mpPtr);
  return mpPtr;
}

template <class T>
inline bool IntrusivePtr<T>::operator==(const T* ptr) const
{
  return mpPtr == ptr;
}

template <class T>
inline bool IntrusivePtr<T>::operator!=(const T* ptr) const
{
  return mpPtr != ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
IntrusivePtr accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline T* IntrusivePtr<T>::GetPtr() const
{
  return mpPtr;
}

template <class T>
inline bool IntrusivePtr<T>::IsUnique() const
{
  return IntrusiveReferenceCounterIsUnique(mpPtr);
}

/*----------------------------------------------------------------------------------------------------------------------
IntrusivePtr private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void IntrusivePtr<T>::Swap(IntrusivePtr& other)
{
  T* tmp = mpPtr;
  mpPtr = other.mpPtr;
  other.mpPtr = tmp;
}

/*----------------------------------------------------------------------------------------------------------------------
IntrusiveReferenceCounter initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class DerivedType, typename CounterType, class DeleterClass>
inline IntrusiveReferenceCounter<DerivedType, CounterType, DeleterClass>::IntrusiveReferenceCounter()
  : mCounter(0)
{
}

template <class DerivedType, typename CounterType, class DeleterClass>
inline IntrusiveReferenceCounter<DerivedType, CounterType, DeleterClass>::IntrusiveReferenceCounter(const IntrusiveReferenceCounter<DerivedType, CounterType, DeleterClass>&)
  : mCounter(0)
{
}

template <class DerivedType, typename CounterType, class DeleterClass>
inline IntrusiveReferenceCounter<DerivedType, CounterType, DeleterClass>::~IntrusiveReferenceCounter()
{
}

/*----------------------------------------------------------------------------------------------------------------------
IntrusiveReferenceCounter operators
----------------------------------------------------------------------------------------------------------------------*/

template <class DerivedType, typename CounterType, class DeleterClass>
inline IntrusiveReferenceCounter<DerivedType, CounterType, DeleterClass>& IntrusiveReferenceCounter<DerivedType, CounterType, DeleterClass>::operator=(const IntrusiveReferenceCounter<DerivedType, CounterType, DeleterClass>&)
{ 
  return *this;
}
}

#endif
