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

// Created 21-Sep-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file CharList.h
This file defines the CharList class. CharList represents a dynamic string buffer. It is implemented on
top of List.
*/

#ifndef E3_CHAR_LIST_H
#define E3_CHAR_LIST_H

#include <Containers/List.h>
#include "CharArray.h"
#include "CharArray.h"

namespace E
{
namespace Text
{
/*----------------------------------------------------------------------------------------------------------------------
CharList

Please note that this class has the following usage contract: 

1. StringBuffer is not a substitute of a default string, use CharArray or CharArray for that purpose.
2. StringBuffer allows working with char and wchar_t types offering also input conversion from other basic types. However
it is not intended as a serialization class as it does not allow to recover the original type value. For string based 
serialization use the ISerializer based class StringSerializer.
3. EnsureSize preserves existing context while Reserve just ensures memory allocation (content is not preserved).
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class CharList
{
public:
  // Types
  typedef typename T CharType;

  explicit CharList(size_t size = kDefaultMinSize);
  CharList(const CharList& other);  // Non-explicit to be used by friend operators
  CharList(const T* pStr);          // Non-explicit to be used by friend operators
  CharList(const T* pStr, size_t length);
  ~CharList();

  // Operators
  CharList&   operator=(const CharList& other);
  T           operator[](size_t index) const;
  T&          operator[](size_t index);

  bool        operator==(const CharList& other) const;
  template <size_t Size>
  bool	      operator==(const CharArray<T, Size>& other) const;
  bool        operator==(const T* pStr) const;
  bool        operator!=(const CharList& other) const;
  template <size_t Size>
  bool	      operator!=(const CharArray<T, Size>& other) const;
  bool        operator!=(const T* pStr) const;

  CharList&   operator<<(const CharList& str);
  template <size_t Size>   
  CharList&   operator<<(const CharArray<T, Size>& str);
  CharList&   operator<<(const T* pStr);
  CharList&   operator<<(bool b);
  CharList&   operator<<(T c);
  CharList&   operator<<(I32 v);
  CharList&   operator<<(U32 v);
  CharList&   operator<<(I64 v);
  CharList&   operator<<(U64 v);
  CharList&   operator<<(F32 v);
  CharList&   operator<<(D64 v);

  // Friend operators
  template <typename T, size_t Size>
  friend bool	operator==(const CharArray<T, Size>& a, const CharList& b);
  template <typename T>
  friend bool	operator==(const T* a, const CharList<T> &b);
  template <typename T, size_t Size>
  friend bool	operator!=(const CharArray<T, Size>& a, const CharList& b);
  template <typename T>
  friend bool	operator!=(const T* a, const CharList<T> &b);

  // Getters
  size_t      GetLength() const;
  size_t      GetSize() const;
  const T*    GetPtr() const;
  void        SetLength(size_t length);

  // Methods
  void        Append(const T* pStr, size_t length);
  void        Clear();
  void        Compact();
  void        EnsureSize(size_t size);
  void        Print(const T* pFormattedSourceStr, ... );
  void        Reserve(size_t size);

private:
  static const size_t kDefaultNumericalValueStringLength = 32;
  static const U8     kDefaultMinSize = 64;

  Containers::List<T> mList;

  template<typename U>  
  void                AppendValue(U v);
  void                AppendEnding();

  // Relying on default copy constructor and assignment operator
  // Note that copy construction is allowed through CharList(const T* pStr)
};

/*----------------------------------------------------------------------------------------------------------------------
CharList initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline CharList<T>::CharList(size_t size /* = kDefaultMinSize */)
  : mList(size)
{
  AppendEnding();
}

template <typename T>
inline CharList<T>::CharList(const CharList& other)
  : mList(other.mList)
{
  AppendEnding();
}

template <typename T>
inline CharList<T>::CharList(const T* pStr)
{
  Append(pStr, Text::GetLength(pStr));
}

template <typename T>
inline CharList<T>::CharList(const T* pStr, size_t length)
{
  Append(pStr, length);
}

template <typename T>
inline CharList<T>::~CharList()
{
}

/*----------------------------------------------------------------------------------------------------------------------
CharList operators
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline CharList<T>& CharList<T>::operator=(const CharList& other)
{
  mList = other.mList;
  return *this;
}

template <typename T>
inline T CharList<T>::operator[](size_t index) const
{
  return mList[index];
}

template <typename T>
inline T& CharList<T>::operator[](size_t index)
{
  return mList[index];
}

template <typename T>
inline bool CharList<T>::operator==(const CharList& other) const
{
  if (mList.GetCount() != other.mList.GetCount())
    return false;

  return Memory::IsEqual(mList.GetPtr(), other.mList.GetPtr(), mList.GetCount() + 1);
}

template <typename T>
template <size_t OtherSize>
inline bool CharList<T>::operator==(const CharArray<T, OtherSize>& other) const
{
  if (mList.GetCount() != other.GetLength())
    return false;

  return Memory::IsEqual(mList.GetPtr(), other.GetPtr(), mList.GetCount() + 1);
}

template <typename T>
inline bool CharList<T>::operator==(const T* pStr) const
{
  U32 strLength = Text::GetLength(pStr);
  if (GetLength() != strLength)
    return false;

  return Memory::IsEqual(mList.GetPtr(), pStr, mList.GetCount() + 1);
}

template <typename T>
inline bool CharList<T>::operator!=(const CharList& other) const
{
  return !((*this) == other);
}

template <typename T>
template <size_t OtherSize>
inline bool CharList<T>::operator!=(const CharArray<T, OtherSize>& other) const
{
  return !((*this) == other);
}

template <typename T>
inline bool CharList<T>::operator!=(const T* pStr) const
{
  return !((*this) == pStr);
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(const CharList& other)
{
  Append(other.GetPtr(), other.GetLength());
  return *this;
}

template <typename T>
template <size_t Size>
inline CharList<T>& CharList<T>::operator<<(const CharArray<T, Size>& str)
{
  Append(str.GetPtr(), str.GetLength());
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(const T* pStr)
{
  Append(pStr, Text::GetLength(pStr));
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(bool b)
{
  AppendValue(b);
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(T c)
{
  // Ensure current count + 1 + null terminator
  mList.EnsureSize(mList.GetCount() + 2);
  mList.PushBack(c);
  AppendEnding();
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(I32 v)
{
  AppendValue(v);
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(U32 v)
{
  AppendValue(v);
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(I64 v)
{
  AppendValue(v);
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(U64 v)
{
  AppendValue(v);
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(F32 v)
{
  AppendValue(v);
  return *this;
}

template <typename T>
inline CharList<T>& CharList<T>::operator<<(D64 v)
{
  AppendValue(v);
  return *this;
}

/*----------------------------------------------------------------------------------------------------------------------
CharList friend operators
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, size_t Size>
inline bool operator==(const CharArray<T, Size>& a, const CharList<T>& b)
{
  return b == a;
}

template <typename T>
inline bool operator==(const T* a, const CharList<T>& b)  
{
  return b == a;
}

template <typename T, size_t Size>
inline bool operator!=(const CharArray<T, Size>& a, const CharList<T>& b)
{
  return b != a;
}

template <typename T>
inline bool operator!=(const T* a, const CharList<T>& b)  
{
  return b != a;
}

/*----------------------------------------------------------------------------------------------------------------------
CharList accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline size_t CharList<T>::GetLength() const
{
  return mList.GetCount();
}

template <typename T>
inline size_t CharList<T>::GetSize() const
{
  return mList.GetSize();
}

template <typename T>
inline const T* CharList<T>::GetPtr() const
{
  return mList.GetPtr();
}

template <typename T>
inline void CharList<T>::SetLength(size_t length)
{
  mList.SetCount(length);
  AppendEnding();
}

/*----------------------------------------------------------------------------------------------------------------------
CharList methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
inline void CharList<T>::Append(const T* pStr, size_t length)
{
  // Ensure current count + length + null terminator
  mList.EnsureSize(mList.GetCount() + length + 1);
  mList.Copy(pStr, length, mList.GetCount());
  AppendEnding();
}

template <typename T>
inline void CharList<T>::Clear()
{
  mList.Clear();
}

template <typename T>
inline void CharList<T>::Compact()
{
  mList.Compact();
}

template <typename T>
inline void CharList<T>::EnsureSize(size_t size)
{
  mList.EnsureSize(size);
}

template <typename T>
inline void CharList<T>::Print(const T* pFormattedSourceStr, ...) 
{
  va_list variableArguments;
  va_start(variableArguments, pFormattedSourceStr);
  E_ASSERT_MSG(mData.GetSize() != 0, E_ASSERT_MSG_MATH_NON_ZERO_VALUE);
  mLength = Text::PrintArguments(mData.GetPtr(), static_cast<U32>(mData.GetSize()), pFormattedSourceStr, variableArguments);
  va_end(variableArguments);
}

template <typename T>
inline void CharList<T>::Reserve(size_t size)
{
  mList.Reserve(size);
}

/*----------------------------------------------------------------------------------------------------------------------
CharList private methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T>
template <typename U>
inline void CharList<T>::AppendValue(U v)
{
  T str[kDefaultNumericalValueStringLength];
  Append(str, Text::Print(str, kDefaultNumericalValueStringLength, v));
}

template <typename T>
inline void CharList<T>::AppendEnding()
{
  // Ending character should not count to the final string length but we 
  // must ensure string is correctly terminated. A Push-Pop does the job.
  mList.PushBack(0);
  mList.PopBack();
}
}
}

#endif
