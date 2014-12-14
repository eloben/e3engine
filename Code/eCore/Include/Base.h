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

// Created 02-Feb-2015 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Base.h
This file defines the library core macro definitions and includes used by the Utilities library. Base is therefore the
library main header. Note that there are library settings that can be user modified by changing its corresponding macro. 
All library setting macros start by 'E_SETTING_'. You may redefine setting macros before using the affected library 
classes (before including the relevant Utility headers).
*/

#ifndef E3_BASE_H
#define E3_BASE_H

/*----------------------------------------------------------------------------------------------------------------------
Build error messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_BUILD_ERROR_PLATFORM_NOT_SUPPORTED "Platform not supported."
#define E_BUILD_ERROR_COMPILER_NOT_SUPPORTED "Compiler not supported."

#ifndef WIN32
#error E_BUILD_ERROR_PLATFORM_NOT_SUPPORTED
#endif

#ifdef _MSC_VER
#define E_COMPILER_MSVC
#include "Msvc/PlatformBase.h"
#else
#error E_BUILD_ERROR_COMPILER_NOT_SUPPORTED
#endif
#include <memory>

/*----------------------------------------------------------------------------------------------------------------------
Static assertion messages

Note that this macros are intended to be used in combination with static_assert e.g:

  static_assert(PodTypeTraits<KeyType>::value, E_STATIC_ASSERT_MSG_TYPES_IS_POD(KeyType));
----------------------------------------------------------------------------------------------------------------------*/
#define E_STATIC_ASSERT_MSG_TYPES_IS_POD(Type) E_TO_STR(Type)" must be POD"
#define E_STATIC_ASSERT_MSG_TYPES_IS_CHAR_TYPE(Type) E_TO_STR(Type)" must be a valid char type"

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (CPU architecture)
----------------------------------------------------------------------------------------------------------------------*/
#define E_PTR_SIZE              E_PLATFORM_PTR_SIZE

#ifdef E_PLATFORM_CPU_X86
  #define E_CPU_X86             1
#else
  #ifdef E_PLATFORM_CPU_X64
    #define E_CPU_X64           1
  #endif
#endif

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (debug)
----------------------------------------------------------------------------------------------------------------------*/

#define E_FILE                  E_PLATFORM_FILE
#define E_FUNCTION              E_PLATFORM_FUNCTION
#define E_LINE                  E_PLATFORM_LINE

#ifdef E_PLATFORM_DEBUG
#define E_DEBUG
#define E_DEBUG_OUTPUT          E_PLATFORM_DEBUG_OUTPUT
#define E_DEBUG_ANALYSIS_ASSUME E_PLATFORM_DEBUG_ANALYSIS_ASSUME
#define E_DEBUG_BREAK           E_PLATFORM_DEBUG_BREAK
#define E_DEBUG_BREAK_VERBOSE   E_PLATFORM_DEBUG_BREAK_VERBOSE
#endif

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (namespace declaration helpers)

Please note that this macro has the following usage contract: 

1. The macro defines an API function to be imported unless the E_SETTING_DYNAMIC_LIBRARY macro is defined (which sets
the API function to be exported). Only projects compiled as dynamic libraries should define E_SETTING_DYNAMIC_LIBRARY.
----------------------------------------------------------------------------------------------------------------------*/
#define E_BEGIN_NAMESPACE(x)    namespace x {
#define E_END_NAMESPACE(x)      }

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (method declaration helpers)

Please note that this macro has the following usage contract: 

1. The macro defines an API function to be imported unless the E_SETTING_DYNAMIC_LIBRARY macro is defined (which sets
the API function to be exported). Only projects compiled as dynamic libraries should define E_SETTING_DYNAMIC_LIBRARY.
----------------------------------------------------------------------------------------------------------------------*/
#define E_FORCE_INLINE          E_PLATFORM_FORCE_INLINE
#define E_API                   E_PLATFORM_API

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (class declaration helpers)

Please note that this macro has the following usage contract: 

1. E_DISABLE_COPY_AND_ASSSIGNMENT macro allows disabling copy for a concrete <class> type.
2. Classes implementing E_DISABLE_COPY_AND_ASSSIGNMENT macro MUST specify a default constructor.
3. E_DECLARE_POD must be used inside the GLOBAL namespace. Remember that according to C++ Standard 14.7.3/2: an explicit 
specialization shall be declared in the namespace of which the template is a member, or, for member templates, in the 
namespace of which the enclosing class or enclosing class template is a member.
----------------------------------------------------------------------------------------------------------------------*/

#define E_DECLARE_POD(class_name) namespace E { template <> struct PodTypeTraits<class_name> { static const bool value = true; }; }
#define E_DISABLE_COPY_AND_ASSSIGNMENT(class_name) \
  private: \
  class_name(const class_name&) = delete; \
  class_name& operator=(const class_name&) = delete; \
  class_name(class_name &&) = delete;

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (type definition helpers)

Please note that this macro has the following usage contract: 

1. E_PIMPL declares a private implementation class named Impl through a const std::auto_ptr idiom. Use this idiom to 
avoid cyclic dependencies in header files. The const aut_ptr idiom allows automatic memory release of contained object 
at destruction and never looses ownership. Note that using this macro although you need to create the hosted type 
instance through the operator new (in the constructor), you do NOT have to call delete. However remember that a 
destructor MUST BE DEFINED.
----------------------------------------------------------------------------------------------------------------------*/
#define E_PIMPL class Impl; const std::unique_ptr<Impl>

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (method helpers)
----------------------------------------------------------------------------------------------------------------------*/
#define E_ELEMENT_COUNT(x)  (sizeof(x) / sizeof(x[0]))
#define E_TO_STR(x) #x

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
Type definitions

Note about char type: char is the smallest type (1 byte) by definition, according to the C and C++ standards (C99, 
section 6.5.3.4).

Note that most platforms follow the IEEE standard for floating-point values, so floats will be 32 bits in size and 
doubles will be 64 bytes. If we ever support a platform that uses a weird floating-point representation, we would 
have to redefine F32 and D64 for that particular platform.
----------------------------------------------------------------------------------------------------------------------*/
typedef void*         Ptr;
typedef PlatformI8    I8;
typedef PlatformU8    U8;
typedef U8            Byte;
typedef PlatformI16   I16;
typedef PlatformU16   U16;
typedef PlatformI32   I32;
typedef PlatformU32   U32;
typedef PlatformI64   I64;
typedef PlatformU64   U64;
typedef float         F32;
typedef double        D64;

/*----------------------------------------------------------------------------------------------------------------------
PodTypeTraits

Please note that this class has the following usage contract: 

1. Types declared as Pod must specialize this template. The E_DECLARE_POD macro may be used for this purpose.
2. The type definition Type allows checking if a concrete type is POD during compile time. E.g. 
    
    return static_cast<PodTypeTraits<PodType>::Type*>(Copy(pTarget, pSource, sizeof(PodType) * count));

3. The static method IsPod allows checking if a concrete type is POD in runtime. E.g.

    E_ASSERT(PodTypeTraits<PodType>::value);
    return static_cast<PodType*>(Copy(pTarget, pSource, sizeof(PodType) * count));

4. A POD class should have no user-defined copy-assignment operator and destructor and none of its non-static 
members should be a non-POD class, array of non-POD, or a reference. In C++ 11 the requirements also requires the 
class to not having a user-defined default constructor either (or using e.g. MyClass() = default). The important
thing to bear in mind is to ensure that the POD candidate class does not have a default constructor and assignment 
operator that does something in addition to plainly copy the data for it to be compatible with plain memory copy
functions such as memcpy: e.g. a default constructor with just an initialization list to default its member values
although user-defined (C++ 11) respects the intrinsic POD type memory copy requirements. Just use the template 
wisely.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T> struct PodTypeTraits            { static const bool value = false; };
template <typename T> struct PodTypeTraits<T*>        { static const bool value = true; };
template <>           struct PodTypeTraits<bool>      { static const bool value = true; };
template <>           struct PodTypeTraits<wchar_t>   { static const bool value = true; };
template <>           struct PodTypeTraits<I8>        { static const bool value = true; };
template <>           struct PodTypeTraits<U8>        { static const bool value = true; };
template <>           struct PodTypeTraits<I16>       { static const bool value = true; };
template <>           struct PodTypeTraits<U16>       { static const bool value = true; };
template <>           struct PodTypeTraits<I32>       { static const bool value = true; };
template <>           struct PodTypeTraits<U32>       { static const bool value = true; };
template <>           struct PodTypeTraits<I64>       { static const bool value = true; };
template <>           struct PodTypeTraits<U64>       { static const bool value = true; };
template <>           struct PodTypeTraits<F32>       { static const bool value = true; };
template <>           struct PodTypeTraits<D64>       { static const bool value = true; };

/*----------------------------------------------------------------------------------------------------------------------
CharTypeTraits
----------------------------------------------------------------------------------------------------------------------*/
template <typename T> struct CharTypeTraits           { static const bool value = false; };
template <>           struct CharTypeTraits<char>     { static const bool value = true; };
template <>           struct CharTypeTraits<wchar_t>  { static const bool value = true; };
}

#endif
