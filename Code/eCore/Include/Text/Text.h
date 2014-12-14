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

// Created 02-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Text.h
This file declares some handy functions for formatted raw string printing.
*/

#ifndef E3_TEXT_H
#define E3_TEXT_H

#include <Base.h>
#include <cstdarg>

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
Text API functions
----------------------------------------------------------------------------------------------------------------------*/
namespace Text
{
E_API U32   Print(char* pTargetStr, U32 maxLength, const char* pFormattedSourceStr, ...);
E_API U32   Print(wchar_t* pTargetStr, U32 maxLength, const wchar_t* pFormattedSourceStr, ...);
E_API U32   PrintArguments(char* pTargetStr, U32 maxLength, const char* pFormattedSourceStr, va_list variableArguments);
E_API U32   PrintArguments(wchar_t* pTargetStr, U32 maxLength, const wchar_t* pFormattedSourceStr, va_list variableArguments);

template <typename CharType>
U32         GetLength(const CharType* pStr);

inline U32  Print(char* pTargetStr, U32 maxLength, bool b)      { return b ? Print(pTargetStr, maxLength, "true") : Print(pTargetStr, maxLength, "false"); }
inline U32  Print(char* pTargetStr, U32 maxLength, I32 v)       { return Print(pTargetStr, maxLength, "%d", v); }
inline U32  Print(char* pTargetStr, U32 maxLength, U32 v)       { return Print(pTargetStr, maxLength, "%u", v); }
inline U32  Print(char* pTargetStr, U32 maxLength, I64 v)       { return Print(pTargetStr, maxLength, "%lld", v); }
inline U32  Print(char* pTargetStr, U32 maxLength, U64 v)       { return Print(pTargetStr, maxLength, "%llu", v); }
inline U32  Print(char* pTargetStr, U32 maxLength, F32 v)       { return Print(pTargetStr, maxLength, "%.4f", v);; }
inline U32  Print(char* pTargetStr, U32 maxLength, D64 v)       { return Print(pTargetStr, maxLength, "%.8lf", v); }
inline U32  Print(wchar_t* pTargetWstr, U32 maxLength, bool b)  { return b ? Print(pTargetWstr, maxLength, L"true") : Print(pTargetWstr, maxLength, L"false"); }
inline U32  Print(wchar_t* pTargetWstr, U32 maxLength, I32 v)   { return Print(pTargetWstr, maxLength, L"%d", v); }
inline U32  Print(wchar_t* pTargetWstr, U32 maxLength, U32 v)   { return Print(pTargetWstr, maxLength, L"%u", v); }
inline U32  Print(wchar_t* pTargetWstr, U32 maxLength, I64 v)   { return Print(pTargetWstr, maxLength, L"%lld", v);  }
inline U32  Print(wchar_t* pTargetWstr, U32 maxLength, U64 v)   { return Print(pTargetWstr, maxLength, L"%llu", v); }
inline U32  Print(wchar_t* pTargetWstr, U32 maxLength, F32 v)   { return Print(pTargetWstr, maxLength, L"%.5f", v);; }
inline U32  Print(wchar_t* pTargetWstr, U32 maxLength, D64 v)   { return Print(pTargetWstr, maxLength, L"%.7lf", v); }

/*----------------------------------------------------------------------------------------------------------------------
Text methods
----------------------------------------------------------------------------------------------------------------------*/

inline U32 Print(char* pTargetStr, U32 maxLength, const char* pFormattedSourceStr, ...) 
{ 
  U32 length;
  va_list variableArguments;
  va_start(variableArguments, pFormattedSourceStr);
  length = PrintArguments(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
  va_end(variableArguments);
  return length;
}

inline U32 PrintArguments(char* pTargetStr, U32 maxLength, const char* pFormattedSourceStr, va_list variableArguments) 
{ 
  return vsnprintf(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
}

inline U32 Print(wchar_t* pTargetStr, U32 maxLength, const wchar_t* pFormattedSourceStr, ...) 
{ 
  U32 length;
  va_list variableArguments;
  va_start(variableArguments, pFormattedSourceStr);
  length = PrintArguments(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
  va_end(variableArguments);
  return length;
}

inline U32 PrintArguments(wchar_t* pTargetStr, U32 maxLength, const wchar_t* pFormattedSourceStr, va_list variableArguments) 
{ 
  return vswprintf(pTargetStr, maxLength, pFormattedSourceStr, variableArguments);
}

/*----------------------------------------------------------------------------------------------------------------------
Text::GetLength

Note: CharTypeTraits<CharType>::Type is not used in the argument list to allow template deduction without explicit call.

 e.g. forcing the user to call the following is avoided:

  Text::GetLength<MyCharType>(pStr);

However CharTypeTraits<CharType>::Type is used inside the function to do the type check during compilation.
----------------------------------------------------------------------------------------------------------------------*/
template <typename CharType>
inline U32 GetLength<CharType>(const CharType* pStr)
{
  static_assert(CharTypeTraits<CharType>::value, E_STATIC_ASSERT_MSG_TYPES_IS_CHAR_TYPE(CharType));
  const CharType* pEos = pStr;
  while (*pEos++);
  return static_cast<U32>(pEos - pStr - 1);
}
}
}

#endif
