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

/** @file XmlSerializer::Impl.h
This file declares the XmlSerializer::Impl class. 
This class uses the third party library pugixml (http://pugixml.org/)
*/

#ifndef E3_XML_SERIALIZER_IMPL_H
#define E3_XML_SERIALIZER_IMPL_H

#include <pugixml/pugixml.hpp>

namespace E
{
namespace Serialization
{
/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer::Impl
----------------------------------------------------------------------------------------------------------------------*/
class XmlSerializer::Impl : public ISerializer, public Memory::ProxyAllocated
{
public:	
	Impl();

  // Operators
  ISerializer&        operator<<(bool v);
  ISerializer&        operator<<(I8 v);
  ISerializer&        operator<<(U8 v);
  ISerializer&        operator<<(I16 v);
  ISerializer&        operator<<(U16 v);
  ISerializer&        operator<<(I32 v);
  ISerializer&        operator<<(U32 v);
  ISerializer&        operator<<(I64 v);
  ISerializer&        operator<<(U64 v);
  ISerializer&        operator<<(F32 v);
  ISerializer&        operator<<(D64 v);
  ISerializer&        operator<<(const StringBuffer& v);
  ISerializer&        operator<<(const String& v);
  ISerializer&        operator<<(const char* pTr);

  void                operator>>(bool& v);
  void                operator>>(I8& v);
  void                operator>>(U8& v);
  void                operator>>(I16& v);
  void                operator>>(U16& v);
  void                operator>>(I32& v);
  void                operator>>(U32& v);
  void                operator>>(I64& v);
  void                operator>>(U64& v);
  void                operator>>(F32& v);
  void                operator>>(D64& v);
  void                operator>>(StringBuffer& v);
  void                operator>>(String& v);

  // Methods
  void                BeginTag(const StringBuffer& name);
  void                Clear();
  void                EndTag();
  void                Save(const StringBuffer& fileName);

private:
  static const char*  kValueTag;
  pugi::xml_document  mDocument;
  pugi::xml_node      mCurrentNode;

  E_DISABLE_COPY_AND_ASSSIGNMENT(Impl)
};
}
}

#endif
