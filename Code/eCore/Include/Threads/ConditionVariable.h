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

// Created 05-Feb-2010 by Elías Lozada-Benavente (updated 15-Jan-2014)
// Based on original by Ángel Riesgo
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ConditionVariable.h
This file declares a ConditionVariable class. Like other thread-related classes, ConditionVariable 
uses a "pimpl" idiom to isolate the	public class from any platform dependencies. ConditionVariableImpl
holds the platform specific implementation.
*/

#ifndef E3_CONDITION_VARIABLE_H
#define E3_CONDITION_VARIABLE_H

#include "Mutex.h"

namespace E
{
namespace Threads
{
class ConditionVariable
{
public:
  E_API ConditionVariable();
  E_API ~ConditionVariable();

  E_API void Wait(Mutex& m);
  E_API void Signal();
  E_API void Broadcast();

private:
  E_PIMPL mpImpl;
  E_DISABLE_COPY_AND_ASSSIGNMENT(ConditionVariable);
};
}
}

#endif
