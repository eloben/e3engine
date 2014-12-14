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

/** @file Thread.h
This file declares a Thread class. The Thread class hides away platform dependencies by using a pimpl idiom, 
storing a pointer to a private implementation class that will differ depending on platform. Currently Windows
version is the only implemented. An interface IRunnable is also provided to implement thread executable objects.
IRunnable interface is intended to be used as a base class to define an actual operation that needs to be carried 
out in a separate thread. IRunnable	objects are passed to Thread instances on initialization, and will be executed 
on a new thread.
*/

#ifndef E3_THREAD_H
#define E3_THREAD_H

#include "IRunnable.h"
#include <Text/String.h>
#include <Time/Time.h>
#include <Base.h>

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
Thread
----------------------------------------------------------------------------------------------------------------------*/	
class Thread
{
public:
  E_API explicit Thread(IRunnable& entryPoint);
  E_API ~Thread();

  // Accessors
  E_API const String& GetName() const;
  E_API bool		            IsRunning() const;
  E_API void		            SetName(const String& name);

  // Methods
  E_API void		            Start();
  E_API I32			            WaitForTermination();

  // Static methods
  E_API static U32          GetProcessorCount();
  E_API static void         Sleep(TimeValue tv);
  
private:
  E_PIMPL mpImpl;
  E_DISABLE_COPY_AND_ASSSIGNMENT(Thread)
};
}
}

#endif
