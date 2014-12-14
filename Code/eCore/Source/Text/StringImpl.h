/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

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

// Created 29-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file StringImpl.h
This file defines Utf8 string conversion functions.
The Utf8 string conversion functions use the third party library pugixml (http://pugixml.org/)
*/

#ifndef E3_STRING_IMPL_H
#define E3_STRING_IMPL_H

#include <CorePch.h>
#include <pugixml/pugixml.hpp>

namespace E
{
namespace Text
{
/*----------------------------------------------------------------------------------------------------------------------
Text::Impl (StringBuffer)

Please note that this class has the following usage contract: 

1. Pointer version functions must ensure pTarget allocation size to host the converted string. For this reason it is 
encouraged to use the CharArray or CharList version instead.
2. Pointer version functions do not null-terminate converted strings.
3. Pointer version conversion functions return the converted length.
----------------------------------------------------------------------------------------------------------------------*/
namespace Impl
{
size_t  Utf8ToWide(wchar_t* pTarget, const char* pSource, size_t sourceLength);
void    Utf8ToWide(WStringBuffer& target, const StringBuffer& source);
size_t  WideToUtf8(char* pTarget, const wchar_t* pSource, size_t sourceLength);
void    WideToUtf8(StringBuffer& target, const WStringBuffer& source);
}
}

/*----------------------------------------------------------------------------------------------------------------------
Text::Impl methods
----------------------------------------------------------------------------------------------------------------------*/

inline size_t Text::Impl::Utf8ToWide(wchar_t* pTarget, const char* pSource, size_t sourceLength)
{
  size_t targetLength = pugi::wide_size(pSource, sourceLength);
  pugi::as_wide(pTarget, targetLength, pSource, sourceLength);
  return targetLength;
}

inline void Text::Impl::Utf8ToWide(WStringBuffer& target, const StringBuffer& source)
{
  size_t targetLength = pugi::wide_size(source.GetPtr(), source.GetLength());
  target.Reserve(targetLength + 1);
  pugi::as_wide(&target[0], targetLength, source.GetPtr(), source.GetLength());
  target.SetLength(targetLength);
}

inline size_t Text::Impl::WideToUtf8(char* pTarget, const wchar_t* pSource, size_t sourceLength)
{
  size_t targetLength = pugi::utf8_size(pSource, sourceLength);
  pugi::as_utf8(pTarget, targetLength, pSource, sourceLength);
  return targetLength;
}

inline void Text::Impl::WideToUtf8(StringBuffer& target, const WStringBuffer& source)
{
  size_t targetLength = pugi::utf8_size(source.GetPtr(), source.GetLength());
  target.Reserve(targetLength + 1);
  pugi::as_utf8(&target[0], targetLength, source.GetPtr(), source.GetLength());
  target.SetLength(targetLength);
}
}

#endif
