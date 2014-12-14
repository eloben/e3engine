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

/** @file SharedPtr.cpp
This file defines SharedPtr test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

class IFoo
{
public:
  enum Type
  {
    eA,
    eB,
    eTypeCount
  };


  virtual ~IFoo()
  {}

  virtual void Print() = 0;
};

class FooA : public IFoo
{
public:
  FooA()
    : mFloatValue(1.0f)
    , mCharValue('a')
  {

  }

  void Print()
  {
    std::cout << "I am FooA" << std::endl;
  }

private:
  F32   mFloatValue;
  char  mCharValue;
};

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

//typedef E::SharedPtr<Bar> BarPtr;              // Non thread-safe reference count
typedef E::SharedPtr<Bar, E::Threads::Atomic<U32>> BarPtr;   // Thread-safe reference count
typedef E::SharedPtr<Bar, E::Threads::Atomic<U64>> BarPtr64;   // Thread-safe reference count

template <typename T>
struct CustomDeleter
{
  static void Delete(T* pObject) { E_DELETE(pObject);}
};

struct BarTask : public E::Threads::IRunnable
{
  BarTask(const BarPtr& p)
    : ptr(p)
  {
  }

  I32 Run()
  {
    for (U32 i = 0; i < 1000; ++i)
    {
      //std::cout << '.';
      BarPtr p = ptr;
      p->i = i;
    }

    return 0;
  }

  BarPtr ptr;
};

struct BarTask64 : public E::Threads::IRunnable
{
  BarTask64(const BarPtr64& p)
    : ptr(p)
  {
  }

  I32 Run()
  {
    for (U32 i = 0; i < 1000; ++i)
    {
      //std::cout << '.';
      BarPtr64 p = ptr;
      p->i = i;
    }

    return 0;
  }

  BarPtr64 ptr;
};

struct IBoo
{
  virtual ~IBoo() {}

  virtual void SayMyName() = 0;
};

struct Boo: IBoo
{
  void SayMyName() { std::cout << "I am A" << std::endl; }
  I32 booValue;
};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::SharedPtr::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::SharedPtr::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::SharedPtr::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::SharedPtr::RunFunctionalityTest()
{
  std::cout << "[Test::SharedPtr::RunFunctionalityTest]" << std::endl;

  // Standard SharedPtr usage:
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA;
    spFooA = pFooA;
    spFooA->Print();
  }

  // Direct construction 
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA(pFooA);
    spFooA->Print();
  }

  // Copy construction SharedPtr usage:
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA, spAnotherFooA;
    spFooA = pFooA;
    spFooA->Print();
    spAnotherFooA = spFooA;
    spAnotherFooA->Print();
    E::SharedPtr<FooA> spYetAnotherFooA(spFooA);
    spYetAnotherFooA->Print();
  }

  // SharedPtr implicit conversion
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA, spAnotherFooA;
    E::SharedPtr<IFoo> spIFoo;

    spFooA = pFooA;
    spFooA->Print();
    spIFoo = spFooA;
    spIFoo->Print();
    spAnotherFooA = spIFoo;
    spAnotherFooA->Print();
  }

  // SharedPtr destruction by nullptr
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA, spAnotherFooA;
    E::SharedPtr<IFoo> spIFoo;
    spFooA = pFooA;
    spIFoo = spFooA;
    spFooA = nullptr;
    spIFoo = nullptr;
  }

  // Custom SharedPtr
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA, A32> spFooA, spAnotherFooA;
    E::SharedPtr<IFoo, A32> spIFoo;

    spFooA = pFooA;
    spFooA->Print();
    spIFoo = spFooA;
    spIFoo->Print();
    spAnotherFooA = spIFoo;
    spAnotherFooA->Print();
  }

  // Custom SharedPtr II
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA, A32, Memory::Deleter<IFoo>> spFooA, spAnotherFooA;
    E::SharedPtr<IFoo, A32, Memory::Deleter<IFoo>> spIFoo;

    spFooA = pFooA;
    spFooA->Print();
    spIFoo = spFooA;
    spIFoo->Print();
    spAnotherFooA = spIFoo;
    spAnotherFooA->Print();
  }

   // Shared pointer
  BarPtr barPtr(new Bar());
  E_ASSERT(barPtr.IsUnique());
  BarPtr barPtr2 = barPtr;
  E_ASSERT(!barPtr.IsUnique() && !barPtr2.IsUnique());
  barPtr = nullptr;
  E_ASSERT(barPtr2.IsUnique());
  barPtr = nullptr;
  barPtr2 = nullptr;

  BarPtr barPtrs[2];
  barPtrs[0] = BarPtr(new Bar());
  barPtrs[1] = new Bar();
  E_ASSERT(barPtrs[0]);
  E_ASSERT(barPtrs[1]);
  barPtr = barPtrs[1];
  barPtrs[1] = nullptr;
  E_ASSERT(barPtr.IsUnique());

  // Implicit static cast test
  E::SharedPtr<Boo> spBoo(new Boo());
  E_ASSERT(spBoo.IsUnique());
  E::SharedPtr<Boo> spBoo2;
  E_ASSERT(!spBoo2.IsUnique());
  spBoo2 = spBoo;
  E_ASSERT(spBoo2);
  E_ASSERT(!spBoo.IsUnique() && !spBoo2.IsUnique());

  // Basic cast between default templates
  E::SharedPtr<IBoo> spIBoo = spBoo;
  E::SharedPtr<Boo> spBooBack = spIBoo;

  // Specialized template cast from default template
  E::SharedPtr<IBoo, U32, CustomDeleter<IBoo>> spCustomIBoo = spBoo;
  spCustomIBoo = spIBoo;
  
  // Specialized template cast from specialized template
  E::SharedPtr<Boo, U32, CustomDeleter<IBoo>> spCustomBoo = spCustomIBoo;
  spCustomBoo = spBoo;
  spCustomIBoo = spCustomBoo;

  /*
  E::SharedPtr<IBoo, A32, CustomDeleter<IBoo>> spCustomIBoo2 = spBoo;        // CounterType cannot be mixed
  E::SharedPtr<Boo, A32, CustomDeleter<IBoo>> spCustomBoo2 = spCustomIBoo2;  // CounterType cannot be mixed
  */

  E_ASSERT(!spBoo.IsUnique() && !spBoo2.IsUnique() && !spIBoo.IsUnique() && !spCustomIBoo.IsUnique());
  E::SharedPtr<IBoo> spIBoo2;
  spIBoo->SayMyName();
  spIBoo2 = spBoo;
  spBoo = nullptr;
  spBoo2 = nullptr;
  E_ASSERT(spBoo2 == nullptr);
  spIBoo2 = spBoo;
  spIBoo = nullptr;
  E_ASSERT(!spIBoo2.IsUnique());
  E_ASSERT(spIBoo2 == nullptr);
  
  return true;
}

bool Test::SharedPtr::RunPerformanceTest()
{
#ifdef E_CPU_X64
  U32 i = 8;
#else
  U32 i = 4;
#endif
   i +=8;
  try
  {
    std::cout << "[Test::SharedPtr::RunPerformanceTest]" << std::endl;

    std::cout << "ThreadPool Test" << std::endl;
    E::Time::Timer t;
    E::Threads::ThreadPool pool;
    U32 taskCount = 1000;
    E::Containers::List<E::Threads::IRunnable*> barList;
  
    /*-----------------------------------------------------------------
    SharedPtr
    -----------------------------------------------------------------*/
    
    BarPtr barPtr(new Bar());
    E_ASSERT(barPtr.IsUnique());

    // Load Foo thread tasks
    for (U32 i = 0; i < taskCount; ++i)
    {
      BarTask* pTask = new BarTask(barPtr);
      barList.PushBack(pTask);
    }
  
    t.Reset();
    for (E::Containers::List<E::Threads::IRunnable*>::ConstIterator cit = barList.GetBegin(); cit != barList.GetEnd(); ++cit)
    {
      pool.AddItem(*cit);
    }
    pool.WaitForIdle();

    std::cout << "Thread Shared pointer 32 bit test elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms." << std::endl;

    for (E::Containers::List<E::Threads::IRunnable*>::ConstIterator cit = barList.GetBegin(); cit != barList.GetEnd(); ++cit)
    {
      delete (*cit);
    }
    barList.Clear();

    E_ASSERT_MSG(barPtr.IsUnique(), "Reference count should be unique: try a thread-safe reference count method instead!");
    if (!barPtr.IsUnique())
    {
      throw Exception("Reference count should be unique: try a thread-safe reference count method instead!");
    }

    /*-----------------------------------------------------------------
    SharedPtr
    -----------------------------------------------------------------*/
    
    BarPtr64 barPtr64(new Bar());
    E_ASSERT(barPtr64.IsUnique());

    // Load Foo thread tasks
    for (U32 i = 0; i < taskCount; ++i)
    {
      BarTask64* pTask = new BarTask64(barPtr64);
      barList.PushBack(pTask);
    }
  
    t.Reset();
    for (E::Containers::List<E::Threads::IRunnable*>::ConstIterator cit = barList.GetBegin(); cit != barList.GetEnd(); ++cit)
    {
      pool.AddItem(*cit);
    }
    pool.WaitForIdle();

    std::cout << "Thread Shared pointer 64 bit test elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms." << std::endl;

    for (E::Containers::List<E::Threads::IRunnable*>::ConstIterator cit = barList.GetBegin(); cit != barList.GetEnd(); ++cit)
    {
      delete (*cit);
    }
    barList.Clear();

    E_ASSERT_MSG(barPtr.IsUnique(), "Reference count should be unique: try a thread-safe reference count method instead!");
    if (!barPtr.IsUnique())
    {
      throw Exception("Reference count should be unique: try a thread-safe reference count method instead!");
    }

    /*
    E:Atomic vs std::atomic (MSVC 2012 Update 4 implementation) performance comparison:

    X86:                                                                          X64:

    Thread Shared pointer std::atomic 32 bit elapsed time: 91.95 ms.              Thread Shared pointer std::atomic 32 bit elapsed time: 96.131 ms. 
    
    Thread Intrusive pointer 32 bit test elapsed time: 58.275 ms.                 Thread Intrusive pointer 32 bit test elapsed time: 49.632 ms.
    Thread Shared pointer 32 bit test elapsed time: 46.121 ms.                    Thread Shared pointer 32 bit test elapsed time: 46.991 ms.
    Thread Shared pointer 64 bit test elapsed time: 164.384 ms.                   Thread Shared pointer 64 bit test elapsed time: 47.622 ms.
    Succeeded! Elapsed time: 290.01 ms [Test::SharedPtr::RunPerformanceTest]   Succeeded! Elapsed time: 167.12 ms [Test::SharedPtr::RunPerformanceTest]
    
    */
  }
  catch (...)
  {
    return false;
  }
  return true;
}
