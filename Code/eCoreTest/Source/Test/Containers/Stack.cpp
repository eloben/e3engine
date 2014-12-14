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

// Created 10-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file E::Containers::Stack.cpp
This file defines E::Containers::Stack test functions.
*/

#include <CoreTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

#ifndef TEST_SMALL_SIZE
#define TEST_SMALL_SIZE 256
#endif

#ifndef TEST_BIG_SIZE
#define TEST_BIG_SIZE 4096
#endif

#ifndef TEST_SIZE
#define TEST_SIZE TEST_BIG_SIZE
#endif

struct Pod
{
  char c;
  int i;
  float f;
};

E_DECLARE_POD(Pod)

struct NonPod
{
  char c;
  int i;
  float f;
};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Stack::Run()
{
  try
  {
    std::cout << "[Test::Stack::Run] using array size of " << TEST_SIZE << std::endl;
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Stack::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Stack::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Stack::RunFunctionalityTest()
{
  try
  {
    std::cout << "[Test::Stack::RunFunctionalityTest]" << std::endl;

    /*-----------------------------------------------------------------
    Construction
    -----------------------------------------------------------------*/
    //Stack();
    //explicit Stack(U32 size);
    //~Stack();
    
    E::Containers::Stack<I32> stack;
    E::Containers::Stack<I32>  preSizedStack(TEST_SIZE);
    E::Containers::List<I32> list(TEST_SIZE);

    // Assignment
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      U32 n = Math::Global::GetRandom().GetI32(-65536, 65536);
      list.PushBack(n);
      preSizedStack.Push(n);
    }
    stack = preSizedStack;

    size_t listIndex = list.GetCount() - 1;
    while(!stack.IsEmpty() && !preSizedStack.IsEmpty())
    {
      E_ASSERT(stack.GetTop() == preSizedStack.GetTop());
      E_ASSERT(stack.GetTop() == list[listIndex]);
      listIndex --;
      stack.Pop();
      preSizedStack.Pop();
    }

    for (E::Containers::List<I32>::ConstIterator cit = list.GetBegin(); cit != list.GetEnd(); ++cit)
    {
      stack.Push(*cit);
    }

    /*-----------------------------------------------------------------
    Accessors
    -----------------------------------------------------------------*/
    //U32                     GetCount() const;
    //U32                     GetGranularity() const;
    //F32                     GetGrowthPercentage() const;
    //U32                     GetSize() const;
    //const T&                GetTop() const; (already tested)
    //T&                      GetTop();
    //bool                    Has(const T& value) const;
    //bool                    IsEmpty() const;
    //void                    SetGranularity(U32 v);
    //void                    SetGrowthFactor(F32 v);

    E_ASSERT(stack.GetCount() == TEST_SIZE);
    E_ASSERT(stack.GetGranularity() == preSizedStack.GetGranularity());
    E_ASSERT(stack.GetGrowthPercentage() == preSizedStack.GetGrowthPercentage());
    E_ASSERT(stack.GetSize() == preSizedStack.GetSize());

    for (E::Containers::List<I32>::ConstIterator cit = list.GetBegin(); cit != list.GetEnd(); ++cit)
    {
      E_ASSERT(stack.HasValue(*cit));
    }
    E_ASSERT(!stack.IsEmpty());
    
    stack.GetTop() = 0;
    E_ASSERT(stack.GetTop() == 0);

    E_ASSERT(stack.GetGranularity() == 1 && stack.GetGrowthPercentage() == 50);

    /*-----------------------------------------------------------------
    Methods
    -----------------------------------------------------------------*/
    //void                    Clear();
    //void                    Compact();
    //void                    EnsureSize(U32 size);
    //void                    Pop(U32 count = 1);
    //void                    Push(const T& value);
    //void                    Push(const List<T, AllocatorClass>& rhs);
    //void                    PushFast(const T& value);
    //void                    PushFast(const List<T, AllocatorClass>& rhs);
    //void                    Reserve(U32 size);
    //void                    Resize(U32 size);

    // Resize test
    stack.Trim(10);
    E_ASSERT(stack.GetTop() == list[9]);
    E_ASSERT(stack.GetCount() == 10);
    stack.Clear();
    E_ASSERT(stack.IsEmpty());
    stack.Compact();
    E_ASSERT(stack.GetSize() == 0);
    stack.EnsureSize(TEST_SIZE);
    E_ASSERT(stack.GetSize() >= TEST_SIZE);
    stack.Push(list.GetPtr(), list.GetCount());
    stack.Resize(0);
    E_ASSERT(stack.GetSize() == 0);
    stack.Reserve(TEST_SIZE);
    E_ASSERT(stack.GetSize() >= TEST_SIZE);
    stack.Resize(10);
    E_ASSERT(stack.GetSize() == 10);
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool Test::Stack::RunPerformanceTest()
{
  try
  {
    std::cout << "[Test::Stack::RunPerformanceTest]" << std::endl;
 
  }
  catch (...)
  {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary methods
----------------------------------------------------------------------------------------------------------------------*/
