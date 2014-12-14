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
// $Date: $
// $Author: $

/** @file IntrusivePtr.cpp
This file defines IntrusivePtr test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

//struct Foo : IntrusiveReferenceCounter<Foo>             // Non thread-safe reference count
struct Foo : IntrusiveReferenceCounter<Foo, E::Threads::Atomic<U32>>  // Thread-safe reference count 
{
  Foo()
  {
    i = j = 1;
  }
  ~Foo()
  {
    i = j = 0;
  }

  I32 i;
  I32 j;
};

typedef IntrusivePtr<Foo> FooPtr;

struct Bar
{
  Bar()
  {
    i = j = 1;
  }
  ~Bar()
  {
    i = j = 0;
  }

  I32 i;
  I32 j;
};

struct FooTask : public E::Threads::IRunnable
{
  FooTask(const FooPtr& p)
    : ptr(p)
  {
  }

  I32 Run()
  {
    for (U32 i = 0; i < 1000; ++i)
    {
      //std::cout << '.';
      FooPtr p = ptr;
      p->i = i;
    }

    return 0;
  }

  FooPtr ptr;
};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::IntrusivePtr::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::IntrusivePtr::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::IntrusivePtr::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::IntrusivePtr::RunFunctionalityTest()
{
  std::cout << "[Test::IntrusivePtr::RunFunctionalityTest]" << std::endl;

  // Intrusive pointer
  FooPtr fooPtr(new Foo());
  E_ASSERT(fooPtr.IsUnique());
  FooPtr fooPtr2 = fooPtr;
  E_ASSERT(!fooPtr.IsUnique() && !fooPtr2.IsUnique());
  fooPtr = nullptr;
  E_ASSERT(fooPtr2.IsUnique());
  fooPtr = nullptr;
  fooPtr2 = nullptr;

  return true;
}

bool Test::IntrusivePtr::RunPerformanceTest()
{
#ifdef E_CPU_X64
  U32 i = 8;
#else
  U32 i = 4;
#endif
   i +=8;
  try
  {
    std::cout << "[Test::IntrusivePtr::RunPerformanceTest]" << std::endl;

    std::cout << "ThreadPool Test" << std::endl;
    E::Time::Timer t;
    E::Threads::ThreadPool pool;
    U32 taskCount = 1000;
    E::Containers::List<E::Threads::IRunnable*> fooList;
  
    /*-----------------------------------------------------------------
    IntrusivePtr
    -----------------------------------------------------------------*/
    
    FooPtr fooPtr(new Foo());
    E_ASSERT(fooPtr.IsUnique());

    // Load Foo thread tasks
    for (U32 i = 0; i < taskCount; ++i)
    {
      FooTask* pTask = new FooTask(fooPtr);
      fooList.PushBack(pTask);
    }

    t.Reset();
    for (E::Containers::List<E::Threads::IRunnable*>::ConstIterator cit = fooList.GetBegin(); cit != fooList.GetEnd(); ++cit)
    {
      pool.AddItem(*cit);
    }
    pool.WaitForIdle();
    std::cout << "Thread Intrusive pointer 32 bit test elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms." << std::endl;

    for (E::Containers::List<E::Threads::IRunnable*>::ConstIterator cit = fooList.GetBegin(); cit != fooList.GetEnd(); ++cit)
    {
      delete (*cit);
    }

    E_ASSERT_MSG(fooPtr.IsUnique(), "Reference count should be unique: try a thread-safe reference count method instead!");
    if (!fooPtr.IsUnique())
    {
      throw Exception("Reference count should be unique: try a thread-safe reference count method instead!");
    }
  }
  catch (...)
  {
    return false;
  }
  return true;
}
