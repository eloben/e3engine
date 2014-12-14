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

// Created 13-Sep-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Array.h
This file defines the Array class. This implementation is based on the original by:
Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
*/

#ifndef E3_ARRAY_H
#define E3_ARRAY_H

#include <Base.h>
#include <Math/Comparison.h>
#include <Memory/Memory.h>
#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
Array assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_ARRAY_INDEX_VALUE      "Index value (%d) must be smaller than the static array size (%d)"
#define E_ASSERT_MSG_ARRAY_COPY_SIZE_VALUE  "Copy count (%d) must be equal or smaller than the static array size (%d)"

namespace E
{
namespace Containers
{
/*----------------------------------------------------------------------------------------------------------------------
Array

Please note that this class has the following usage contract: 

1. The maximum number of elements is defined by Size
2. Valid index values should be passed to methods using an index as argument.
3. Copy checks memory bounds (unsafe).
4. TryCopy checks memory bounds and return the copied count.
----------------------------------------------------------------------------------------------------------------------*/
template <class T, size_t Size>
class Array
{
public:
  static const size_t kSize = Size;

  const T& operator [] (size_t index) const;
  T& operator [] (size_t index);

  size_t		GetByteSize() const;
  const T*	GetPtr() const;
  T*			  GetPtr();
  size_t    GetSize() const;
  void      SetZero();

  void      Copy(const T* pData, size_t count, size_t startIndex = 0);
  size_t    TryCopy(const T* pData, size_t count, size_t startIndex = 0);
    
private:
  T         mpPtr[Size];

  // This class defines a copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
Array operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, size_t Size>
inline const T& Array<T, Size>::operator [] (size_t index) const
{
  E_ASSERT_MSG(Size > index, E_ASSERT_MSG_ARRAY_INDEX_VALUE, index, Size);
  return mpPtr[index];
}

template <class T, size_t Size>
inline T& Array<T, Size>::operator [] (size_t index)
{
  E_ASSERT_MSG(Size > index, E_ASSERT_MSG_ARRAY_INDEX_VALUE, index, Size);
  return mpPtr[index];
}

/*----------------------------------------------------------------------------------------------------------------------
Array accessors
----------------------------------------------------------------------------------------------------------------------*/

template<class T, size_t Size>
inline size_t Array<T, Size>::GetByteSize() const
{
  return sizeof(mpPtr);
}

template<class T, size_t Size>
const T* Array<T, Size>::GetPtr() const
{
  return mpPtr;
}

template<class T, size_t Size>
T* Array<T, Size>::GetPtr()
{
  return mpPtr;
}

template<class T, size_t Size>
inline size_t Array<T, Size>::GetSize() const
{
  return Size;
}

template<class T, size_t Size>
inline void Array<T, Size>::SetZero()
{
  Memory::Zero(mpPtr, Size);
}  

/*----------------------------------------------------------------------------------------------------------------------
Array methods
----------------------------------------------------------------------------------------------------------------------*/

template<class T, size_t Size>
inline void Array<T, Size>::Copy(const T* pData, size_t count, size_t startIndex)
{
  E_ASSERT_MSG(Size >= startIndex + count, E_ASSERT_MSG_ARRAY_COPY_SIZE_VALUE, startIndex + count, Size);
  Memory::Copy(&mpPtr[startIndex], pData, count);
}

template<class T, size_t Size>
inline size_t Array<T, Size>::TryCopy(const T* pData, size_t count, size_t startIndex)
{
  size_t copyCount = Math::Min(count, Size - startIndex);
  Copy(pData, copyCount, startIndex);
  return copyCount;
}
}
}

#endif
