/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

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

// Created 29-Jan-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $E::Time::Date: $
// $Author: $

/** @file Time.cpp
This file defines Time test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

inline E::Serialization::ISerializer& operator<<(E::Serialization::ISerializer& target, const E::Time::Date& source)
{
  static const char* kWeekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  E::String str;
  str.Print("%s %02d/%02d/%04d %02d:%02d:%02d",  kWeekDays[source.weekDay], source.day, source.month, source.year,
    source.hour, source.minute, source.second);
  return target << str;
}

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Time::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Time::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Time::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}


bool Test::Time::RunFunctionalityTest()
{
  std::cout << "[Test::Time::RunFunctionalityTest]" << std::endl;
  TimeValue now = E::Time::GetCurrentTime();
  TimeValue nowUtc = E::Time::GetCurrentTimeUtc();

  E::Time::Date today;
  E::Time::GetDate(now, today);

  E::Time::Date todayUtc;
  E::Time::GetDate(nowUtc, todayUtc);

  E::Serialization::StringSerializer ss;
  ss << "Now: " << today;
  std::cout << ss.GetPtr() << std::endl;
  
  ss.Clear();

  ss << "Now UTC: " << todayUtc;
  std::cout << ss.GetPtr() << std::endl;
  
  return true;
}

bool Test::Time::RunPerformanceTest()
{
  std::cout << "[Test::Time::RunPerformanceTest]" << std::endl;

  return true;
}
