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

/** @file Thread::Impl.cpp
This file defines the Windows version  of the Thread::Impl class.
*/

#include <CorePch.h>
#include "ThreadImpl.h"
#include <process.h>

namespace E
{
namespace Threads
{
#ifdef E_COMPILER_MSVC
/*----------------------------------------------------------------------------------------------------------------------
Msvc

This function sets the name of the current thread as it appears in the Threads window while debugging in Visual Studio.
Sets the thread debugging name. This is the name that will appear in the MSVC debugger. Note this method will get the 
name from GetName() method in Win32ThreadProc method during thread initialization. That means that SetName() method must 
be call before thread start in order to modify the thread name appearing in the debugger.
----------------------------------------------------------------------------------------------------------------------*/
void SetDebuggingThreadName(const String& name);
#endif
/*----------------------------------------------------------------------------------------------------------------------
Thread assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_THREAD_RUNNING "Thread should not be running on destruction"

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/	

static const char* kDefaultThreadName = "E::Thread";

#define E_MSVC_THREAD_NAME_EXCEPTION 0x406D1388

#pragma pack(push, 8)
struct ThreadNameInfo
{
   DWORD dwType;      // Must be 0x1000.
   LPCSTR szName;     // Pointer to name (in user addr space).
   DWORD dwThreadID;  // Thread ID (-1=caller thread).
   DWORD dwFlags;     // Reserved for future use, must be zero.
};
#pragma pack(pop)

HANDLE BeginThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,
  PVOID lpParameter,
  DWORD dwCreationFlags,
  LPDWORD lpThreadId);

/*----------------------------------------------------------------------------------------------------------------------
Thread::Impl initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

/** Constructor. It simply initializes the members. The new thread will only start when
the client code calls Start.
@param entryPoint the entry point object that will be executed, through its Run method, from the new thread.
@throw nothing.
*/
Threads::Thread::Impl::Impl(IRunnable& entryPoint)
  : mEntryPoint(entryPoint)
  , mRunning(false)
  , mRunningMutex()
  , mReturnCode(0)
  , mReturnCodeMutex()
  , mName(kDefaultThreadName)
  , mNameMutex()
  , mHandle(0)
{
}

/** Destructor. It is important to ensure that thread objects are not destroyed while the thread
operation is running. The client code should be able to communicate with the thread operation
to force it to exit before the thread object dies. Together with that, it may need to call
WaitForTermination before letting the thread object die.
@throw nothing
*/
Threads::Thread::Impl::~Impl()
{
	E_ASSERT_MSG(!IsRunning(), E_ASSERT_MSG_THREAD_RUNNING);

	// We don't need to keep track of the thread handle, so we can close it. Note that calling CloseHandle
	// on a thread handle is just a way of notifying Windows that we are not using the handle. It won't affect
	// the running thread at all.
	CloseHandle(mHandle);
}

/*----------------------------------------------------------------------------------------------------------------------
Thread::Impl accessors
----------------------------------------------------------------------------------------------------------------------*/	

const String& Threads::Thread::Impl::GetName() const
{
	Lock l(mNameMutex);
	return mName;
}

bool Threads::Thread::Impl::IsRunning() const
{
  Lock lock(mRunningMutex);
  return mRunning;
}

void Threads::Thread::Impl::SetName(const String& name)
{
	Lock l(mNameMutex);
	mName = name;
}

/*----------------------------------------------------------------------------------------------------------------------
Thread::Impl methods
----------------------------------------------------------------------------------------------------------------------*/	

/**Starts a separate thread, calling Run on the IRunnable object passed to the constructor.
@throw ThreadException if a system error occurs when creating the thread.
*/
void Threads::Thread::Impl::Start()
{
	// We have to lock the mRunningMutex so that calls to IsRunning from other threads will block until
	// Start has returned.
	Lock lock(mRunningMutex);
	DWORD dummy; // this is only required for Windows 95/98/Me, which does not allow a nullptr parameter in CreateThread.
	DWORD creationFlags = CREATE_SUSPENDED;
	mHandle = BeginThread(0, 0, &Win32ThreadProc, (LPVOID)(this), creationFlags, &dummy);
	::ResumeThread(mHandle);
	mRunning = true;
}

/** Method that blocks until the thread is no longer running. It will return immediately if the thread has already
exited or if Start has not been called.
@return	the return value of the entry point's Run method or 0 if Start has not been called.
@throw nothing, except for extremely exceptional ThreadExceptions caused by the mutex object.
*/
I32 Threads::Thread::Impl::WaitForTermination()
{
	WaitForSingleObject(mHandle, INFINITE);

	// // The return code mutex is active while the thread is running, so we can only get hold of it
	// // after the thread terminates (or when it has not started).
	Lock lock(mReturnCodeMutex);
	return mReturnCode;
}


/*----------------------------------------------------------------------------------------------------------------------
Thread::Impl static methods
----------------------------------------------------------------------------------------------------------------------*/	

U32 Threads::Thread::Impl::GetProcessorCount()
{
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return sysinfo.dwNumberOfProcessors;
}

/** This method sleeps the current thread for a specified amount of time.
@param tv a TimeValue.
@throw nothing.
*/
void Threads::Thread::Impl::Sleep(TimeValue tv)
{
	DWORD milliseconds = (DWORD)(tv / 1000L);
	::Sleep(milliseconds);
}

/*----------------------------------------------------------------------------------------------------------------------
Thread::Impl private methods
----------------------------------------------------------------------------------------------------------------------*/	

/** This static method has the right signature for a Win32 thread entry-point. By passing the address of this
function to the Win32 API call that takes a function pointer, we can execute IRunnable::Run from the
function, which also allows us to keep track of when the Run method is called and when it returns.
@param lpParameter a void pointer as required by Windows, which we use to pass the pointer to the class.
@return the return code of the EntryPoint::Run method.
@throw nothing, except for extremely rare Mutex exceptions. IRunnable::Run implementations SHOULD NOT throw exceptions to guarantee this.
*/
DWORD WINAPI Threads::Thread::Impl::Win32ThreadProc(LPVOID lpParameter)
{
	Thread::Impl* pThread = (Thread::Impl*)lpParameter;

  // Instance ComValidityScope to ensure that COM calls will not fail in this thread.
	Win32::ComValidityScope com;	

  #ifdef E_COMPILER_MSVC
  SetDebuggingThreadName(pThread->GetName());
  #endif

	DWORD returnCode = pThread->mEntryPoint.Run();

	{
		Lock lock(pThread->mReturnCodeMutex);
		pThread->mReturnCode = returnCode;
	}
	{
		Lock lock(pThread->mRunningMutex);
		pThread->mRunning = false;
	}
	return returnCode;
}

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary functions
----------------------------------------------------------------------------------------------------------------------*/	

/*Auxiliary function. CreateThread is the standard Win32 function to create a new thread. However,
because it does not initialise the C run-time library, it should not be used if the new thread can
potentially use the standard C or C++ libraries. Such a situation should be perfectly normal in our
code, so we have to create the thread using _beginthreadex, a wrapper function provided by the Microsoft
C run-time library which calls CreateThread and also initialises the C run-time library.
*/
HANDLE BeginThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,
  PVOID lpParameter,
  DWORD dwCreationFlags,
  LPDWORD lpThreadId)
{
  return (HANDLE)_beginthreadex(
    (void*)lpThreadAttributes, 
    (U32)dwStackSize, 
    (U32(__stdcall *)(void*))(lpStartAddress),
    lpParameter, 
    dwCreationFlags, 
    (U32*)lpThreadId);
}



#ifdef E_COMPILER_MSVC
/*----------------------------------------------------------------------------------------------------------------------
Msvc methods
----------------------------------------------------------------------------------------------------------------------*/
void SetDebuggingThreadName(const String& name)
{
  ThreadNameInfo info;
  info.dwType = 0x1000;
  info.szName = name.GetPtr();
  info.dwThreadID = ::GetCurrentThreadId();
  info.dwFlags = 0;
  __try
  {
    RaiseException(E_MSVC_THREAD_NAME_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
  }
}
#endif
}
}
