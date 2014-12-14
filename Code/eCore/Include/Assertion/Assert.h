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

// Created 01-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Assert.h
This file declares the assertion function as well as helpful assertion macros. Note that exceptions and assertions are 
the same: they point out invalid conditions which should not happen. However use Exception when communicating to an 
external third-party user library / application and E_ASSERT otherwise.
*/
#ifndef E3_ASSERT_H
#define E3_ASSERT_H

#include <Base.h>
#include <Text/Text.h>
#include <cstdarg>
#include <iostream>

/*----------------------------------------------------------------------------------------------------------------------
Default assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_TODO "This feature is missing!"

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (assertion)

Please note that this macro has the following usage contract: 

1. Types must be considered when defining a comparison e.g:

   float value = 1.1f;
   E_ASSERT(value == 1.1f); // Correct
   E_ASSERT(value == 1.1);  // Incorrect

   Please note that this macro has the following usage contract: 

Note: E_ASSERT_HANDLE_FAILURE macro uses a parenthesis surrounding the condition: if ((condition) == false). Otherwise composed 
conditions could not be asserted correctly e.g. E_PLATFORM_ASSERT_CONDITION(a == 1 || b == 0, Foo, "").

Note 2: Debug macros depend on __VA_ARGS__ which cannot be redefined to another macro (e.g. E_ARGS)
----------------------------------------------------------------------------------------------------------------------*/

#ifdef E_DEBUG

  /*----------------------------------------------------------------------------------------------------------------------
  Debug message header macros- assertion messages
  ----------------------------------------------------------------------------------------------------------------------*/
  #define E_ASSERT_MSG_HEADER_FAILED  "[Debug Assertion Failed] "
  #define E_ASSERT_MSG_HEADER_MESSAGE "[Debug Info] "
  #define E_ASSERT_MSG_HEADER_WARNING "[Debug Warning] "

  /*----------------------------------------------------------------------------------------------------------------------
  E_ASSERT_ALWAYS

  Assertion macro to always generate a verbose debug break.
  ----------------------------------------------------------------------------------------------------------------------*/
  #define E_ASSERT_ALWAYS(failureMessage, ...) E::Assertion::HandleFailure(E_FILE, E_LINE, E_FUNCTION, "", failureMessage, __VA_ARGS__)
  
  /*----------------------------------------------------------------------------------------------------------------------
  E_ASSERT_HANDLE_FAILURE

  Main assertion macro to generate a verbose debug break when a certain condition is not met.
  ----------------------------------------------------------------------------------------------------------------------*/
  #define E_ASSERT_HANDLE_FAILURE(condition, conditionFailureHandleFunction, conditionFailureMessage, ...) \
    E_DEBUG_ANALYSIS_ASSUME(condition); \
    if ((condition) == false) conditionFailureHandleFunction(E_FILE, E_LINE, E_FUNCTION, #condition, conditionFailureMessage, __VA_ARGS__)

  /*----------------------------------------------------------------------------------------------------------------------
  E_ASSERT_HANDLE_FAILURE specializations
  ----------------------------------------------------------------------------------------------------------------------*/
  #define E_ASSERT(condition) E_ASSERT_HANDLE_FAILURE(condition, E::Assertion::HandleFailure, "", __VA_ARGS__)
  #define E_ASSERT_MSG(condition, failureMessage, ...) E_ASSERT_HANDLE_FAILURE(condition, E::Assertion::HandleFailure, failureMessage, __VA_ARGS__)
  #define E_ASSERT_NULL(ptr) E_ASSERT_HANDLE_FAILURE(ptr == nullptr, E::Assertion::HandleFailure, "Pointer must be nullptr", __VA_ARGS__)
  #define E_ASSERT_PTR(ptr) E_ASSERT_HANDLE_FAILURE(ptr != nullptr, E::Assertion::HandleFailure, "Pointer cannot be nullptr", __VA_ARGS__)
  #define E_ASSERT_TODO E_ASSERT_ALWAYS(E_ASSERT_MSG_TODO)

  /*----------------------------------------------------------------------------------------------------------------------
  E_DEBUG_MSG

  Helper macro to debug output info messages
  ----------------------------------------------------------------------------------------------------------------------*/
  #define E_DEBUG_MSG(infoMessage, ...) E::Assertion::HandleMessage(E_ASSERT_MSG_HEADER_MESSAGE, infoMessage, __VA_ARGS__)

  /*----------------------------------------------------------------------------------------------------------------------
  E_WARNING / E_WARNING_IF

  Helper macros to debug output warning and warning conditional messages
  ----------------------------------------------------------------------------------------------------------------------*/
  #define E_WARNING(warningMessage, ...) E::Assertion::HandleMessage(E_ASSERT_MSG_HEADER_WARNING, warningMessage, __VA_ARGS__)
  #define E_WARNING_IF(condition, warningMessage, ...) if (condition)  E::Assertion::HandleWarning(E_ASSERT_MSG_HEADER_WARNING, warningMessage, __VA_ARGS__)

#else
  #define E_ASSERT_ALWAYS(failureMessage, ...)
  #define E_ASSERT_HANDLE_FAILURE(condition, conditionFailureHandleFunction, conditionFailureMessage, ...)
  #define E_ASSERT(condition)
  #define E_ASSERT_MSG(condition, failureMessage, ...)
  #define E_ASSERT_NULL(ptr)
  #define E_ASSERT_PTR(ptr)
  #define E_ASSERT_TODO
  #define E_DEBUG_MSG
  #define E_WARNING(warningMessage, ...)
  #define E_WARNING_IF(condition, warningMessage, ...)
#endif

namespace E 
{
/*----------------------------------------------------------------------------------------------------------------------
Assertion

Please note that this function has the following usage contract: 

1. The handle failure function needs to be compatible with the E_ASSERT_HANDLE_FAILURE macro:
  - First parameter must be the source file (const char*).
  - Second parameter must be the line (U32).
  - Third parameter must be the function (const char*).
  - Fourth parameter must be the condition expression (const char*).
  - Fifth parameter must be the failure message (const char*).
  - Further parameters can be added as part as a formatted failure message.

----------------------------------------------------------------------------------------------------------------------*/
namespace Assertion
{
  E_API void HandleFailure(const char* pSourceFileStr, U32 line, const char* pFunctionStr, const char* pConditionExpressionStr, const char* pFailureMessageStr, ...);
  E_API void HandleMessage(const char* pMessageHeaderStr, const char* pMessageStr, ...);
}

/*----------------------------------------------------------------------------------------------------------------------
Assertion methods

Please note that this function has the following usage contract: 

1. This default handle failure method uses the E_DEBUG_BREAK_MESSAGE macro whose implementation is platform dependent.
An alternative could be using the C++ standard error output e.g:

  std::cerr << "Debug Assertion Failed!" 
  << "\nFile: " << pSourceFileStr 
  << "\nLine: " << line
  << "\nFunction: " << pFunctionStr
  << "\nExpression: " << formattedConditionFailureMessageStr;    
  std::abort();
----------------------------------------------------------------------------------------------------------------------*/	
#ifdef E_DEBUG
inline void Assertion::HandleFailure(const char* pSourceFileStr, U32 line, const char* pFunctionStr, const char* pConditionExpressionStr, const char* pConditionFailureMessageStr, ...)
{
  va_list variableArguments;
  va_start(variableArguments, pConditionFailureMessageStr);
  char formattedConditionFailureMessageStr[512];
  Text::PrintArguments(&formattedConditionFailureMessageStr[0], 512 - 1, pConditionFailureMessageStr, variableArguments);
  va_end(variableArguments);
  E_DEBUG_OUTPUT(E_ASSERT_MSG_HEADER_FAILED);
  E_DEBUG_OUTPUT(formattedConditionFailureMessageStr);
  E_DEBUG_OUTPUT(".\n");
  E_DEBUG_BREAK_VERBOSE(pSourceFileStr, line, pFunctionStr, pConditionExpressionStr, formattedConditionFailureMessageStr);
}
inline void Assertion::HandleMessage(const char* pMessageHeaderStr, const char* pMessageStr, ...)
{
  va_list variableArguments;
  va_start(variableArguments, pMessageStr);
  char formattedMessageStr[512];
  Text::PrintArguments(&formattedMessageStr[0], 512 - 1, pMessageStr, variableArguments);
  va_end(variableArguments);
  E_DEBUG_OUTPUT(pMessageHeaderStr);
  E_DEBUG_OUTPUT(formattedMessageStr);
  E_DEBUG_OUTPUT(".\n");
}
#endif
}
#endif
