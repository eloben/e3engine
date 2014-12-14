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

/** @file Thread.cpp
This file defines the the Thread class.
*/

#include <CorePch.h>
#ifdef WIN32
#include "Win32/ThreadImpl.h"
#endif

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
Thread initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

Threads::Thread::Thread(IRunnable& entryPoint)
: mpImpl(new Impl(entryPoint))
{
}

Threads::Thread::~Thread()
{
}

/*----------------------------------------------------------------------------------------------------------------------
Thread accessors
----------------------------------------------------------------------------------------------------------------------*/	

const String& Threads::Thread::GetName() const
{
	return mpImpl->GetName();
}

bool Threads::Thread::IsRunning() const
{
  return mpImpl->IsRunning();
}

void Threads::Thread::SetName(const String& name)
{
	mpImpl->SetName(name);
}

/*----------------------------------------------------------------------------------------------------------------------
Thread methods
----------------------------------------------------------------------------------------------------------------------*/	

/** This method should be called after creating the object for the new system thread to be created.
The method Run of the entry point object passed to the constructor will be called from the new thread.
@throw ThreadException if an error occurs.
*/
void Threads::Thread::Start()
{
	mpImpl->Start();
}

/** This method returns immediately if the new thread is not running, either because Start has not been called
or because the Run method of the entry point object has already returned. Otherwise, if the Run method of the
entry point object is being executed, it blocks until the thread terminates.
@throw nothing.
*/
I32 Threads::Thread::WaitForTermination()
{
	return mpImpl->WaitForTermination();
}

/*----------------------------------------------------------------------------------------------------------------------
Thread static methods
----------------------------------------------------------------------------------------------------------------------*/	

U32 Threads::Thread::GetProcessorCount()
{
  return Impl::GetProcessorCount();
}

/** This method sleeps the current thread for a specified amount of time.
@param tv a TimeValue.
@throw nothing.
*/
void Threads::Thread::Sleep(TimeValue tv)
{
	Impl::Sleep(tv);
}
}
}
