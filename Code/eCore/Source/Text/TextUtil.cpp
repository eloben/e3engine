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
------------------------------------------------------------------------------------------------------------------------*/

// Created 02-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file TextUtil.cpp.cpp
This file defines some handy functions for formatted raw string printing.
*/

#include <UtilitiesPch.h>
#include <cstdio>
#include <cwchar>
#include <cstdlib>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Text functions
------------------------------------------------------------------------------------------------------------------------*/

U32 Text::Print(char* pTargetStr, U32 maxLength, const char* pFormattedSourceStr, ...) 
 
  U32 length;
  va_list variableArguments;
  va_start(variableArguments, pFormattedSourceStr);
  length = PrintArguments(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
  va_end(variableArguments);
  return length;
}

U32 Text::PrintArguments(char* pTargetStr, U32 maxLength, const char* pFormattedSourceStr, va_list variableArguments) 
{ 
  return vsnprintf(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
}

U32 Text::Print(wchar_t* pTargetStr, U32 maxLength, const wchar_t* pFormattedSourceStr, ...) 
{ 
  U32 length;
  va_list variableArguments;
  va_start(variableArguments, pFormattedSourceStr);
  length = PrintArguments(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
  va_end(variableArguments);
  return length;
}

U32 Text::PrintArguments(wchar_t* pTargetStr, U32 maxLength, const wchar_t* pFormattedSourceStr, va_list variableArguments) 
{ 
  return vswprintf(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
}
