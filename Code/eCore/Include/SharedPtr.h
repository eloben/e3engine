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

/** @file SharedPtr.h
This file defines an non-intrusive smart pointer implementing reference counting. This implementation is based on the
original by the Boost libraries available at: http://www.boost.org/
*/

#ifndef E3_SHARED_POINTER_H
#define E3_SHARED_POINTER_H

#include <Base.h>
#include <Memory/Memory.h>

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_SHARED_PTR_UNIQUE  "Shared pointer must be unique (it must hold the only reference to the contained pointer)"

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
SharedCount
----------------------------------------------------------------------------------------------------------------------*/
template <typename CounterType>
struct SharedCounter
{
  CounterType count;
  CounterType weakCount;

  SharedCounter() 
  : count(1)
  , weakCount(0) {}
};

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr

Please note that this class has the following usage contract: 

1. SharedPtr uses the Memory::GlobalAllocator for internal reference counting.
2. SharedPtr uses the Memory::GlobalAllocator for contained pointer deallocation and the current implementation does
NOT allow specifying a different allocator class.
3. GetPtr methods are not included for safety reasons, however if the contained pointer must be retrieved the operator 
combination &* can be used.
4. RemoveOwnership prevents SharedPtr from releasing the contained pointer, however for safety reasons the shared 
pointer must be unique in order for the method to work.
5. IsUnique returns true whenever the contained pointer is not null and there is only one reference.
6. Reset cleans up the pointer (reducing count if valid) and sets it to an empty state (like default constructed).
7. This class can offer a certain degree of thread-safety when used in conjunction with and Atomic counter such as:

    SharedPtr<Foo, Atomic<U32>> fooPtr;

In conjunction with Atomic SharedPtr is thread-safe as long as the multi-threaded work is done with SharedPtr copies 
(and thus the reference count never gets down to zero). This can be easily achieved by keeping aside a SharedPtr copy 
(or the original) during the multi-threaded task execution and using SharedPtr copies in the thread tasks. To better
understand the reasoning behind this requirement take the following example (commented numbers indicate thread 
execution order):

Thread 1:                                                           
SharedPtr<Foo, Atomic<U32>> fooPtr1;     On destruction:    if (mpPtr)             // 1
                                                            {      
                                                              --(*mpCount);       // 2
                                                              if (*mpCount == 0)  // 7
                                                                delete mpPtr;      // 8  (Thread 2 already 
                                                            }                            deleted the resource!)

Thread 2:
SharedPtr<Foo, Atomic<U32>> fooPtr2;     On destruction:    if (mpPtr)             // 3
                                                            {      
                                                              --(*mpCount);       // 4
                                                              if (*mpCount == 0)  // 5
                                                                delete mpPtr;      // 6
                                                            }

8. SharedPtr allows custom counter types and deleter classes.
9. SharedPtr resolves assignment between static_cast convertible types in a transparent manner. However assignment 
from other type raw pointers is not allowed.
10. Copy construction or assignment from shared pointers with different counter types is not allowed.

Note: you can use the comparison operator against nullptr to check the SharedPtr validity.

Note: the deleter class does not have to be a template: 

"class T, typename CounterType, class DeleterClass" vs "class T, typename CounterType, template<typename> class DeleterClass"
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = U32, class DeleterClass = Memory::Deleter<T>>
class SharedPtr
{
public:
  SharedPtr();
  SharedPtr(const SharedPtr& other);
  template <class U, class DeleterClassU>
  SharedPtr(const SharedPtr<U, CounterType, DeleterClassU>& other);
  template <class U>
  SharedPtr(U* ptr);
  ~SharedPtr();

  // Operators
  SharedPtr&                  operator=(const SharedPtr& other);
  template <class U, class DeleterClassU>
  SharedPtr&                  operator=(const SharedPtr<U, CounterType, DeleterClassU>& other);
  SharedPtr&                  operator=(T* ptr);
  T&                          operator*() const;
  T*                          operator->() const;
  bool                        operator==(const SharedPtr& other) const;
  bool                        operator==(const T* ptr) const;
  bool                        operator!=(const SharedPtr& other) const;
  bool                        operator!=(const T* ptr) const;  

  // Accessors
  bool                        IsUnique() const;

  // Methods
  void                        RemoveOwnership();
  void                        Reset();

private:
  // Allow private member access to copy constructor & assignment operator method versions which require static_cast conversion. 
  template <class U, typename CounterType, class DeleterClass>
  friend class SharedPtr;

  template <class T, typename CounterType>
  friend class WeakPtr;

  T*                          mpPtr;
  SharedCounter<CounterType>* mpCounter;

  void                        Swap(SharedPtr& other);
};

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType, class DeleterClass>
inline SharedPtr<T, CounterType, DeleterClass>::SharedPtr()
  : mpPtr(nullptr)
  , mpCounter(nullptr) {}

template <class T, typename CounterType, class DeleterClass>
inline SharedPtr<T, CounterType, DeleterClass>::SharedPtr(const SharedPtr& other)
  : mpPtr(other.mpPtr)
  , mpCounter(other.mpCounter)
{
  if (mpPtr) ++mpCounter->count;
}

template <class T, typename CounterType, class DeleterClass>
template <class U, class DeleterClassU>
inline SharedPtr<T, CounterType, DeleterClass>::SharedPtr(const SharedPtr<U, CounterType, DeleterClassU>& other)
  : mpPtr(static_cast<T*>(other.mpPtr))
  , mpCounter(other.mpCounter)
{
  if (mpPtr) ++mpCounter->count;
}

template <class T, typename CounterType, class DeleterClass>
template <class U>
inline SharedPtr<T, CounterType, DeleterClass>::SharedPtr(U* ptr)
  : mpPtr(static_cast<T*>(ptr))
  , mpCounter(nullptr)
{
  if (mpPtr) mpCounter = E_NEW(SharedCounter<CounterType>);
}

template <class T, typename CounterType, class DeleterClass>
inline SharedPtr<T, CounterType, DeleterClass>::~SharedPtr()
{
  Reset();
}

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType, class DeleterClass>
inline SharedPtr<T, CounterType, DeleterClass>& SharedPtr<T, CounterType, DeleterClass>::operator=(const SharedPtr& other)
{
  SharedPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType, class DeleterClass>
template <class U, class DeleterClassU>
inline SharedPtr<T, CounterType, DeleterClass>& SharedPtr<T, CounterType, DeleterClass>::operator=(const SharedPtr<U, CounterType, DeleterClassU>& other)
{
  SharedPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType, class DeleterClass>
inline SharedPtr<T, CounterType, DeleterClass>& SharedPtr<T, CounterType, DeleterClass>::operator=(T* ptr)
{
  SharedPtr(ptr).Swap(*this);
  return *this;
}

template <class T, typename CounterType, class DeleterClass>
inline T& SharedPtr<T, CounterType, DeleterClass>::operator*() const
{
  E_ASSERT_PTR(mpPtr);
  return *mpPtr;
}

template <class T, typename CounterType, class DeleterClass>
inline T* SharedPtr<T, CounterType, DeleterClass>::operator->() const
{
  E_ASSERT_PTR(mpPtr);
  return mpPtr;
}

template <class T, typename CounterType, class DeleterClass>
inline bool SharedPtr<T, CounterType, DeleterClass>::operator==(const SharedPtr& other) const
{
  return mpPtr == other.mpPtr;
}

template <class T, typename CounterType, class DeleterClass>
inline bool SharedPtr<T, CounterType, DeleterClass>::operator==(const T* ptr) const
{
  return mpPtr == ptr;
}

template <class T, typename CounterType, class DeleterClass>
inline bool SharedPtr<T, CounterType, DeleterClass>::operator!=(const SharedPtr& other) const
{
  return mpPtr != other.mpPtr;
}

template <class T, typename CounterType, class DeleterClass>
inline bool SharedPtr<T, CounterType, DeleterClass>::operator!=(const T* ptr) const
{
  return mpPtr != ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType, class DeleterClass>
inline bool SharedPtr<T, CounterType, DeleterClass>::IsUnique() const
{
  return (mpPtr == nullptr) ? false : mpCounter->count == 1;
}

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType, class DeleterClass>
inline void SharedPtr<T, CounterType, DeleterClass>::RemoveOwnership()
{
  E_ASSERT_MSG(IsUnique(), E_ASSERT_MSG_SHARED_PTR_UNIQUE);
  mpPtr = nullptr;
}

template <class T, typename CounterType, class DeleterClass>
inline void SharedPtr<T, CounterType, DeleterClass>::Reset()
{
  if (mpCounter) 
  {      
    --mpCounter->count;
    if (mpCounter->count == 0)
    { 
      DeleterClass::Delete(mpPtr);
      if (mpCounter->weakCount == 0)
      {
        E_DELETE(mpCounter);
      }
    }
    mpPtr = nullptr;
    mpCounter = nullptr;
  }
}

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType, class DeleterClass>
inline void SharedPtr<T, CounterType, DeleterClass>::Swap(SharedPtr& other)
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
