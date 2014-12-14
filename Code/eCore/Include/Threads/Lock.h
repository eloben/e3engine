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

// Created 05-Feb-2010 by Elías Lozada-Benavente
// Based on original by Ángel Riesgo
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Lock.h
This file defines a helper class, Lock, that can be used to lock mutexes in an exception-safe way.
Lock class implements a RAII idiom ("rein acquisition is initialization") for mutexes. It is 
strongly advised to use this helper class for locking mutexes rather than calling Mutex::Lock and 
Mutex::Unlock directly.
*/

#ifndef E3_LOCK_H
#define E3_LOCK_H

#include "Mutex.h"
#include <Base.h>

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
Lock
----------------------------------------------------------------------------------------------------------------------*/	
class Lock
{
public:
	explicit Lock(Mutex& m) : mMutex(m) { mMutex.Lock();}
	~Lock() { mMutex.Unlock(); }

private:
	Mutex& mMutex;
	E_DISABLE_COPY_AND_ASSSIGNMENT(Lock)
};
}
}

#endif
