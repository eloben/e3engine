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

// Created 05-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ByteSerializer.h
This file defines the ByteSerializer class for basic type value serialization to a raw byte buffer.
ByteSerializer implements the ISerializer interface.
*/

#ifndef E3_BYTE_SERIALIZER_H
#define E3_BYTE_SERIALIZER_H

#include "ISerializer.h"
#include <Containers/List.h>
#include <Base.h>

namespace E
{
namespace Serialization
{
/*----------------------------------------------------------------------------------------------------------------------
ByteSerializer
----------------------------------------------------------------------------------------------------------------------*/
class ByteSerializer : public ISerializer
{
public:
	ByteSerializer();

  // Operators
  ISerializer&  operator<<(bool v);
  ISerializer&  operator<<(I8 v);
  ISerializer&  operator<<(U8 v);
  ISerializer&  operator<<(I16 v);
  ISerializer&  operator<<(U16 v);
  ISerializer&  operator<<(I32 v);
  ISerializer&  operator<<(U32 v);
  ISerializer&  operator<<(I64 v);
  ISerializer&  operator<<(U64 v);
  ISerializer&  operator<<(F32 v);
  ISerializer&  operator<<(D64 v);
  ISerializer&  operator<<(const StringBuffer& v);
  ISerializer&  operator<<(const String& v);
  ISerializer&  operator<<(const char* pTr);

  void          operator>>(bool& v);
  void          operator>>(I8& v);
  void          operator>>(U8& v);
  void          operator>>(I16& v);
  void          operator>>(U16& v);
  void          operator>>(I32& v);
  void          operator>>(U32& v);
  void          operator>>(I64& v);
  void          operator>>(U64& v);
  void          operator>>(F32& v);
  void          operator>>(D64& v);
  void          operator>>(StringBuffer& v);
  void          operator>>(String& v);

  // Accessors
  size_t	      GetLength() const;
  const Byte*   GetPtr() const;
  size_t	      GetSize() const;
  void          SetBegin();

  // Methods
  void          BeginTag(const StringBuffer& name);
  void          Clear();
  void          EndTag();
  void          Ignore(U32 v = 1);

private:
  Containers::List<Byte>  mBuffer;
  size_t                  mBufferPosition;

  E_DISABLE_COPY_AND_ASSSIGNMENT(ByteSerializer)
};

/*----------------------------------------------------------------------------------------------------------------------
ByteSerializer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

inline ByteSerializer::ByteSerializer()
  : mBufferPosition(0)
{
}

/*----------------------------------------------------------------------------------------------------------------------
ByteSerializer operators
----------------------------------------------------------------------------------------------------------------------*/

inline ISerializer& ByteSerializer::operator<<(bool v)
{
  return operator<<(static_cast<U8>(v ? 1 : 0));
}

inline ISerializer& ByteSerializer::operator<<(I8 v)
{
  return operator<<(static_cast<U8>(v));
}

inline ISerializer& ByteSerializer::operator<<(U8 v)
{
  mBuffer.PushBack(static_cast<Byte>(v));
  return *this;
}

inline ISerializer& ByteSerializer::operator<<(I16 v)
{
  return operator<<(static_cast<U16>(v));
}

inline ISerializer& ByteSerializer::operator<<(U16 v)
{
  mBuffer.PushBack(static_cast<Byte>(v >> 8));
  mBuffer.PushBack(static_cast<Byte>(v));
  return *this;
}

inline ISerializer& ByteSerializer::operator<<(I32 v)
{
  return operator<<(static_cast<U32>(v));
}

inline ISerializer& ByteSerializer::operator<<(U32 v)
{
  mBuffer.PushBack(static_cast<Byte>(v >> 24));
  mBuffer.PushBack(static_cast<Byte>(v >> 16));
  mBuffer.PushBack(static_cast<Byte>(v >> 8));
  mBuffer.PushBack(static_cast<Byte>(v));
  return *this;
}

inline ISerializer& ByteSerializer::operator<<(I64 v)
{
  return operator<<(static_cast<U64>(v));
}

inline ISerializer& ByteSerializer::operator<<(U64 v)
{
  mBuffer.PushBack(static_cast<Byte>(v >> 56));
  mBuffer.PushBack(static_cast<Byte>(v >> 48));
  mBuffer.PushBack(static_cast<Byte>(v >> 40));
  mBuffer.PushBack(static_cast<Byte>(v >> 32));
  mBuffer.PushBack(static_cast<Byte>(v >> 24));
  mBuffer.PushBack(static_cast<Byte>(v >> 16));
  mBuffer.PushBack(static_cast<Byte>(v >> 8));
  mBuffer.PushBack(static_cast<Byte>(v));
  return *this;
}

inline ISerializer& ByteSerializer::operator<<(F32 v)
{
  U32 uv = *((U32*)&v);
  return operator<<(uv);
}

inline ISerializer& ByteSerializer::operator<<(D64 v)
{
  U64 uv = *((U64*)&v);
  return operator<<(uv);
}

inline ISerializer& ByteSerializer::operator<<(const StringBuffer& v)
{
  operator<<(v.GetLength());
  const Byte* pBytes = reinterpret_cast<const Byte*>(v.GetPtr());
  mBuffer.PushBack(pBytes, v.GetLength());
  return *this;
}

inline ISerializer& ByteSerializer::operator<<(const String& v)
{
  operator<<(v.GetLength());
  const Byte* pBytes = reinterpret_cast<const Byte*>(v.GetPtr());
  mBuffer.PushBack(pBytes, v.GetLength());
  return *this;
}

inline ISerializer& ByteSerializer::operator<<(const char* pStr)
{
  size_t strLength = Text::GetLength(pStr);
  operator<<(strLength);
  mBuffer.PushBack(reinterpret_cast<const Byte*>(pStr), strLength);
  return *this;
}

inline void ByteSerializer::operator>>(bool& v)
{
  U8 uv;
  operator>>(uv);
  v = (uv == 1) ? true : false;
}

inline void ByteSerializer::operator>>(I8& v)
{
  U8 uv;
  operator>>(uv);
  v = uv;
}

inline void ByteSerializer::operator>>(U8& v)
{
  v = mBuffer[mBufferPosition++];
}

inline void ByteSerializer::operator>>(I16& v)
{
  U16 uv;
  operator>>(uv);
  v = uv;
}

inline void ByteSerializer::operator>>(U16& v)
{
  v =
    (mBuffer[mBufferPosition    ] << 8) |
    (mBuffer[mBufferPosition + 1]);
  mBufferPosition += 2;
}

inline void ByteSerializer::operator>>(I32& v)
{
  U32 uv;
  operator>>(uv);
  v = uv;
}

inline void ByteSerializer::operator>>(U32& v)
{
  v =
    (mBuffer[mBufferPosition    ] << 24) |
    (mBuffer[mBufferPosition + 1] << 16) |
    (mBuffer[mBufferPosition + 2] << 8) |
    (mBuffer[mBufferPosition + 3]);
   mBufferPosition += 4;
}

inline void ByteSerializer::operator>>(I64& v)
{
  U64 uv;
  operator>>(uv);
  v = uv;
}

inline void ByteSerializer::operator>>(U64& v)
{
  v = 
    (static_cast<U64>(mBuffer[mBufferPosition    ]) << 56) |
    (static_cast<U64>(mBuffer[mBufferPosition + 1]) << 48) |
    (static_cast<U64>(mBuffer[mBufferPosition + 2]) << 40) |
    (static_cast<U64>(mBuffer[mBufferPosition + 3]) << 32) |
    (static_cast<U64>(mBuffer[mBufferPosition + 4]) << 24) |
    (static_cast<U64>(mBuffer[mBufferPosition + 5]) << 16) |
    (static_cast<U64>(mBuffer[mBufferPosition + 6]) << 8) |
    (static_cast<U64>(mBuffer[mBufferPosition + 7]));
  mBufferPosition += 8;
}

inline void ByteSerializer::operator>>(F32& v)
{
  U32 uv;
  operator>>(uv);
  v = *((F32*)&uv);
}

inline void ByteSerializer::operator>>(D64& v)
{
  U64 uv;
  operator>>(uv);
  v = *((D64*)&uv);
}

inline void ByteSerializer::operator>>(StringBuffer& v)
{
  size_t length;
  operator>>(length);
  v = StringBuffer(reinterpret_cast<StringBuffer::CharType*>(&mBuffer[mBufferPosition]), length);
  mBufferPosition += length;
}

inline void ByteSerializer::operator>>(String& v)
{
  size_t length;
  operator>>(length);
  v = String(reinterpret_cast<StringBuffer::CharType*>(&mBuffer[mBufferPosition]), length);
  mBufferPosition += length;
}

/*----------------------------------------------------------------------------------------------------------------------
ByteSerializer accessors
----------------------------------------------------------------------------------------------------------------------*/

inline size_t ByteSerializer::GetLength() const
{
  return mBuffer.GetCount();
}

inline const Byte* ByteSerializer::GetPtr() const
{
  return mBuffer.GetPtr();
}

inline size_t ByteSerializer::GetSize() const
{
  return mBuffer.GetSize();
}

inline void ByteSerializer::SetBegin()
{
  mBufferPosition = 0;
}

/*----------------------------------------------------------------------------------------------------------------------
ByteSerializer methods
----------------------------------------------------------------------------------------------------------------------*/

inline void ByteSerializer::BeginTag(const StringBuffer&)
{
}

inline void ByteSerializer::Clear()
{
  mBuffer.Clear();
  mBufferPosition = 0;
}

inline void ByteSerializer::EndTag()
{
}

inline void ByteSerializer::Ignore(U32 v)
{
  mBufferPosition += v;
}
}
}

#endif
