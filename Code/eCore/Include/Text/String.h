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

/** @file StringBuffer.h
This file defines the library text string representation types both for static and dynamic strings. UTF-8 is the chosen
encoding for char based strings. Read below.
*/

#ifndef E3_STRING_H
#define E3_STRING_H

#include "CharArray.h"
#include "CharList.h"
#include "CharArray.h"

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (E_INTERNAL_SETTING_STRING_SIZE)

Please note that this macro has the following usage contract:

1. This macro defines a default string size value for static strings.
2. This is a global library setting macro which can be predefined by the user.
3. This macro value MUST be an natural number greater than 0.
4. This macro is internal and CANNOT be defined outside the library as it is used as a template parameter for the 
default static string class, which is used by other library classes as an argument in some of their methods. You may
redefine it at the beginning of Base.h instead.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef E_INTERNAL_SETTING_STRING_SIZE
#define E_INTERNAL_SETTING_STRING_SIZE 256
#endif

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
StringBuffer types

Please note that these types has the following usage contract: 

1. Default String and WString size can be overridden by defining the E_INTERNAL_SETTING_STRING_SIZE macro.

Note: this class is only used as an argument in inline methods (not defined in a source file). Otherwise the
E_INTERNAL_SETTING_STRING_SIZE would be already defined by the library (being impossible to pre-define it in
an external project).
----------------------------------------------------------------------------------------------------------------------*/
typedef Text::CharArray<char, E_INTERNAL_SETTING_STRING_SIZE> String;
typedef Text::CharArray<wchar_t, E_INTERNAL_SETTING_STRING_SIZE> WString;
typedef Text::CharList<char> StringBuffer;
typedef Text::CharList<wchar_t> WStringBuffer;

/*----------------------------------------------------------------------------------------------------------------------
Text (conversion)

Please note that this functions have the following usage contract:

1. Pointer version functions must ensure pTarget allocation size to host the converted string. For this reason it is 
encouraged to use the CharArray or CharList version instead.
2. Pointer version functions do not null-terminate converted strings.
3. Pointer version conversion functions return the converted length.

Note: The library assumes UTF-8 but Windows uses UTF-16 encoding natively. For this reason the default library project 
configuration is Unicode which makes all Windows API native calls use wchar_t. This way all UTF-16 to UTF-8 
conversions are clearly identified by a wchar_t to char string conversion. The conversion utility functions below 
provide this functionality.

Do not be mislead to using multi-byte encoding to avoid conversions between Windows API calls as UTF-16 to UTF-8 
conversion is still required even if both encoding representations use char as the underlying string character.
----------------------------------------------------------------------------------------------------------------------*/
namespace Text
{
E_API size_t  Utf8ToWide(wchar_t* pTarget, const char* pSource, size_t sourceLength);
E_API void    Utf8ToWide(WStringBuffer& target, const StringBuffer& source);
E_API size_t  WideToUtf8(char* pTarget, const wchar_t* pSource, size_t sourceLength);
E_API void    WideToUtf8(StringBuffer& target, const WStringBuffer& source);

template <size_t Size>
void          Utf8ToWide(CharArray<wchar_t, Size>& target, const CharArray<char, Size>& source) { target.SetLength(Utf8ToWide(&target[0], source.GetPtr(), source.GetLength())); }
template <size_t Size>
void          WideToUtf8(CharArray<char, Size>& target, const CharArray<wchar_t, Size>& source) { target.SetLength(WideToUtf8(&target[0], source.GetPtr(), source.GetLength())); }
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::String)
E_DECLARE_POD(E::WString)

#endif
