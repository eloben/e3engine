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

/** @file StringBuffer.cpp
This file defines Utf8 string conversion functions.
The Utf8 string conversion functions use the third party library pugixml (http://pugixml.org/)
*/

#include <CorePch.h>
#include "StringImpl.h"

namespace E
{
namespace Text
{
/*----------------------------------------------------------------------------------------------------------------------
Text conversion functions
----------------------------------------------------------------------------------------------------------------------*/

size_t Utf8ToWide(wchar_t* pTarget, const char* pSource, size_t sourceLength)
{
  return Impl::Utf8ToWide(pTarget, pSource, sourceLength);
}

void Utf8ToWide(WStringBuffer& target, const StringBuffer& source)
{
  Impl::Utf8ToWide(target, source);
}

size_t WideToUtf8(char* pTarget, const wchar_t* pSource, size_t sourceLength)
{
  return Impl::WideToUtf8(pTarget, pSource, sourceLength);
}

void WideToUtf8(StringBuffer& target, const WStringBuffer& source)
{
  Impl::WideToUtf8(target, source);
}
}
}
