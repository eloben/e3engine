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

// Created 16-Sep-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ContainerTest.h
This file declares container test functions
*/

#ifndef E3_TEST_COMMMON_H
#define E3_TEST_COMMMON_H

#include <Assertion/Exception.h>
#include <Time/Timer.h>
#include <Text/String.h>
#include <iostream>

namespace E
{
  namespace Test
  {
    void PrintException(const E::Exception& e);
    void PrintResultTime(bool result, const E::Time::Timer& t, const E::StringBuffer& msg = "");
    void PrintResultTimeAndReset(bool result, E::Time::Timer& t, const E::StringBuffer& msg = "");
    void PrintTime(const E::Time::Timer& t, const E::StringBuffer& msg = "");
    void PrintTimeAndReset(E::Time::Timer& t, const E::StringBuffer& msg = "");
  }

  /*----------------------------------------------------------------------------------------------------------------------
  Test functions
  ----------------------------------------------------------------------------------------------------------------------*/

  inline void Test::PrintException(const E::Exception& e)
  {
    std::cout << "Exception [" << e.description.GetPtr() << "] " << std::endl;
  }

  inline void Test::PrintResultTime(bool result, const E::Time::Timer& t, const E::StringBuffer& msg /* = "" */)
  {
    result ? std::cout << "Succeeded! " : std::cout << " Failed! ";
    PrintTime(t, msg);
  }

  inline void Test::PrintResultTimeAndReset(bool result, E::Time::Timer& t, const E::StringBuffer& msg /* = "" */)
  {
    result ? std::cout << "Succeeded! " : std::cout << " Failed! ";
    PrintTimeAndReset(t, msg);
  }

  inline void Test::PrintTime(const E::Time::Timer& t, const E::StringBuffer& msg /*= ""*/)
  {
    std::cout << "Elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms [" << msg.GetPtr() << "]" << std::endl;
  }

  inline void Test::PrintTimeAndReset(E::Time::Timer& t, const E::StringBuffer& msg /*= ""*/)
  {
    PrintTime(t, msg);
    t.Reset();
  }
}
#endif
