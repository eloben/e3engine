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

/** @file XmlSerializer.cpp
This file defines the XmlSerializer class.
*/

#include <CorePch.h>
#include "XmlSerializerImpl.h"

namespace E
{
namespace Serialization
{
/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

XmlSerializer::XmlSerializer()
  : mpImpl(new Impl())
{
}

XmlSerializer::~XmlSerializer()
{
}

/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer operators
----------------------------------------------------------------------------------------------------------------------*/
    
ISerializer& XmlSerializer::operator<<(bool v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(I8 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(U8 v)
{
  return mpImpl->operator<<(v);
}
  
ISerializer& XmlSerializer::operator<<(I16 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(U16 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(I32 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(U32 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(I64 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(U64 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(F32 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(D64 v)
{
  return mpImpl->operator<<(v);
}
    
ISerializer& XmlSerializer::operator<<(const StringBuffer& v)
{
  return mpImpl->operator<<(v);
}

ISerializer& XmlSerializer::operator<<(const String& v)
{
  return mpImpl->operator<<(v);
}

ISerializer& XmlSerializer::operator<<(const char* pStr)
{
  return mpImpl->operator<<(pStr);
}
    
void XmlSerializer::operator>>(bool& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(I8& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(U8& v)
{
  mpImpl->operator>>(v);
}

void XmlSerializer::operator>>(I16& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(U16& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(I32& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(U32& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(I64& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(U64& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(F32& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(D64& v)
{
  mpImpl->operator>>(v);
}
    
void XmlSerializer::operator>>(StringBuffer& v)
{
  mpImpl->operator>>(v);
}

void XmlSerializer::operator>>(String& v)
{
  mpImpl->operator>>(v);
}

/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer methods
----------------------------------------------------------------------------------------------------------------------*/

void XmlSerializer::BeginTag(const StringBuffer& name)
{
  mpImpl->BeginTag(name);
}

void XmlSerializer::Clear()
{
  mpImpl->Clear();
}

void XmlSerializer::EndTag()
{
  mpImpl->EndTag();
}

void XmlSerializer::Save(const StringBuffer& fileName)
{
  mpImpl->Save(fileName);
}
}
}
