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

// Created 08-Feb-2010 by Elías Lozada-Benavente (updated 14-Jan-2014)
// Based on original by Ángel Riesgo
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file WinConditionVariableImpl.cpp
This file defines the Windows version  of the ThreadImpl class.
*/

#include <CorePch.h>
#include "ConditionVariableImpl.h"

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
ConditionVariable::Impl initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

ConditionVariable::Impl::Impl() 
: mWaitersCount(0)
, mReleaseCount(0)
, mWaitGenerationCount(0)
, mEvent(0)
{
	mEvent = CreateEvent(
    nullptr, // No security
		TRUE, // Manual-reset
		FALSE,// Non-signaled initially
		nullptr);// Unnamed
}

ConditionVariable::Impl::~Impl()
{
	CloseHandle(mEvent);
}

/*----------------------------------------------------------------------------------------------------------------------
ConditionVariable::Impl methods
----------------------------------------------------------------------------------------------------------------------*/	

/** Waits until the condition is signaled/broadcast unlocking the locked mutex m internally.
@param m the mutex linked with the condition variable.
@throw nothing.
*/
void ConditionVariable::Impl::Wait(Mutex& m)
{
	I32 myGeneration = 0;
	// [Critical section]
	{
		Lock l(mWaitersCountMutex);
		++mWaitersCount;
		myGeneration = mWaitGenerationCount;
	}

	m.Unlock();

	bool waitDone = false;
	while(!waitDone)
	{
		WaitForSingleObject(mEvent, INFINITE);
		// [Critical section]
		{
			Lock l(mWaitersCountMutex);
			waitDone = (mReleaseCount > 0 && mWaitGenerationCount != myGeneration);
		}
	}

	m.Lock();

	bool lastWaiter = false;
	// [Critical section]
	{
		Lock l(mWaitersCountMutex);
		--mWaitersCount;
		--mReleaseCount;
		lastWaiter = (mReleaseCount == 0);
	}

	if(lastWaiter)
		ResetEvent(mEvent);
}

/** Wakes one waiting thread.
@throw nothing.
*/
void ConditionVariable::Impl::Signal()
{
	Lock l(mWaitersCountMutex);
	if(mWaitersCount > mReleaseCount)
	{
		SetEvent(mEvent);
		++mReleaseCount;
		++mWaitGenerationCount;
	}
}

/** Wakes all waiting threads.
@throw nothing.
*/
void ConditionVariable::Impl::Broadcast()
{
	Lock l(mWaitersCountMutex);
	if(mWaitersCount > 0)
	{
		SetEvent(mEvent);
		// Release all the threads in this generation.
		mReleaseCount = mWaitersCount;
		// Start a new generation.
		++mWaitGenerationCount;
	}
}
}
}
