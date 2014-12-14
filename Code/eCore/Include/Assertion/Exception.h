/*-----/*----------------------------------------------------------------------------------------------------------------------
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

// Created 26-Jan-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Exception.h
This file defines a very simple exception struct as well as useful helper functions. Exception contains a type 
identifier and a textual description. The type identifier can be used to group exceptions relating them to a specific 
subsystem. Note that exceptions and assertions are the same: they point out invalid conditions which should not happen.
However use Exception when communicating to an external third-party user library / application and E_ASSERT otherwise.
*/

#ifndef E_EXCEPTION_H
#define E_EXCEPTION_H

#include <Text/String.h>

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
Exception

Please note that this class has the following usage contract:

1. Exception does NOT allocate heap memory.
2. The description field is optional and for internally purposes only: use type to index proper end user messages.
3. Exception allows tagging exception types using the extensible enum type Type. In order to extend it 
adding custom Tag types the setting macro E_SETTING_CUSTOM_EXCEPTION_TYPES must be defined. 
E_SETTING_CUSTOM_EXCEPTION_TYPES must define the additional Type types desired. For this purpose the helper
macro E_EXCEPTION_TYPE is supplied e.g:

#define E_SETTING_CUSTOM_EXCEPTION_TYPES \
E_EXCEPTION_TYPE(CustomNew) \
E_EXCEPTION_TYPE(CustomDelete)

4. A default Type is included for general purpose however it is not used within the library.
5. An Type type counter eExceptionTypeCount is included.
----------------------------------------------------------------------------------------------------------------------*/
struct Exception
{
  enum Type
  {
    #define E_EXCEPTION_TYPE(name) eExceptionType##name,
    E_EXCEPTION_TYPE(Default)
    #ifdef E_SETTING_CUSTOM_EXCEPTION_TYPES
    E_SETTING_CUSTOM_EXCEPTION_TYPES
    #endif
    E_EXCEPTION_TYPE(Count)
    #undef E_EXCEPTION_TYPE
  };

  Type          type;
  String  description;

  explicit Exception(Type type)
    : type(type)
	{}

  Exception(const String& description) 
    : type(eExceptionTypeDefault)
    , description(description)
  {}

  Exception(Type type, const String& description) 
    : type(type)
    , description(description)
  {}
};

/*----------------------------------------------------------------------------------------------------------------------
Exception helper functions
----------------------------------------------------------------------------------------------------------------------*/

inline void ThrowIf(bool condition, Exception::Type type)
{
  if (condition)
  {
    throw Exception(type);
  }
}

inline void ThrowIfFailed(I32 result, Exception::Type type)
{
  ThrowIf(result < 0, type);
}

inline void ThrowIfFalse(bool result, Exception::Type type)
{
  ThrowIf(!result, type);
}

inline bool ThrowIfNull(void* ptr, Exception::Type type)
{
  ThrowIf(ptr == nullptr, type);
}

inline void ThrowIfZero(I32 result, Exception::Type type)
{
  ThrowIf(result == 0, type);
}
}

#endif
