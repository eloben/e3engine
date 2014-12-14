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

/** @file MutexImpl.h
This file contains the declaration of the MutexImpl implementation class for Windows.
*/

#ifndef E3_MUTEX_IMPL_H
#define E3_MUTEX_IMPL_H

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
MutexImpl

MutexImpl provides a lightweight implementation class of a Windows mutex. In Win32 there are two kinds of mutex 
objects: Critical Section and Windows Mutex. The latter are actually named mutexes that can be shared by different
processes. This class is a wrapper on the critical section type which is around 25 times faster due to the fact
that critical sections do not require to enter the kernel every time they are used (while Windows Mutex does):
http://preshing.com/20111124/always-use-a-lightweight-mutex/

Please note that this class has the following usage contract: 

1. MutexImpl implements a recursive mutex: if a mutex is locked N times, it will have to be unlocked also N times.
----------------------------------------------------------------------------------------------------------------------*/	
class Mutex::Impl : public Memory::ProxyAllocated
{
public:
        Impl()   { InitializeCriticalSection(&mWinCriticalSection); }
        ~Impl()  { DeleteCriticalSection(&mWinCriticalSection); }

  void  Lock()   { EnterCriticalSection(&mWinCriticalSection); }
  void  Unlock() { LeaveCriticalSection(&mWinCriticalSection); }

private:
  CRITICAL_SECTION	mWinCriticalSection;

  E_DISABLE_COPY_AND_ASSSIGNMENT(Impl)
};
}
}

#endif
