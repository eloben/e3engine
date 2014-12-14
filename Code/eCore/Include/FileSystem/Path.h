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

// Created 31-Aug-2014 by Elías Lozada-Benavente
// Based on original created 12-Feb-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Path.h
This file defines the library file system path types.
*/

#ifndef E3_PATH_H
#define E3_PATH_H

#include <Text/String.h>

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (E_INTERNAL_SETTING_PATH_SIZE)

Please note that this macro has the following usage contract:

1. This macro defines a default string size value for file paths.
2. This is a global library setting macro which can be predefined by the user.
3. This macro value MUST be an natural number greater than 0.
4. This macro is internal and CANNOT be defined outside the library as it is used as a template parameter for the 
default path representation, which may be used by other library classes as an argument in some of their methods. You may
redefine it at the beginning of Base.h instead.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef E_INTERNAL_SETTING_PATH_SIZE
#define E_INTERNAL_SETTING_PATH_SIZE 320
#endif

namespace E
{
namespace FileSystem
{
 /*----------------------------------------------------------------------------------------------------------------------
 Path

 Please note that these types has the following usage contract: 

 1. Default Path and WPath size can be overridden by defining the E_INTERNAL_SETTING_PATH_SIZE macro.
 
 Note: although Path / WPath are based on CharArray as well as String / WString it is recommended to use Path / WPath
 when relating to file system paths for semantic reasons. On the other CharArray allows you to work with different
 sizes in a transparent manner.
 ----------------------------------------------------------------------------------------------------------------------*/
typedef E::Text::CharArray<char, E_INTERNAL_SETTING_PATH_SIZE> Path;
typedef E::Text::CharArray<wchar_t, E_INTERNAL_SETTING_PATH_SIZE> WPath;
}

/*----------------------------------------------------------------------------------------------------------------------
File types
----------------------------------------------------------------------------------------------------------------------*/
typedef FileSystem::Path FilePath;
typedef FileSystem::WPath WFilePath;
}

#endif
