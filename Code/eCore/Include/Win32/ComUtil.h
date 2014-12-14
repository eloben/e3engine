/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Engine

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

// Created 09-Feb-2010 by Elías Lozada-Benavente
// based on original by Ángel Riesgo
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ComUtil.h
This file defines Windows COM utility classes and functions. ComUtil depends directly on Platform.h. You may use this
header instead of Platform whenever you require Windows COM support.
*/

#ifndef E_WIN_COM_UTIL_H
#define E_WIN_COM_UTIL_H

#include <Singleton.h>
#include <Assertion/Assert.h>
#include <objbase.h>

/*----------------------------------------------------------------------------------------------------------------------
Com assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_COM_INITIALIZATION_RESULT_VALUE  "Unexpected Com initialization result value"
#define E_ASSERT_MSG_COM_FINALIZATION_THREAD          "Com finalization must be performed by the initialization thread"

namespace E
{
namespace Win32
{
/*------------------------------------------------------------------------------------------------------------------
Win32::Global

Please note that these global methods are inlined as COM initialization in order to allow using their functionality
in third party DLLs or executables.
------------------------------------------------------------------------------------------------------------------*/
namespace Global
{
  inline void  InitializeCom();
  inline bool  IsComInitialized();
}

/*------------------------------------------------------------------------------------------------------------------
ComValidityScope

There are several parts of a system that may use COM. Ensure to make COM available in every thread created to ensure that
COM calls will not fail when run in a working thread. ComValidityScope implements the RAII idiom ("resource acquisition 
is initialization").

Please note that this class has the following usage contract: 

1. Calls to both the constructor and the destructor must take place in the same thread.
2. COM initialization in constructor must return a proper result value (it will assert otherwise).
------------------------------------------------------------------------------------------------------------------*/
class ComValidityScope
{
public:
  ComValidityScope();
  ~ComValidityScope();

private:
  DWORD mThreadId;
  bool mSucceeded;

  E_DISABLE_COPY_AND_ASSSIGNMENT(ComValidityScope)
};

/*------------------------------------------------------------------------------------------------------------------
Global

Please note that these methods have the following usage contract: 

1. Singleton<ComValidityScope> is used to initialize COM in the main application thread. By calling InitializeCom 
before the initialization of any singleton that may rely on COM-based classes, we can ensure that the COM system 
outlives any other singletons whose destructors may use COM-based libraries.

Note that a ComValidityScope local variable in the WinMain function does not cover the destruction of singletons after 
WinMain returns. That is why a call to InitializeCom is usually necessary.
------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------
Win
------------------------------------------------------------------------------------------------------------------*/
template <class T> 
void Release(T*& pComObjectRef);

/*------------------------------------------------------------------------------------------------------------------
ComValidityScope initialization & finalization
------------------------------------------------------------------------------------------------------------------*/	

inline ComValidityScope::ComValidityScope()
  : mThreadId(0)
  , mSucceeded(false)
{
  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  E_ASSERT_MSG(hr == S_OK || hr == S_FALSE || hr == RPC_E_CHANGED_MODE, E_ASSERT_MSG_COM_INITIALIZATION_RESULT_VALUE);
  mSucceeded = SUCCEEDED(hr);
  mThreadId = GetCurrentThreadId();
}

inline ComValidityScope::~ComValidityScope()
{
  bool sameThread = (mThreadId == GetCurrentThreadId());
  E_ASSERT_MSG(sameThread, E_ASSERT_MSG_COM_FINALIZATION_THREAD);
  if(mSucceeded && sameThread)
  {
    CoUninitialize();
  }
}

/*------------------------------------------------------------------------------------------------------------------
Win::Global methods

Auxiliary function that queries whether COM has already been initialized. There is no function to perform this kind of
query, so we have to do it indirectly by attempting to initialize COM and checking the return value.
------------------------------------------------------------------------------------------------------------------*/	
inline void Global::InitializeCom() { Singleton<ComValidityScope>::GetInstance(); }

inline bool Global::IsComInitialized()
{
  bool initialized = false;

  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (hr == S_OK)
  {
    CoUninitialize();
  }
  else
  {
    if (hr == S_FALSE)
    {
      // Calls that return S_FALSE must be marched by a call to CoUninitialize.
      CoUninitialize();
    }
    initialized = true;
  }
  return initialized;
}

/*------------------------------------------------------------------------------------------------------------------
Win methods
------------------------------------------------------------------------------------------------------------------*/	

template <class T> 
inline void ReleaseCom(T*& pComObjectRef)
{
  if (pComObjectRef)
  {
    pComObjectRef->Release();
    pComObjectRef = NULL;
  }
}
}
}

#endif
