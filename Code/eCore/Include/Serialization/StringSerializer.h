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

// Created 058-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file StringSerializer.h
This file defines the StringSerializer class for basic type value serialization to a string buffer.
StringSerializer implements the ISerializer interface. Its implementation is based on the StringBuffer class and
std::StringSerializer.
*/

#ifndef E3_STRING_SERIALIZER_H
#define E3_STRING_SERIALIZER_H

#include "ISerializer.h"
#include <Text/CharList.h>
#include <Base.h>
#include <Assertion/Assert.h>
#include <sstream>
#include <iomanip>

/*----------------------------------------------------------------------------------------------------------------------
StringSerializer assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_STRING_SERIALIZER_DELIMITER  "StringBuffer stream delimiter expected"

namespace E
{
namespace Serialization
{
/*----------------------------------------------------------------------------------------------------------------------
StringSerializer

Please note that this class has the following usage contract: 

1. A token delimiter character ' ' is used to differentiate serialized values.
2. StringBuffer values include an additional delimiter character '"' to deserialize this type correctly,
therefore StringBuffer values cannot contain this character.
----------------------------------------------------------------------------------------------------------------------*/
class StringSerializer : public ISerializer
{
public:	
	StringSerializer();

  // Operators
  ISerializer&      operator<<(bool v);
  ISerializer&      operator<<(I8 v);
  ISerializer&      operator<<(U8 v);
  ISerializer&      operator<<(I16 v);
  ISerializer&      operator<<(U16 v);
  ISerializer&      operator<<(I32 v);
  ISerializer&      operator<<(U32 v);
  ISerializer&      operator<<(I64 v);
  ISerializer&      operator<<(U64 v);
  ISerializer&      operator<<(F32 v);
  ISerializer&      operator<<(D64 v);
  ISerializer&      operator<<(const StringBuffer& v);
  ISerializer&      operator<<(const String& v);
  ISerializer&      operator<<(const char* pTr);

  void              operator>>(bool& v);
  void              operator>>(I8& v);
  void              operator>>(U8& v);
  void              operator>>(I16& v);
  void              operator>>(U16& v);
  void              operator>>(I32& v);
  void              operator>>(U32& v);
  void              operator>>(I64& v);
  void              operator>>(U64& v);
  void              operator>>(F32& v);
  void              operator>>(D64& v);
  void              operator>>(StringBuffer& v);
  void              operator>>(String& v);
  
  // Accessors
  size_t            GetLength() const;
  size_t            GetSize() const;
  const char*       GetPtr() const;
  void              SetBegin();

  // Methods
  void              BeginTag(const StringBuffer& name);
  void              Clear();
  void              EndTag();
  void              Ignore(U32 v = 1);

private:
  static const char kTokenSeparator = ' ';
  static const char kStringDelimiter = '"';

  StringBuffer      mBuffer;
  size_t            mBufferPosition;

  E_DISABLE_COPY_AND_ASSSIGNMENT(StringSerializer)
};

/*----------------------------------------------------------------------------------------------------------------------
StringSerializer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

inline StringSerializer::StringSerializer()
  : mBufferPosition(0)
{
}

/*----------------------------------------------------------------------------------------------------------------------
StringSerializer operators
----------------------------------------------------------------------------------------------------------------------*/

inline ISerializer& StringSerializer::operator<<(bool v)
{
  return operator<<(static_cast<U64>(v));
}

inline ISerializer& StringSerializer::operator<<(I8 v)
{
  mBuffer << v;
  mBuffer << kTokenSeparator;
  return *this;
}

inline ISerializer& StringSerializer::operator<<(U8 v)
{
  return operator<<(static_cast<I8>(v));
}

inline ISerializer& StringSerializer::operator<<(I16 v)
{
  return operator<<(static_cast<I64>(v));
}

inline ISerializer& StringSerializer::operator<<(U16 v)
{
  return operator<<(static_cast<U64>(v));
}

inline ISerializer& StringSerializer::operator<<(I32 v)
{
  return operator<<(static_cast<I64>(v));
}

inline ISerializer& StringSerializer::operator<<(U32 v)
{
  return operator<<(static_cast<U64>(v));
}

inline ISerializer& StringSerializer::operator<<(I64 v)
{
  std::ostringstream oss;
  oss << v << kTokenSeparator;
  mBuffer << oss.str().c_str();
  return *this;
}

inline ISerializer& StringSerializer::operator<<(U64 v)
{
  std::ostringstream oss;
  oss << v << kTokenSeparator;
  mBuffer << oss.str().c_str();
  return *this;
}

inline ISerializer& StringSerializer::operator<<(F32 v)
{
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(5) << v << kTokenSeparator;
  mBuffer << oss.str().c_str();
  return *this;
}

inline ISerializer& StringSerializer::operator<<(D64 v)
{
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(7) << v << kTokenSeparator;
  mBuffer << oss.str().c_str();
  return *this;
}

inline ISerializer& StringSerializer::operator<<(const StringBuffer& v)
{
  mBuffer << kStringDelimiter;
  mBuffer.Append(v.GetPtr(), v.GetLength());
  mBuffer << kStringDelimiter;
  mBuffer << kTokenSeparator;
  return *this;
}

inline ISerializer& StringSerializer::operator<<(const String& v)
{
  mBuffer << kStringDelimiter;
  mBuffer.Append(v.GetPtr(), v.GetLength());
  mBuffer << kStringDelimiter;
  mBuffer << kTokenSeparator;
  return *this;
}

inline ISerializer& StringSerializer::operator<<(const char* pStr)
{
  size_t strLength = Text::GetLength(pStr);
  mBuffer << kStringDelimiter;
  mBuffer.Append(pStr, strLength);
  mBuffer << kStringDelimiter;
  mBuffer << kTokenSeparator;
  return *this;
}

inline void StringSerializer::operator>>(bool& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(I8& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(U8& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(I16& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(U16& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(I32& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(U32& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(I64& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(U64& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(F32& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(D64& v)
{
  std::istringstream iss(std::string(&mBuffer[mBufferPosition], mBuffer.GetLength() - mBufferPosition));
  iss >> v;
  mBufferPosition += static_cast<size_t>(iss.tellg()) + 1;
}

inline void StringSerializer::operator>>(StringBuffer& v)
{
  E_ASSERT_MSG(mBuffer[mBufferPosition++] == kStringDelimiter, E_ASSERT_MSG_STRING_SERIALIZER_DELIMITER);
  size_t strBegin = mBufferPosition;
  while (mBuffer[mBufferPosition++] != kStringDelimiter);
  v = StringBuffer(&mBuffer[strBegin], mBufferPosition++ - strBegin - 1); // Increment mBufferPosition to include the token delimiter
}

inline void StringSerializer::operator>>(String& v)
{
  E_ASSERT_MSG(mBuffer[mBufferPosition++] == kStringDelimiter, E_ASSERT_MSG_STRING_SERIALIZER_DELIMITER);
  size_t strBegin = mBufferPosition;
  while (mBuffer[mBufferPosition++] != kStringDelimiter);
  v = String(&mBuffer[strBegin], mBufferPosition++ - strBegin - 1); // Increment mBufferPosition to include the token delimiter
}

/*----------------------------------------------------------------------------------------------------------------------
StringSerializer accessors
----------------------------------------------------------------------------------------------------------------------*/

inline size_t StringSerializer::GetLength() const
{
  return mBuffer.GetLength();
}

inline size_t StringSerializer::GetSize() const
{
  return mBuffer.GetSize();
}

inline const char* StringSerializer::GetPtr() const
{
  return mBuffer.GetPtr();
}

inline void StringSerializer::SetBegin()
{
  mBufferPosition = 0;
}

/*----------------------------------------------------------------------------------------------------------------------
StringSerializer methods
----------------------------------------------------------------------------------------------------------------------*/

inline void StringSerializer::BeginTag(const StringBuffer&)
{
}

inline void StringSerializer::Clear()
{
  mBuffer.Clear();
  mBufferPosition = 0;
}

inline void StringSerializer::EndTag()
{
}

inline void StringSerializer::Ignore(U32 v)
{
  mBufferPosition += v;
}
}
}

#endif
