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

/** @file PlatformBase.h
This file defines the library base definitions for the Windows platform.

Note the platform dependencies layer distribution:

  1. Platform.h:      platform specific headers required by platform dependent implementations (includes PlatformTypes).
  2. PlatformTypes.h: platform version of the basic types used by the library (used by Types.h).

  *. PlatformBase.h:  platform version of the library base definitions used by the Utilities library (used by Base.h).
*/

#ifndef E3_PLATFORM_BASE_H
#define E3_PLATFORM_BASE_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <intrin.h>

/*----------------------------------------------------------------------------------------------------------------------
Removed macro definitions

1. min, max conflict with with std::max and std::min.
2. TRUE and FALSE macros may conflicts with other libraries. It is safer to redefine them as C++ constants.
----------------------------------------------------------------------------------------------------------------------*/

#undef min
#undef max
#undef Yield
#undef TRUE
#undef FALSE
#undef GetCurrentTime
#undef GetTickCount

/*----------------------------------------------------------------------------------------------------------------------
Compiler requirements (currently only Microsoft Visual Studio is supported)
----------------------------------------------------------------------------------------------------------------------*/
#if (_MSC_VER < 1600) // Minimum Visual Studio version required (Visual Studio 2010)
  #error "Minimum Visual Studio version not supported."
#endif

/*----------------------------------------------------------------------------------------------------------------------
Disabled warnings
----------------------------------------------------------------------------------------------------------------------*/

#pragma warning(push)
#pragma warning(disable: 4351) // new behavior: elements of array will be default initialized
#pragma warning(disable: 4345) // behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
//#pragma warning(disable: 4127) // conditional expression is constant (temporary removed can hide unwanted infinite loops)

/*----------------------------------------------------------------------------------------------------------------------
Platform detection (32 or 64 bit)
----------------------------------------------------------------------------------------------------------------------*/
#ifdef _M_X64
  #define E_PLATFORM_CPU_X64 1
  #define E_PLATFORM_PTR_SIZE 8
#else 
  #ifdef _M_IX86
    #define E_PLATFORM_CPU_X86 1
    #define E_PLATFORM_PTR_SIZE 4
  #else
    #error "Unrecognized platform!"
  #endif
#endif

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (null pointer)
----------------------------------------------------------------------------------------------------------------------*/

#define E_PLATFORM_FORCE_INLINE __forceinline

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (API export / import)

Please note that this macro has the following usage contract: 

1. The macro defines an API function to be imported unless the E_SETTING_DYNAMIC_LIBRARY macro is defined (which sets
the API function to be exported). Only projects compiled as dynamic libraries should define E_SETTING_DYNAMIC_LIBRARY.
----------------------------------------------------------------------------------------------------------------------*/

#ifdef E_SETTING_DYNAMIC_LIBRARY
  #define E_PLATFORM_API __declspec(dllexport)
#else
  #define E_PLATFORM_API __declspec(dllimport)
#endif

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (debug)
----------------------------------------------------------------------------------------------------------------------*/

#define E_PLATFORM_FILE __FILE__
#define E_PLATFORM_FUNCTION __FUNCTION__
#define E_PLATFORM_LINE __LINE__

#ifdef _DEBUG
  #define E_PLATFORM_DEBUG
  #define E_PLATFORM_DEBUG_OUTPUT OutputDebugStringA
  #define E_PLATFORM_DEBUG_ANALYSIS_ASSUME(condition) __analysis_assume(condition)
  #define E_PLATFORM_DEBUG_BREAK { __debugbreak(); }
  #define E_PLATFORM_DEBUG_BREAK_VERBOSE(file, line, function, expression, message) \
    if (_CrtDbgReport(_CRT_ASSERT, file, line, nullptr, "'%s'\nMessage: \t%s\nFunction: \t%s", expression, message, function) != 0) \
        { E_PLATFORM_DEBUG_BREAK; }
#endif

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
Platform type definitions

Note that minimum _MSC_VER is 1310 (Visual Studio 2003) however the minimum version required by the library is defined 
by the E_MSVC_MIN_VERSION macro defined in PlatformBase.h.
----------------------------------------------------------------------------------------------------------------------*/
typedef __int8            PlatformI8;
typedef unsigned __int8   PlatformU8;

typedef __int16           PlatformI16;
typedef unsigned __int16  PlatformU16;

typedef __int32           PlatformI32;
typedef unsigned __int32  PlatformU32;

typedef __int64           PlatformI64;
typedef unsigned __int64  PlatformU64;
}

/*----------------------------------------------------------------------------------------------------------------------
Constants definitions
----------------------------------------------------------------------------------------------------------------------*/
const E::PlatformI32 FALSE = 0;
const E::PlatformI32 TRUE = 1;

#endif
