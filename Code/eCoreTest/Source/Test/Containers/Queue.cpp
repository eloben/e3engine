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

/** @file E::Containers::Queue.cpp
This file defines E::Containers::Queue test functions.
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
#define TEST_SIZE 1
#endif

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Queue::Run()
{
  try
  {
    std::cout << "[Test::Queue::Run] using array size of " << TEST_SIZE << std::endl;
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Queue::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Queue::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Queue::RunFunctionalityTest()
{
  try
  {
    std::cout << "[Test::Queue::RunFunctionalityTest]" << std::endl;

    /*-----------------------------------------------------------------
    Construction
    -----------------------------------------------------------------*/
    //Queue();
    //explicit Queue(U32 size);

    E::Containers::Queue<I32> queue;
    E::Containers::Queue<I32> preSizedQueue(TEST_SIZE);
    E::Containers::List<I32> list(TEST_SIZE);

    // Assignment
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      U32 n = Math::Global::GetRandom().GetI32(-65536, 65536);
      list.PushBack(n);
      preSizedQueue.Push(n);
    }
    queue = preSizedQueue;

    U32 listIndex = 0;
    while(!queue.IsEmpty() && !preSizedQueue.IsEmpty())
    {
      E_ASSERT(queue.GetFront() == preSizedQueue.GetFront());
      E_ASSERT(queue.GetFront() == list[listIndex]);
      listIndex ++;
      queue.Pop();
      preSizedQueue.Pop();
    }

    for (E::Containers::List<I32>::ConstIterator cit = list.GetBegin(); cit != list.GetEnd(); ++cit)
    {
      queue.Push(*cit);
    }

    /*-----------------------------------------------------------------
    Accessors
    -----------------------------------------------------------------*/

    //U32                     GetCount() const;
    //const T&                GetFront() const; (already tested)
    //T&                      GetFront();
    //U32                     GetGranularity() const;
    //F32                     GetGrowthPercentage() const;
    //U32                     GetSize() const;
    //bool                    Has(const T& value) const;
    //bool                    IsEmpty() const;
    //void                    SetGranularity(U32 v);
    //void                    SetGrowthFactor(F32 v);
  
    E_ASSERT(queue.GetCount() == TEST_SIZE);
    E_ASSERT(queue.GetGrowthPercentage() == preSizedQueue.GetGrowthPercentage());
    E_ASSERT(queue.GetSize() == preSizedQueue.GetSize());
    
    for (E::Containers::List<I32>::ConstIterator cit = list.GetBegin(); cit != list.GetEnd(); ++cit)
    {
      E_ASSERT(queue.HasValue(*cit));
    }
    E_ASSERT(!queue.IsEmpty());

    queue.GetFront() = 0;
    E_ASSERT(queue.GetFront() == 0);

    /*-----------------------------------------------------------------
    Methods
    -----------------------------------------------------------------*/
 
    //void                    Clear();
    //void                    Compact();
    //void                    EnsureSize(U32 size);
    //void                    Pop(U32 count = 1);  (already tested)
    //void                    Push(const T& value); (already tested)
    //void                    Push(const T* pData, U32 count);
    //void                    Reserve(U32 size);
    //void                    Resize(U32 size);
    //void                    Trim(U32 size);

    // Resize test
    queue.Trim(10);
    E_ASSERT(queue.GetCount() == 10);
    queue.Clear();
    E_ASSERT(queue.IsEmpty());
    queue.Compact();
    E_ASSERT(queue.GetSize() == 0);
    queue.EnsureSize(TEST_SIZE);
    E_ASSERT(queue.GetSize() >= TEST_SIZE);
    queue.Push(list.GetPtr(), list.GetCount());
    queue.Resize(0);
    E_ASSERT(queue.GetSize() == 0);
    queue.Reserve(TEST_SIZE);
    E_ASSERT(queue.GetSize() >= TEST_SIZE);
    queue.Resize(10);
    E_ASSERT(queue.GetSize() == 10);

    // Main Test
    queue.Resize(0);
    for (U32 i = 0; i < 10; ++i)
    {
      for (U32 j = 0; j < 3; ++j)
      {
        queue.Push(j);
      }
      queue.Pop(3);
    }
    E_ASSERT(queue.IsEmpty());

    // Grow Test
    queue.Reserve(0);
    for (U32 i = 0; i < 10; ++i)
    {
      for (U32 j = 0; j < i; ++j)
      {
        for (U32 k = 0; k < 3; ++k)
        {
          queue.Push(k);
          E_ASSERT(queue.HasValue(k));
        }
      }
    }
    queue.Resize(10);

    // Trim test
    for (U32 i = 0; i < 10; ++i)
    {
      queue.Push(i);
    }
    for (U32 i = 0; i < 10; ++i)
    {
      E_ASSERT(queue.HasValue(i));
    }
    queue.Pop(9);
    E_ASSERT(queue.GetFront() == 9);

    for (U32 i = 0; i < 5; ++i)
    {
      queue.Push(i);
    }
    for (U32 i = 0; i < 5; ++i)
    {
      E_ASSERT(queue.HasValue(i));
    }
    queue.Trim(5);
    queue.Pop();
    
    for (I32 i = 0; i < 4; ++i)
    {
      E_ASSERT(queue.GetFront() == i);
      queue.Pop();
    }
    E_ASSERT(queue.IsEmpty());
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool Test::Queue::RunPerformanceTest()
{
  try
  {
    std::cout << "[Test::Queue::RunPerformanceTest]" << std::endl;

    U32 maxOperationCount = 1 << 15; // 20;
    U32 iterationCount = 100; // 25;
    U32 queueOperationRange = 100; // 25;
    F32 popTime = 0;
    F32 pushTime = 0;;
    F32 popTimeStd = 0;;
    F32 pushTimeStd = 0;;

    std::queue<I32> stdQ;
    E::Containers::Queue<I32> q;
    
    std::cout << "Iterations: " << iterationCount << " Max ops: " << maxOperationCount << std::endl << std::endl;
    for (U32 j = 0; j < iterationCount; ++j)
    {
      for (U32 i = 2; i <= maxOperationCount; i*=2)
      {
        U32 pushOperationCount = Math::Global::GetRandom().GetU32(queueOperationRange);
        for (U32 p = 0; p  < pushOperationCount; ++p)
        {
          I32 v = Math::Global::GetRandom().GetI32(0, 1000);
          E::Time::Timer t;
          stdQ.push(v);
          pushTimeStd += static_cast<F32>(t.GetElapsed().GetMilliseconds());
          t.Reset();
          q.Push(v);
          pushTime += static_cast<F32>(t.GetElapsed().GetMilliseconds());
        }

        U32 popOperationCount = Math::Global::GetRandom().GetU32(pushOperationCount);
        for (U32 p = 0; p  < popOperationCount; ++p)
        {
          E::Time::Timer t;
          stdQ.pop();
          popTimeStd += static_cast<F32>(t.GetElapsed().GetMilliseconds());
          t.Reset();
          q.Pop();
          popTime += static_cast<F32>(t.GetElapsed().GetMilliseconds());
        }
      }

      std::cout << "Iteration: " << j << " max ops: " << maxOperationCount << std::endl;
      std::cout << "Push time [" << pushTime << " / " << pushTimeStd << "]\t" << (pushTimeStd / pushTime * 100.0) - 100.0 << "% faster" << std::endl;
      std::cout << "Pop time [" << popTime << " / " << popTimeStd << "]\t" << (popTimeStd / popTime * 100.0) - 100.0 << "% faster" << std::endl;
      std::cout << std::endl;
      /*
      Iteration: 99 operation count: 32768
      Push time [19.6709 / 52.9868]   169.366% faster
      Pop time [8.85995 / 9.02602]    1.87439% faster
      Elapsed time: 1087.4 ms
      */
    }

    while (!q.IsEmpty())
    {
      E_ASSERT(q.GetFront() == stdQ.front());
      q.Pop();
      stdQ.pop();
    }
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
