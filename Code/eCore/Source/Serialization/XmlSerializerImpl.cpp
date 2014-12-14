/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2012 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 04-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file XmlSerializer::Impl.cpp
This file defines the XmlSerializer::Impl class.
This class uses the third party library pugixml (http://pugixml.org/)
*/

#include <CorePch.h>
#include "XmlSerializerImpl.h"

namespace E
{
namespace Serialization
{
/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer::Impl constants
----------------------------------------------------------------------------------------------------------------------*/

const char*  XmlSerializer::Impl::kValueTag = "value";

/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer::Impl initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

XmlSerializer::Impl::Impl()
  : mCurrentNode(mDocument)
{
}

/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer::Impl operators
----------------------------------------------------------------------------------------------------------------------*/
    
ISerializer& XmlSerializer::Impl::operator<<(bool v)
{
  pugi::xml_attribute attribute = mCurrentNode.append_attribute(kValueTag);
  attribute.set_value(v);
  return *this;
}
    
ISerializer& XmlSerializer::Impl::operator<<(I8 v)
{
  return operator<<(static_cast<I32>(v));
}
    
ISerializer& XmlSerializer::Impl::operator<<(U8 v)
{
  return operator<<(static_cast<U32>(v));
}

  
ISerializer& XmlSerializer::Impl::operator<<(I16 v)
{
  return operator<<(static_cast<I32>(v));
}
    
ISerializer& XmlSerializer::Impl::operator<<(U16 v)
{
  return operator<<(static_cast<U32>(v));
}
    
ISerializer& XmlSerializer::Impl::operator<<(I32 v)
{
  pugi::xml_attribute attribute = mCurrentNode.append_attribute(kValueTag);
  attribute.set_value(v);
  return *this;
}
    
ISerializer& XmlSerializer::Impl::operator<<(U32 v)
{
  pugi::xml_attribute attribute = mCurrentNode.append_attribute(kValueTag);
  attribute.set_value(v);
  return *this;
}
    
ISerializer& XmlSerializer::Impl::operator<<(I64 v)
{
  return operator<<(static_cast<I32>(v));
}
    
ISerializer& XmlSerializer::Impl::operator<<(U64 v)
{
  return operator<<(static_cast<U32>(v));
}
    
ISerializer& XmlSerializer::Impl::operator<<(F32 v)
{
  return operator<<(static_cast<D64>(v));
}
    
ISerializer& XmlSerializer::Impl::operator<<(D64 v)
{
  pugi::xml_attribute attribute = mCurrentNode.append_attribute(kValueTag);
  attribute.set_value(v);
  return *this;
}
    
ISerializer& XmlSerializer::Impl::operator<<(const StringBuffer& v)
{
  return operator<<(v.GetPtr());
}

ISerializer& XmlSerializer::Impl::operator<<(const String& v)
{
  return operator<<(v.GetPtr());
}
    
ISerializer& XmlSerializer::Impl::operator<<(const char* pStr)
{
  pugi::xml_attribute attribute = mCurrentNode.append_attribute(kValueTag);
  attribute.set_value(pStr);
  return *this;
}
    
void XmlSerializer::Impl::operator>>(bool& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_bool();
}
    
void XmlSerializer::Impl::operator>>(I8& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = static_cast<I8>(attribute.as_int());
}
    
void XmlSerializer::Impl::operator>>(U8& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = static_cast<U8>(attribute.as_uint());
}

void XmlSerializer::Impl::operator>>(I16& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = static_cast<I16>(attribute.as_int());
}
    
void XmlSerializer::Impl::operator>>(U16& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = static_cast<U16>(attribute.as_uint());
}
    
void XmlSerializer::Impl::operator>>(I32& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_int();
}
    
void XmlSerializer::Impl::operator>>(U32& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_uint();
}
    
void XmlSerializer::Impl::operator>>(I64& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_int();
}
    
void XmlSerializer::Impl::operator>>(U64& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_uint();
}
    
void XmlSerializer::Impl::operator>>(F32& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_float();
}
    
void XmlSerializer::Impl::operator>>(D64& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_double();
}
    
void XmlSerializer::Impl::operator>>(StringBuffer& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_string();
}

void XmlSerializer::Impl::operator>>(String& v)
{
  pugi::xml_attribute attribute = mCurrentNode.attribute(kValueTag);
  v = attribute.as_string();
}

/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer::Impl methods
----------------------------------------------------------------------------------------------------------------------*/

void XmlSerializer::Impl::BeginTag(const StringBuffer& name)
{
  pugi::xml_node childNode = mCurrentNode.child(name.GetPtr());
  if (childNode == nullptr)
    childNode  = mCurrentNode.append_child(name.GetPtr());

  mCurrentNode  = childNode;
}

void XmlSerializer::Impl::Clear()
{
  mDocument.reset();
  mCurrentNode = mDocument;
}

void XmlSerializer::Impl::EndTag()
{
  mCurrentNode  = mCurrentNode.parent();
}

void XmlSerializer::Impl::Save(const StringBuffer& fileName)
{
  mDocument.save_file(fileName.GetPtr());
}
}
}
