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

// Created 10-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Timer.h
This file declares the Timer class. Timer class acts as a stopwatch. It allows to measure time between two 
or more points of execution.
*/

#ifndef E3_TIME_TIMER_H
#define E3_TIME_TIMER_H

#include "Time.h"
#include <Base.h>

namespace E 
{
namespace Time
{
/*----------------------------------------------------------------------------------------------------------------------
Timer

Please note that this class has the following usage contract: 

1. Reset returns the elapsed time.
----------------------------------------------------------------------------------------------------------------------*/
class Timer
{
public:
  Timer();

  // Accessors
  TimeValue GetElapsed() const;

  // Methods
  TimeValue Reset();

private:
  TimeValue mStart;

  E_DISABLE_COPY_AND_ASSSIGNMENT(Timer)
};

/*----------------------------------------------------------------------------------------------------------------------
Timer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

inline Timer::Timer()
{
  mStart = Time::GetCpuTime();
}

/*----------------------------------------------------------------------------------------------------------------------
Timer accessors
----------------------------------------------------------------------------------------------------------------------*/

inline TimeValue Timer::GetElapsed() const
{
  TimeValue present = Time::GetCpuTime();
  return (present - mStart);
}

/*----------------------------------------------------------------------------------------------------------------------
Timer methods
----------------------------------------------------------------------------------------------------------------------*/

inline TimeValue Timer::Reset()
{
  TimeValue present = Time::GetCpuTime();
  TimeValue delta = present - mStart;
  mStart = present;
  return delta;
}
}

/*----------------------------------------------------------------------------------------------------------------------
Time types
----------------------------------------------------------------------------------------------------------------------*/
typedef Time::Timer Timer;
}

#endif
