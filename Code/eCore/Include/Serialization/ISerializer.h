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

// Created 04-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ISerializer.h
This file defines the ISerializer interface serialization. The interface defines operators for basic
types and two tag methods for serialization structuring when necessary.
*/

#ifndef E3_ISERIALIZER_H
#define E3_ISERIALIZER_H

#include <Base.h>
#include <Text/String.h>

namespace E
{
namespace Serialization
{
/*----------------------------------------------------------------------------------------------------------------------
ISerializer

Please note that this interface has the following usage contract: 

1. A const char* parameter operator is defined for ISerializer serialization and is intended to
be redirected to the StringBuffer parameter version. As it maybe observed only a StringBuffer parameter
version operator is available for deserialization.

This operator is added due to the fact that otherwise raw string serialization such as:

mySerializer << "A nice raw string";

would result in a call to the boolean parameter operator:

virtual ISerializer& operator<<(bool& v) = 0;

instead of the const StringBuffer& parameter version:

virtual ISerializer& operator<<(const StringBuffer& v) = 0;

This is due to the facet that even that the StringBuffer class allows implicit conversions from 
const char*, the compiler will find that the boolean operator would be a better match to a 
raw string input due to its pointer nature (evaluating const char* == 0).

2. No WStringBuffer operator is added intentionally as Utf8 is assumed to be the default string encoding format.
----------------------------------------------------------------------------------------------------------------------*/	
class ISerializer
{
public:	
	virtual               ~ISerializer() {}
  
  virtual ISerializer&  operator<<(bool v) = 0;
  virtual ISerializer&  operator<<(I8 v) = 0;
  virtual ISerializer&  operator<<(U8 v) = 0;
  virtual ISerializer&  operator<<(I16 v) = 0;
  virtual ISerializer&  operator<<(U16 v) = 0;
  virtual ISerializer&  operator<<(I32 v) = 0;
  virtual ISerializer&  operator<<(U32 v) = 0;
  virtual ISerializer&  operator<<(I64 v) = 0;
  virtual ISerializer&  operator<<(U64 v) = 0;
  virtual ISerializer&  operator<<(F32 v) = 0;
  virtual ISerializer&  operator<<(D64 v) = 0;
  virtual ISerializer&  operator<<(const StringBuffer& v) = 0;
  virtual ISerializer&  operator<<(const String& v) = 0;
  virtual ISerializer&  operator<<(const char* pStr) = 0;

  virtual void          operator>>(bool& v) = 0;
  virtual void          operator>>(I8& v) = 0;
  virtual void          operator>>(U8& v) = 0;
  virtual void          operator>>(I16& v) = 0;
  virtual void          operator>>(U16& v) = 0;
  virtual void          operator>>(I32& v) = 0;
  virtual void          operator>>(U32& v) = 0;
  virtual void          operator>>(I64& v) = 0;
  virtual void          operator>>(U64& v) = 0;
  virtual void          operator>>(F32& v) = 0;
  virtual void          operator>>(D64& v) = 0;
  virtual void          operator>>(StringBuffer& v) = 0;
  virtual void          operator>>(String& v) = 0;

  virtual void          BeginTag(const StringBuffer& name) = 0;
  virtual void          EndTag() = 0;
};
}
}

/*----------------------------------------------------------------------------------------------------------------------
ISerializer macros

Helper macros to ease the serialization operator definition for custom classes.

inline friend ISerializer& operator<<(ISerializer& target, const class& source) {}

inline friend void operator>>(ISerializer& target, const class& source) {}
----------------------------------------------------------------------------------------------------------------------*/	
#define E_SERIALIZE(target, source, name) \
  target.BeginTag(#name); \
  target << source; \
  target.EndTag()

#define E_DESERIALIZE(source, target, name) \
  source.BeginTag(#name); \
  source >> target; \
  source.EndTag()

#define E_FRIEND_SERIALIZE(name) E_SERIALIZE(target, source.name, name)
#define E_FRIEND_DESERIALIZE(name) E_DESERIALIZE(source, target.name, name)

#endif
