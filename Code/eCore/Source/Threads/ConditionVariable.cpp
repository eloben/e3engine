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

/** @file ConditionVariable.cpp
This file defines the the ConditionVariable class.
*/

#include <CorePch.h>
#ifdef WIN32
#include "Win32/ConditionVariableImpl.h"
#endif

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
ConditionVariable initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

Threads::ConditionVariable::ConditionVariable()
: mpImpl(new Impl)
{
}

Threads::ConditionVariable::~ConditionVariable()
{
}

/*----------------------------------------------------------------------------------------------------------------------
ConditionVariable methods
----------------------------------------------------------------------------------------------------------------------*/	

/** Waits until the condition is signaled / broadcast unlocking the locked mutex m internally.
@throw very rare ThreadExceptions if an error occurs.
*/
void Threads::ConditionVariable::Wait(Mutex& m)
{
	mpImpl->Wait(m);
}

/** Wakes one waiting thread.
@throw very rare ThreadExceptions if an error occurs.
*/
void Threads::ConditionVariable::Signal()
{
	mpImpl->Signal();
}

/** Wakes all waiting threads.
@throw very rare ThreadExceptions if an error occurs.
*/
void Threads::ConditionVariable::Broadcast()
{
	mpImpl->Broadcast();
}
}
}
