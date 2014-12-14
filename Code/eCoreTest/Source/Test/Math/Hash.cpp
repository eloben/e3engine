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

// Created 04-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Hash.cpp
This file defines Hash test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Hash::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Hash::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Hash::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

struct HashFoo
{
  HashFoo(I32 i) : intValue(i) {}
  I32 intValue;
};

bool Test::Hash::RunFunctionalityTest()
{
  std::cout << "[Test::Hash::RunFunctionalityTest]" << std::endl;

  E::String s("A wonderful string");
  std::cout << "Djb2 Hashing: " << s.GetPtr() << " " << Math::Djb2<E::String>::Hash(s) << std::endl;
  std::cout << "Fnv32 Hashing: " << s.GetPtr() << " " << Math::Fnv32<E::String>::Hash(s) << std::endl;

  for (U32 i = 0; i < 32; ++i)
  {
    I32 v = Math::Global::GetRandom().GetI32(Math::NumericLimits<I32>::Min(), Math::NumericLimits<I32>::Max());
    std::cout << "Value: " << v << " Murmur3 32: " << Math::Murmur3<U32>::Hash(v)
              << " Murmur3 64: " << Math::Murmur3<U64>::Hash(v) << std::endl;
  }
  E::String s2;
  s2[0] = (E::StringBuffer::CharType) Math::Global::GetRandom().GetI32('a', 'z');
  s2[1] = (E::StringBuffer::CharType) Math::Global::GetRandom().GetI32('a', 'z');
  s2[2] = (E::StringBuffer::CharType) Math::Global::GetRandom().GetI32('a', 'z');
  s2[3] = 0;
  std::cout << "Hashing string ptr [" << s2.GetPtr() << "]: " << Math::Murmur3<size_t>::Hash((size_t)s2.GetPtr()) << std::endl;
  std::cout << std::endl;

  return true;
}

bool Test::Hash::RunPerformanceTest()
{
  std::cout << "[Test::Hash::RunPerformanceTest]" << std::endl;

  return true;
}
