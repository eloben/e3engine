/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2012 Elías Lozada-Benavente

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
// Based on original by Ángel Riesgo
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Thread::Impl.h
This file declares the Thread::Impl implementation class for Windows.
*/

#ifndef E3_THREAD_IMPL_H
#define E3_THREAD_IMPL_H

namespace E
{
namespace Threads
{
// Forward declarations
class IRunnable;

/*----------------------------------------------------------------------------------------------------------------------
Thread::Impl
----------------------------------------------------------------------------------------------------------------------*/	
class Thread::Impl : public Memory::ProxyAllocated
{
public:
                      explicit Impl(IRunnable& entryPoint);
                      ~Impl();

  // Accessors
  const String&       GetName() const;
  bool			          IsRunning() const;
  void			          SetName(const String& name);

  // Methods          
  void			          Start();
  I32				          WaitForTermination();

  // Static methods
  E_API static U32    GetProcessorCount();
  static void	        Sleep(TimeValue tv);

private:
  IRunnable&		      mEntryPoint;      // Entry point object called from the new thread.
  bool			          mRunning;         // Boolean flag set to true while the new thread is running.
  mutable Mutex 	    mRunningMutex;    // Mutex to protect the access to the mRunning member.
  I32				          mReturnCode;      // Member that stores the value returned by the thread procedure.
  mutable Mutex	      mReturnCodeMutex; // Mutex that protects access to the mReturnCode member.
  String              mName;            // Member that stores the thread name.
  mutable Mutex	      mNameMutex;	      // Mutex that protects access to the mName member.
  HANDLE			        mHandle;          // HANDLE of the Win32 thread; we need to store it so that we can wait on it in the WaitForTermination method.

  static DWORD WINAPI Win32ThreadProc(LPVOID lpParameter); // /<Thread entry-point function as required by Win32

  E_DISABLE_COPY_AND_ASSSIGNMENT(Impl)
};
}
}

#endif
