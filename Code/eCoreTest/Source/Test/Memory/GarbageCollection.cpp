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

/** @file GCGenericFactory.cpp
This file defines GCGenericFactory test functions.
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
  virtual ~IFoo(){}
  virtual void Print() = 0;
};

class FooA : public IFoo
{
public:
  FooA()
    : mFloatValue(1.0f)
    , mCharValue('a') {}

  void Print() { std::cout << "I am FooA" << std::endl; }

private:
  F32   mFloatValue;
  char  mCharValue;
};

class FooB : public IFoo
{
public:
  FooB()
    : mFloatValue(2.0f)
    , mIntValue(23)
    , mCharValue('b') {}

  void Print() { std::cout << "I am FooB" << std::endl; }

protected:
  F32   mFloatValue;
  I32   mIntValue;
  char  mCharValue;
};

typedef Memory::GCGenericFactory<IFoo>  IFooFactory;
typedef Memory::GCConcreteFactory<FooA> FooAFactory;
typedef IFooFactory::Ref                IFooInstance;
typedef Memory::GCWeakPtr<FooA>             FooAInstance;
typedef Memory::GCWeakPtr<FooB>             FooBInstance;
typedef Memory::GCThisPtr<FooA>       FooAOwner;

class IFooFactoryRegistrar
{
public:
  IFooFactoryRegistrar(IFooFactory& factory)
    : mFactory(factory)
  {
    mFactory.Register(&mAFactory, IFoo::eA);
    mFactory.Register(&mBFactory, IFoo::eB);
  }

  ~IFooFactoryRegistrar()
  {
    mFactory.Unregister(&mBFactory);
    mFactory.Unregister(&mAFactory);
  }

private:
  IFooFactory&							          mFactory;
  Memory::AbstractFactory<IFoo, FooA>	mAFactory;
  Memory::AbstractFactory<IFoo, FooB>	mBFactory;

  E_DISABLE_COPY_AND_ASSSIGNMENT(IFooFactoryRegistrar);
};

struct FooTask : public E::Threads::IRunnable
{
  FooTask(const IFooInstance& foo)
    : foo(foo) {}

  I32 Run()
  {
    std::cout << ".";
    U32 count = Math::Global::GetRandom().GetU32(10000);
    count += 5000;
    for (U32 i = 0; i < count; ++i)
    {
      IFooInstance someFoo = foo;
    }
    foo.Reset();

    return 0;
  }

  IFooInstance foo;
};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::GarbageCollection::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::GarbageCollection::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::GarbageCollection::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::GarbageCollection::RunFunctionalityTest()
{
  std::cout << "[Test::GarbageCollection::RunFunctionalityTest]" << std::endl;

  // GCThisPtr
  {
    FooA aFoo;
    FooAOwner fooOwner(&aFoo);
    IFooInstance foo = fooOwner;
    foo->Print();
  }

  // GCThisPtr inverse order definition
  {
    FooA aFoo;
    IFooInstance foo;
    FooAOwner fooOwner(&aFoo);
    foo = fooOwner;
    foo->Print();
  }

  // GCThisPtr inverse order definition
  {
    IFooInstance foo;
    FooA aFoo;
    FooAOwner fooOwner(&aFoo);
    foo = fooOwner;
    foo->Print();
  }

  // GCConcreteFactory
  {
    FooAFactory fooAFactory;
    IFooInstance foo = fooAFactory.Create();
    //E_ASSERT(sizeof(foo) == sizeof(IFoo*)); // same size as a raw pointer!!

    foo->Print();
    fooAFactory.CleanUp();
    E_ASSERT(foo == nullptr);

    // Null comparison and assignment
    {
      IFooInstance anotherFoo, anotherEmptyFoo;
      IFooInstance yetAnotherFoo = fooAFactory.Create();
      // Null comparison
      E_ASSERT_NULL(anotherFoo);

      // Empty reference comparison
      anotherFoo = anotherEmptyFoo;
      E_ASSERT(anotherFoo == anotherEmptyFoo);
      E_ASSERT(anotherEmptyFoo == anotherFoo);

      // Invalid reference to empty reference comparison
      E_ASSERT(anotherFoo == foo);
      E_ASSERT(foo == anotherFoo);

      // Empty to valid reference comparison
      E_ASSERT(anotherFoo != yetAnotherFoo);
      E_ASSERT(yetAnotherFoo != anotherFoo);

      // Invalid reference assignment comparison
      anotherFoo = foo;
      E_ASSERT(anotherFoo == foo);
      E_ASSERT(foo == anotherFoo);
    }
    // Type conversion
    {
      IFooInstance foo = fooAFactory.Create();
      FooAInstance aFoo = foo;
      aFoo->Print();
      foo.Reset();
      aFoo->Print();
      foo = aFoo;
      foo->Print();
      foo = nullptr;
    }
    E_ASSERT(fooAFactory.GetLiveCount() == 0);
  }

  // GCGenericFactory
  {
    IFooFactory fooFactory;
    IFooFactoryRegistrar fooRegistrar(fooFactory);
    IFooInstance foo = fooFactory.Create(IFoo::eA);

    foo->Print();
    fooFactory.CleanUp();
    E_ASSERT(foo == nullptr);

    {
      IFooInstance foo = fooFactory.Create(IFoo::eA);
      FooAInstance aFoo = foo;
      aFoo->Print();
      foo.Reset();
      aFoo->Print();
      foo = aFoo;
      foo = fooFactory.Create(IFoo::eB);
      foo->Print();
      FooBInstance bFoo = foo;
      bFoo->Print();
    }
    E_ASSERT(fooFactory.GetLiveCount() == 0);
  }

  return true;
}

bool Test::GarbageCollection::RunPerformanceTest()
{
  std::cout << "[Test::SharedPtr::RunPerformanceTest]" << std::endl;

  // GCConcreteFactory
  {
    E::Threads::ThreadPool pool;
    U32 taskCount = 1000;
    E::Containers::List<FooTask*> fooTaskList;
    FooAFactory fooAFactory;

    // Create tasks
    for (U32 i = 0; i < taskCount; ++i) fooTaskList.PushBack(new FooTask(fooAFactory.Create()));

    // Add tasks to pool
    for (auto it = begin(fooTaskList); it != end(fooTaskList); ++it) pool.AddItem(*it);

    // Wait for task completion
    pool.WaitForIdle();

    // Destroy tasks
    for (auto it = begin(fooTaskList); it != end(fooTaskList); ++it) delete *it;
  }

  // GCGenericFactory
  {
    E::Threads::ThreadPool pool;
    U32 taskCount = 1000;
    E::Containers::List<FooTask*> fooTaskList;
    IFooFactory fooFactory;
    IFooFactoryRegistrar fooRegistrar(fooFactory);

    // Create tasks
    for (U32 i = 0; i < taskCount; ++i) fooTaskList.PushBack(new FooTask(fooFactory.Create(IFoo::eA)));

    // Add tasks to pool
    for (auto it = begin(fooTaskList); it != end(fooTaskList); ++it) pool.AddItem(*it);

    // Wait for task completion
    pool.WaitForIdle();

    // Destroy tasks
    for (auto it = begin(fooTaskList); it != end(fooTaskList); ++it) delete *it;
  }

  return true;
}
