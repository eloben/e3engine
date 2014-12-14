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

/** @file CharArray.h
This file defines the CharArray class. CharArray is a lightweight fixed-size string representation.
*/

#ifndef E3_CHAR_ARRAY_H
#define E3_CHAR_ARRAY_H

#include "Text.h"
#include <Containers/Array.h>

/*----------------------------------------------------------------------------------------------------------------------
CharArray assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE  "String length (%d) must be smaller than array size (%d)"

namespace E
{
namespace Text
{
  /*----------------------------------------------------------------------------------------------------------------------
  CharArray

  Please note that this class has the following usage contract: 

  1. This class is based on DynamicArray and is intended as replacement to a raw T*.
  2. Strings are ensure to be null terminated.
  3. Length does not include the null termination character.
  4. Copy construction, assignment and comparison operators work with different size CharArray instances.
  5. Copy construction from raw data expects a valid string length value (< Size).
  6. Operator + is only allowed between CharArray instances of the same size.
  7. SetLength parameter value must be smaller than Size (it will E_ASSERT_MSG otherwise).
  ----------------------------------------------------------------------------------------------------------------------*/
  template <typename T, size_t Size>
  class CharArray
  {
  public:
    // Types
    typedef typename T CharType;

    // Constants
    static const size_t kSize = Size;

	  CharArray();
    template<size_t OtherSize>
    CharArray(const CharArray<T, OtherSize>& other); // Non-explicit to be used by friend operators
    CharArray(const T* pStr);                        // Non-explicit to be used by friend operators
    CharArray(const T* pStr, size_t length);
    ~CharArray();

    // Operators
    template <size_t OtherSize>
    CharArray<T, Size>&               operator=(const CharArray<T, OtherSize>& other);
    T                                 operator[](size_t index) const;
    T&                                operator[](size_t index);
    template <size_t OtherSize>        
    bool                              operator==(const CharArray<T, OtherSize>& other) const;
    bool                              operator==(const T* pStr) const;
    template <size_t OtherSize>        
    bool                              operator!=(const CharArray<T, OtherSize>& other) const;
    bool                              operator!=(const T* pStr) const;
    template <size_t OtherSize>
    CharArray<T, Size>                operator+(const CharArray<T, OtherSize>& other);
    CharArray<T, Size>                operator+(const T* pStr);
    template <size_t OtherSize>       
    CharArray<T, Size>&               operator+=(const CharArray<T, OtherSize>& other);
    CharArray<T, Size>&               operator+=(const T* pStr);
    CharArray<T, Size>&               operator+=(T c);
  
    // Friend operators
    template <typename T, size_t Size>
    friend CharArray<T, Size>         operator+(const T* pStrA, const CharArray<T, Size>& strB);
    template <typename T, size_t Size>
    friend bool	                      operator==(const T* pStrA, const CharArray<T, Size>& strB);
    template <typename T, size_t Size>
    friend bool                       operator!=(const T* pStrA, const CharArray<T, Size> &strB);

    // Getters
    size_t                            GetLength() const;
    const T*                          GetPtr() const;
    size_t                            GetSize() const;
    void                              SetLength(size_t v);

    void                              Append(const T* pStr, size_t length);
    static const CharArray<T, Size>&  Empty();
    void                              Clear();
    void                              Print(const T* pFormattedSourceStr, ... );

  private:
    void                              Initialize(const T* pStr, size_t length);

    Containers::Array<T, Size>        mData;
    size_t                            mLength;
  };

  /*----------------------------------------------------------------------------------------------------------------------
  CharArray initialization & finalization
  ----------------------------------------------------------------------------------------------------------------------*/
  
  template <typename T, size_t Size>
  inline CharArray<T, Size>::CharArray()
    : mLength(0) { mData[mLength] = 0;}

  template <typename T, size_t Size>
  template <size_t OtherSize>
  inline CharArray<T, Size>::CharArray(const CharArray<T, OtherSize>& other)
  {
    Initialize(other.GetPtr(), other.GetLength());
  }

  template <typename T, size_t Size>
  inline CharArray<T, Size>::CharArray(const T* pStr)
  {
    Initialize(pStr, Text::GetLength(pStr));
  }

  template <typename T, size_t Size>
  inline CharArray<T, Size>::CharArray(const T* pStr, size_t length)
  {
    Initialize(pStr, length);
  }

  template <typename T, size_t Size>
  inline CharArray<T, Size>::~CharArray()
  {
  }

  /*----------------------------------------------------------------------------------------------------------------------
  CharArray operators
  ----------------------------------------------------------------------------------------------------------------------*/

  template <typename T, size_t Size>
  template <size_t OtherSize>
  inline CharArray<T, Size>& CharArray<T, Size>::operator=(const CharArray<T, OtherSize>& other)
  {
    Initialize(other.GetPtr(), other.GetLength());
    return *this;
  }

  template <typename T, size_t Size>
  inline T CharArray<T, Size>::operator[](size_t index) const
  {
    return mData[index];
  }

  template <typename T, size_t Size>
  inline T& CharArray<T, Size>::operator[](size_t index)
  {
    return mData[index];
  }

  template <typename T, size_t Size>
  template <size_t OtherSize>
  inline bool CharArray<T, Size>::operator==(const CharArray<T, OtherSize>& other) const
  {
    if (mLength != other.GetLength()) return false;
    return (*this) == other.GetPtr();
  }

  template <typename T, size_t Size>
  inline bool CharArray<T, Size>::operator==(const T* pStr) const
  {
    return Memory::IsEqual(mData.GetPtr(), pStr, (mLength + 1));
  }

  template <typename T, size_t Size>
  template <size_t OtherSize>
  inline bool CharArray<T, Size>::operator!=(const CharArray<T, OtherSize>& other) const
  {
    return !((*this) == other);
  }

  template <typename T, size_t Size>
  inline bool CharArray<T, Size>::operator!=(const T* pStr) const
  {
    return !((*this) == pStr);
  }

  template <typename T, size_t Size>
  template <size_t OtherSize>
  inline CharArray<T, Size> CharArray<T, Size>::operator+(const CharArray<T, OtherSize>& other)
  {
    E_ASSERT_MSG(Size > mLength + other.mLength, E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE);
    CharArray<T, Size> str;
    str.mLength = mLength + other.mLength;
    str.mData.Copy(mData.GetPtr(), mLength);
    str.mData.Copy(other.GetPtr(), other.mLength, mLength);
    str.mData[str.mLength] = 0;
    return str;
  }

  template <typename T, size_t Size>
  inline CharArray<T, Size> CharArray<T, Size>::operator+(const T* pStr)
  {
    size_t strLength = Text::GetLength(pStr);
    E_ASSERT_MSG(Size > mLength + strLength, E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE);
    CharArray<T, Size> str;
    str.mLength = mLength + strLength;
    str.mData.Copy(mData.GetPtr(), mLength);
    str.mData.Copy(pStr, strLength, mLength);
    str.mData[str.mLength] = 0;
    return str;
  }

  template <typename T, size_t Size>
  template <size_t OtherSize>
  inline CharArray<T, Size>& CharArray<T, Size>::operator+=(const CharArray<T, OtherSize>& other)
  {
    Append(other.GetPtr(), other.GetLength());
    return *this;
  }

  template <typename T, size_t Size>
  inline CharArray<T, Size>& CharArray<T, Size>::operator+=(const T* pStr)
  {
    Append(pStr, Text::GetLength(pStr));
    return *this;
  }

  template <typename T, size_t Size>
  inline CharArray<T, Size>& CharArray<T, Size>::operator+=(T c)
  {
    E_ASSERT_MSG(Size > mLength + 1, E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE, mLength + 1, Size);
    mData[mLength++] = c;
    mData[mLength] = 0;

    return *this;
  }

  /*----------------------------------------------------------------------------------------------------------------------
  CharArray friend operators
  ----------------------------------------------------------------------------------------------------------------------*/

  template <typename T, size_t Size>
  inline CharArray<T, Size> operator+(const T* pStrA, const CharArray<T, Size>& strB)  
  {
    size_t strLength = Text::GetLength(pStrA);
    E_ASSERT_MSG(Size > strLength + strB.mLength, E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE);

    CharArray<T, Size> str;
    str.mLength = strLength + strB.mLength;
    str.mData.Copy(pStrA, strLength);
    str.mData.Copy(strB.GetPtr(), strB.mLength, strLength);
    str.mData[str.mLength] = 0;

    return str;
  }

  template <typename T, size_t Size>
  inline bool operator==(const T* pStrA, const CharArray<T, Size> &strB)  
  {
    return strB == pStrA;
  }

  template <typename T, size_t Size>
  inline bool operator!=(const T* pStrA, const CharArray<T, Size> &strB)  
  {
    return strB != pStrA;
  }

  /*----------------------------------------------------------------------------------------------------------------------
  CharArray accessors
  ----------------------------------------------------------------------------------------------------------------------*/

  template <typename T, size_t Size>
  inline size_t CharArray<T, Size>::GetLength() const
  {
    return mLength;
  }

  template <typename T, size_t Size>
  inline size_t CharArray<T, Size>::GetSize() const
  {
    return Size;
  }

  template <typename T, size_t Size>
  inline const T* CharArray<T, Size>::GetPtr() const
  {
    return mData.GetPtr();
  }

  template <typename T, size_t Size>
  inline void CharArray<T, Size>::SetLength(size_t v)
  {
    E_ASSERT_MSG (Size > v, E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE);
    mLength = v;
    mData[mLength] = 0;
  }

  /*----------------------------------------------------------------------------------------------------------------------
  CharArray methods
  ----------------------------------------------------------------------------------------------------------------------*/

  template <typename T, size_t Size>
  inline void CharArray<T, Size>::Append(const T* pStr, size_t length)
  {
    E_ASSERT_MSG(Size > mLength + length, E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE, mLength + length, Size);
    mData.Copy(pStr, length, mLength);
    mLength += length;
    mData[mLength] = 0;
  }

  template <typename T, size_t Size>
  inline void CharArray<T, Size>::Clear()
  {
    mLength = 0;
    mData.SetZero();
  }

  template <typename T, size_t Size>
  inline const CharArray<T, Size>& CharArray<T, Size>::Empty()
  {
    static CharArray<T, Size> sEmpty;
    return sEmpty;
  }

  template <typename T, size_t Size>
  inline void CharArray<T, Size>::Print(const T* pFormattedSourceStr, ...) 
  {
    va_list variableArguments;
    va_start(variableArguments, pFormattedSourceStr);
    mLength = Text::PrintArguments(mData.GetPtr(), Size - 1, pFormattedSourceStr, variableArguments);
    va_end(variableArguments);
  }

  /*----------------------------------------------------------------------------------------------------------------------
  CharArray private methods
  ----------------------------------------------------------------------------------------------------------------------*/

  template <typename T, size_t Size>
  inline void CharArray<T, Size>::Initialize(const T* pStr, size_t length)
  {
    E_ASSERT_MSG(Size > length, E_ASSERT_MSG_CHAR_ARRAY_LENGTH_VALUE);
    mLength = length;
    mData.Copy(pStr, mLength);
    mData[mLength] = 0;
  }
}
}

#endif
