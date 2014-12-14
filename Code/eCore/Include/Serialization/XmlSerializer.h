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

// Created 04-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file XmlSerializer.h
This file declares the XmlSerializer class.
*/

#ifndef E3_XML_SERIALIZER_H
#define E3_XML_SERIALIZER_H

#include "ISerializer.h"
#include <Base.h>

namespace E
{
namespace Serialization
{
/*----------------------------------------------------------------------------------------------------------------------
XmlSerializer
----------------------------------------------------------------------------------------------------------------------*/
class XmlSerializer : public ISerializer
{
public:	
  E_API XmlSerializer();
  E_API ~XmlSerializer();

  // Operators
  E_API ISerializer&  operator<<(bool v);
  E_API ISerializer&  operator<<(I8 v);
  E_API ISerializer&  operator<<(U8 v);
  E_API ISerializer&  operator<<(I16 v);
  E_API ISerializer&  operator<<(U16 v);
  E_API ISerializer&  operator<<(I32 v);
  E_API ISerializer&  operator<<(U32 v);
  E_API ISerializer&  operator<<(I64 v);
  E_API ISerializer&  operator<<(U64 v);
  E_API ISerializer&  operator<<(F32 v);
  E_API ISerializer&  operator<<(D64 v);
  E_API ISerializer&  operator<<(const StringBuffer& v);
  E_API ISerializer&  operator<<(const String& v);
  E_API ISerializer&  operator<<(const char* pTr);

  E_API void          operator>>(bool& v);
  E_API void          operator>>(I8& v);
  E_API void          operator>>(U8& v);
  E_API void          operator>>(I16& v);
  E_API void          operator>>(U16& v);
  E_API void          operator>>(I32& v);
  E_API void          operator>>(U32& v);
  E_API void          operator>>(I64& v);
  E_API void          operator>>(U64& v);
  E_API void          operator>>(F32& v);
  E_API void          operator>>(D64& v);
  E_API void          operator>>(StringBuffer& v);
  E_API void          operator>>(String& v);

  // Methods
  E_API void          BeginTag(const StringBuffer& name);
  E_API void          Clear();
  E_API void          EndTag();
  E_API void          Save(const StringBuffer& fileName);

private:
  E_PIMPL mpImpl;
  E_DISABLE_COPY_AND_ASSSIGNMENT(XmlSerializer)
};
}
}

#endif
