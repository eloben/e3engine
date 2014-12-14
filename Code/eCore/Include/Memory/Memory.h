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

// Created 12-Aug-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Memory.h
This file defines the library memory utility methods.
*/

#ifndef E3_MEMORY_H
#define E3_MEMORY_H

#include "Allocator.h"
#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
MemoryHelper assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_MEMORY_INVALID_READ_MEMORY_ADDRESS     "Source memory address cannot be nullptr with read count (%d)"
#define E_ASSERT_MSG_MEMORY_DESTROY_POINTER_WITH_ZERO_COUNT "Source memory address is not nullptr but read count is zero"

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (Allocation)

Please note that this set of macros has the following usage contract:

1. E_NEW / E_DELETE are the operator new / delete equivalents based on the library global allocator
2. E_NEW always invokes the default constructor in non POD types.
3. In order to use the default operators new / delete with the global allocator the E_SETTING_OVERRIDE_NEW_AND_DELETE 
macro MUST be defined (see below)
----------------------------------------------------------------------------------------------------------------------*/
#define E_NEW(Type, ...)      Memory::Create<Type>(__VA_ARGS__)
#define E_DELETE(ptr, ...)    Memory::Destroy(ptr,__VA_ARGS__)

namespace E 
{
/*----------------------------------------------------------------------------------------------------------------------
Memory

Please note that these namespace methods have the following usage contract:

1. Create and Destroy methods are based on IAllocator interface: these methods intend to substitute the default new / 
delete object construction / destruction method in order to control memory allocation through an IAllocator class.
2. Zero requires the element count even for static arrays. Note that with an array e.g. I32 a[10] sizeof already gives 
the total size. For this purpose Zero can be used in combination with the E_ELEMENT_COUNT macro e.g:
Zero(a, E_ELEMENT_COUNT(a)).
3. Destructs requires a valid pointer.
----------------------------------------------------------------------------------------------------------------------*/
namespace Memory
{
template <typename T>
void        Construct(T* pObject, size_t count = 1); 
template <typename T>
T*          Copy(T* pTarget, const T* pSource, size_t count = 1);
template <typename T>
T*          Create(size_t count = 1, IAllocator* pAllocator = Global::GetAllocator(), IAllocator::Tag tag = IAllocator::eTagNew); 
template <typename T>
void        Destroy(T* pObject, size_t count = 1, IAllocator* pAllocator = Global::GetAllocator(), IAllocator::Tag tag = IAllocator::eTagDelete);
template <typename T>
void        Destruct(T* pObject, size_t count = 1); 
template <typename T>
inline bool IsEqual(const T* ptrA, const T* ptrB, size_t count = 1) { return (memcmp(ptrA, ptrB, sizeof(T) * count) == 0); }
template <typename T>
inline void Move(T* pTarget, const T* pSource, size_t count = 1) { memmove(pTarget, pSource, sizeof(T) * count); }
template <typename T>
inline void Zero(T* pTarget, size_t count = 1) { memset(pTarget, 0, sizeof(T) * count); }

/*----------------------------------------------------------------------------------------------------------------------
Deleter

Deleter supplies a default object destruction static method. Deleter type class are useful for memory manager 
template classes (e.g. smart pointers) as a flavored destruction method definable as a template argument.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct Deleter 
{ 
  static void Delete(T* ptr) { E_DELETE(ptr);}
};

/*----------------------------------------------------------------------------------------------------------------------
IDestroyer

IDestroyer supplies an object destruction interface for memory manager classes implementing destruction (e.g. 
factory classes) allowing third party memory classes such as smart pointers to delegate object destruction.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class IDestroyer 
{ 
public:
  virtual ~IDestroyer() {}
  virtual void Destroy(T* ptr) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
MemoryHelper

This template class allows different behavior for memory operations depending on whether the typename is POD or not.

Please note that this class has the following usage contract: 

1. The non-POD version of Create calls the default constructor forcing the use to define it for non POD types. Note
that this characteristics can be used to identify potential POD types on compile time. Use the E_DECLARE_POD macro
to define a custom type as POD.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, bool isPod = PodTypeTraits<T>::value>
struct MemoryHelper
{  
  static void Construct(T* pObject, size_t count); 
  static T*   Copy(T* pTarget, const T* pSource, size_t count);
  static T*   Create(size_t count, IAllocator* pAllocator, IAllocator::Tag tag);    
  static void Destroy(T* pObject, size_t count, IAllocator* pAllocator, IAllocator::Tag tag);
  static void Destruct(T* pObject, size_t count); 
};

/*----------------------------------------------------------------------------------------------------------------------
MemoryHelper specialization (POD types)
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct MemoryHelper<T, true>
{
  static void Construct(T* /*pObject*/, size_t /*count*/) {}

  static T* Copy(T* pTarget, const T* pSource, size_t count)
  { 
    E_ASSERT_MSG(pSource != nullptr && pTarget != nullptr|| !count, E_ASSERT_MSG_MEMORY_INVALID_READ_MEMORY_ADDRESS, count);
    return static_cast<T*>(memcpy(pTarget, pSource, sizeof(T) * count));
  }

  static T* Create(size_t count, IAllocator* pAllocator, IAllocator::Tag tag)
  {
    E_ASSERT_PTR(pAllocator);
    return reinterpret_cast<T*>(pAllocator->Allocate(sizeof(T) * count, tag));
  }

  static void Destroy(T* pObject, size_t, IAllocator* pAllocator, IAllocator::Tag tag)
  {
    E_ASSERT_PTR(pAllocator);
    pAllocator->Deallocate(pObject, tag);
  }

  static void Destruct(T* /*pObject*/, size_t /*count*/) {}
};

/*----------------------------------------------------------------------------------------------------------------------
MemoryHelper specialization (non-POD types)
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct MemoryHelper<T, false>
{
  static void Construct(T* pObject, size_t count)
  {
    E_ASSERT_PTR(pObject);
    T* pEnd = pObject + count;
    while (pEnd > pObject) new (--pEnd) T();
  }

  static T* Copy(T* pTarget, const T* pSource, size_t count)
  { 
    E_ASSERT_MSG(pSource != nullptr && pTarget != nullptr || !count, E_ASSERT_MSG_MEMORY_INVALID_READ_MEMORY_ADDRESS, count);
    T* pTargetEnd = pTarget + count;
    while (pTarget != pTargetEnd) *pTarget++ = *pSource++;
    // Return ptr start position
    return pTarget - count;
  }

  static T* Create(size_t count, IAllocator* pAllocator, IAllocator::Tag tag)
  {
    //E_DEBUG_MSG("MemoryHelper::Create [allocatpr 0x%p]", pAllocator);
    // Early out on zero count
    if (!count) return nullptr;  
    E_ASSERT_PTR(pAllocator);
    T* pObject = reinterpret_cast<T*>(pAllocator->Allocate(sizeof(T) * count, tag));
    Construct(pObject, count);
    return pObject;
  }

  static void Destroy(T* pObject, size_t count, IAllocator* pAllocator, IAllocator::Tag tag)
  {
    //E_DEBUG_MSG("MemoryHelper::Destroy [allocatpr 0x%p]", pAllocator);
    // Early out on nullptr ptr
    if (pObject == nullptr) return;
    E_ASSERT_MSG(pObject != nullptr && count || !count, E_ASSERT_MSG_MEMORY_DESTROY_POINTER_WITH_ZERO_COUNT);
    Destruct(pObject, count);
    E_ASSERT_PTR(pAllocator);
    pAllocator->Deallocate(pObject, tag);
  }

  static void Destruct(T* pObject, size_t count)
  {
    E_ASSERT_MSG(pObject != nullptr && count || !count, E_ASSERT_MSG_MEMORY_DESTROY_POINTER_WITH_ZERO_COUNT);
    T* pEnd = pObject + count;
    while (pEnd > pObject) (--pEnd)->~T();
  }
};
}

/*----------------------------------------------------------------------------------------------------------------------
Memory methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void Memory::Construct(T* pObject, size_t count)
{
  return Memory::MemoryHelper<T>::Construct(pObject, count);
}

template <typename T>
inline T* Memory::Copy(T* pTarget, const T* pSource, size_t count)
{ 
  return Memory::MemoryHelper<T>::Copy(pTarget, pSource, count);
}

template <class T>
inline T* Memory::Create(size_t count, IAllocator* pAllocator, IAllocator::Tag tag)
{
  return Memory::MemoryHelper<T>::Create(count, pAllocator, tag);
}

template <class T>
inline void Memory::Destroy(T* pObject, size_t count, IAllocator* pAllocator, IAllocator::Tag tag)
{
  MemoryHelper<T>::Destroy(pObject, count, pAllocator, tag);
}

template <class T>
inline void Memory::Destruct(T* pObject, size_t count)
{
  return Memory::MemoryHelper<T>::Destruct(pObject, count);
}
}

/*----------------------------------------------------------------------------------------------------------------------
Operators new / delete

Please note that this class has the following usage contract:

1. The E_SETTING_OVERRIDE_NEW_AND_DELETE macro must be defined in order to override default new and delete operators.
----------------------------------------------------------------------------------------------------------------------*/
#ifdef E_SETTING_OVERRIDE_NEW_AND_DELETE
  inline void operator delete(void* ptr)    { E::Memory::Global::GetAllocator()->Deallocate(ptr, E::Memory::IAllocator::eTagDelete); }
  inline void operator delete[](void* ptr)  { E::Memory::Global::GetAllocator()->Deallocate(ptr, E::Memory::IAllocator::eTagDelete); }
  inline void *operator new(size_t size)    { return E::Memory::Global::GetAllocator()->Allocate(size, E::Memory::IAllocator::eTagNew); }
  inline void *operator new[](size_t size)  { return E::Memory::Global::GetAllocator()->Allocate(size, E::Memory::IAllocator::eTagNew); }
#endif

#endif
