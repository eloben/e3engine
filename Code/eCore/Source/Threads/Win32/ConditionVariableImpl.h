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

/** @file ConditionVariable::Impl.h
This file declares the Windows version  of the ThreadImpl class.
Implementing condition variables for Win32 is notoriously difficult (until the arrival of the newer
Vista API functions). This solution follows the technique by Douglas C. Schmidt and Irfan Pyarali:
http:// www.cs.wustl.edu/~schmidt/win32-cv-1.html (see 3.3 the Generation Count solution). We have
to use the 3.3 solution, and not 3.4, because our Mutex class encapsulates a Win32 CRITICAL_SECTION 
rather than a mutex HANDLE. Besides, the SignalObjectAndWait function used by the solution 3.4 is 
not available in Windows 95, 98 and ME, so this solution is more portable.
*/

#ifndef E3_CONDITION_VARIABLE_IMPL_H
#define E3_CONDITION_VARIABLE_IMPL_H

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
ConditionVariable::Impl
----------------------------------------------------------------------------------------------------------------------*/	
class ConditionVariable::Impl : public Memory::ProxyAllocated
{
public:
          Impl();
          ~Impl();

  void 	  Wait(Mutex& m);
  void 	  Signal();
  void 	  Broadcast();

private:
  I32		  mWaitersCount; // /< Number of waiters.
  Mutex	  mWaitersCountMutex; // /< critical section guard for mWaitersCount.
  I32		  mReleaseCount; // /< Number of threads to release via Signal or Broadcast
  I32		  mWaitGenerationCount; // /< Keeps track of the current generation (see Schmidt and Pyarali).
  HANDLE	mEvent; // /< A manual-reset event used to block and release waiting threads.

  E_DISABLE_COPY_AND_ASSSIGNMENT(Impl)
};
}
}

#endif
