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

// Created 06-May-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file DynamicArray.h
This file defines the DynamicArray class. DynamicArray is the most basic dynamic sequential container, aiming a lean and mean
implementation.
*/

#ifndef E3_DYNAMIC_ARRAY_H
#define E3_DYNAMIC_ARRAY_H

#include <Base.h>
#include <Memory/Memory.h>
#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
DynamicArray assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_DYNAMIC_ARRAY_COPY_SIZE_VALUE  "Copy count (%d) must be equal or smaller than size (%d)"
#define E_ASSERT_MSG_DYNAMIC_ARRAY_COPY_PTR_VALUE   "Copy source cannot be nullptr with non-zero copy count (%d)"
#define E_ASSERT_MSG_DYNAMIC_ARRAY_INDEX_VALUE      "Index value (%d) must be smaller than size (%d)"

namespace E
{
namespace Containers
{
/*----------------------------------------------------------------------------------------------------------------------
DynamicArrayBase

DynamicArrayBase groups all non-template dependent methods from DynamicArray to reduce template code bloat.
----------------------------------------------------------------------------------------------------------------------*/
class DynamicArrayBase
{
public:
  const Memory::IAllocator* GetAllocatorInternal() const;
  size_t                    GetSizeInternal() const;
  void                      SetAllocatorInternal(Memory::IAllocator* p);

protected:
  Memory::IAllocator*       mpAllocator;
  size_t                    mSize;
};

/*----------------------------------------------------------------------------------------------------------------------
DynamicArray

Please note that this class has the following usage contract: 

1. Copy checks memory bounds and nullptr source with non-zero copy count to avoid heap corruption (assert).
2. The non const version of operator [] checks index bounds to prevent invalid write accesses. Read access is
allowed, letting classes using DynamicArray to check the bounds if necessary.
3. Swap or operator= can be used to reallocate the array on demand.
4. Reserve only reallocates when the parameter size value is bigger than the array size.

Note that you can use Resize(0) to destroy the array content and deallocate the memory.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class DynamicArray
{
public:
  DynamicArray();
  explicit DynamicArray(size_t size);
  DynamicArray(const DynamicArray& other);
  DynamicArray(const T* pData, size_t count);
  ~DynamicArray();

  DynamicArray&             operator=(const DynamicArray& other);
  const T&                  operator[](size_t index) const;
  T&                        operator[](size_t index);
  bool                      operator==(const DynamicArray& other) const;
  bool                      operator==(const T* pPtr) const;    
  bool                      operator!=(const DynamicArray& other) const;
  bool                      operator!=(const T* pPtr) const;      

  const Memory::IAllocator* GetAllocator() const;
  size_t		                GetByteSize() const;
  const T*	                GetPtr() const;
  T*			                  GetPtr();
  size_t                    GetSize() const;
  void                      SetAllocator(Memory::IAllocator* p);
  void                      SetZero();

  void                      Copy(const T* pData, size_t count, size_t startIndex = 0);
  void                      Reserve(size_t size);
  void                      Resize(size_t size);
  void                      Swap(DynamicArray& other);
    
private:
  Memory::IAllocator*       mpAllocator;
  T*                        mpPtr;
  size_t                    mSize;

  // This class defines a copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
DynamicArray initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline DynamicArray<T>::DynamicArray()
  : mpAllocator(Memory::Global::GetAllocator())
  , mpPtr(nullptr)
  , mSize(0) {}

template <typename T>
inline DynamicArray<T>::DynamicArray(size_t size)
  : mpAllocator(Memory::Global::GetAllocator())
  , mpPtr(E_NEW(T, size, mpAllocator, Memory::IAllocator::eTagArrayNew))
  , mSize(size) {}

template <typename T>
inline DynamicArray<T>::DynamicArray(const DynamicArray& other)
  : mpAllocator(other.mpAllocator)
  , mpPtr(E_NEW(T, other.mSize, mpAllocator, Memory::IAllocator::eTagArrayNew))
  , mSize(other.mSize)
{
  Copy(other.GetPtr(), mSize);
}

template <typename T>
inline DynamicArray<T>::DynamicArray(const T* pData, size_t count)
  : mpAllocator(Memory::Global::GetAllocator())
  , mpPtr(E_NEW(T, count, mpAllocator, Memory::IAllocator::eTagArrayNew))
  , mSize(count)
{
  Copy(pData, mSize);
}

template <typename T>
inline DynamicArray<T>::~DynamicArray()
{
  E_DELETE(mpPtr, mSize, mpAllocator, Memory::IAllocator::eTagArrayDelete);
  mSize = 0;
  mpPtr = nullptr;
}

/*----------------------------------------------------------------------------------------------------------------------
DynamicArray operators
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other)
{
  DynamicArray<T>(other).Swap(*this);
  return *this;
}

template <typename T>
inline const T& DynamicArray<T>::operator[](size_t index) const
{
  E_ASSERT_MSG(index < mSize, E_ASSERT_MSG_DYNAMIC_ARRAY_INDEX_VALUE, index, mSize);
  return mpPtr[index];
}

template <typename T>
inline T& DynamicArray<T>::operator[](size_t index)
{
  E_ASSERT_MSG(index < mSize, E_ASSERT_MSG_DYNAMIC_ARRAY_INDEX_VALUE, index, mSize);
  return mpPtr[index];
}

template <typename T>
inline bool DynamicArray<T>::operator==(const DynamicArray<T>& other) const
{
  if (mSize != other.mSize) return false; 
  return Memory::IsEqual(mpPtr, other.mpPtr, mSize);
}

template <typename T>
inline bool DynamicArray<T>::operator==(const T* pStr) const
{
  if (pStr) return Memory::IsEqual(mpPtr, pStr, mSize);  
  return (mpPtr == nullptr);
}

template <typename T>
inline bool DynamicArray<T>::operator!=(const DynamicArray<T>& other) const
{
  return !operator==(other);
}

template <typename T>
inline bool DynamicArray<T>::operator!=(const T* pStr) const
{
  return !operator==(pStr);
}

/*----------------------------------------------------------------------------------------------------------------------
DynamicArray accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline const Memory::IAllocator* DynamicArray<T>::GetAllocator() const
{
  return mpAllocator;
}

template<typename T>
inline size_t DynamicArray<T>::GetByteSize() const
{
  return sizeof(T) * mSize;
}

template<typename T>
inline const T* DynamicArray<T>::GetPtr() const
{
  return mpPtr;
}

template<typename T>
inline T* DynamicArray<T>::GetPtr()
{
  return mpPtr;
}

template<typename T>
inline size_t DynamicArray<T>::GetSize() const
{
  return mSize;
}

template <typename T>
inline void DynamicArray<T>::SetAllocator(Memory::IAllocator* p)
{
  E_ASSERT_PTR(p);
  mpAllocator = p;
}

template<typename T>
inline void DynamicArray<T>::SetZero()
{
  Memory::Zero(mpPtr, mSize);
}  

/*----------------------------------------------------------------------------------------------------------------------
DynamicArray methods
----------------------------------------------------------------------------------------------------------------------*/

template<typename T>
inline void DynamicArray<T>::Copy(const T* pData, size_t count, size_t startIndex)
{
  E_ASSERT_MSG(mSize >= startIndex + count, E_ASSERT_MSG_DYNAMIC_ARRAY_COPY_SIZE_VALUE, startIndex + count, mSize);
  E_ASSERT_MSG(pData != nullptr || pData == nullptr && count == 0, E_ASSERT_MSG_DYNAMIC_ARRAY_COPY_PTR_VALUE, count);
  Memory::Copy(&mpPtr[startIndex], pData, count);
}

template <typename T>
inline void DynamicArray<T>::Reserve(size_t size)
{
  if (size > mSize)
  {
    DynamicArray<T>(size).Swap(*this);
  }
}

template <typename T>
inline void DynamicArray<T>::Resize(size_t size)
{
  DynamicArray<T>(size).Swap(*this);
}

template <typename T>
inline void DynamicArray<T>::Swap(DynamicArray<T>& other)
{
  // Create a temporary instance for shallow-copy.
  DynamicArray<T> temp;
  temp.mpAllocator = mpAllocator;
  temp.mpPtr = mpPtr;
  temp.mSize = mSize;

  // Swap instances
  mpAllocator = other.mpAllocator;
  mpPtr = other.mpPtr;
  mSize = other.mSize;

  other.mpAllocator = temp.mpAllocator;
  other.mpPtr = temp.mpPtr;
  other.mSize = temp.mSize;

  // Zero temporary instance memory pointer to prevent deallocation
  temp.mpPtr = nullptr;
}
}
}

#endif
