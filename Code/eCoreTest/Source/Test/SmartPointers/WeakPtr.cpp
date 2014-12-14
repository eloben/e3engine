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

/** @file WeakPtr.cpp
This file defines WeakPtr test functions.
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

class FooB : public IFoo
{
public:
  FooB()
    : mFloatValue(2.0f)
    , mIntValue(23)
    , mCharValue('b')
  {

  }

  void Print()
  {
    std::cout << "I am FooB" << std::endl;
  }

protected:
  F32   mFloatValue;
  I32   mIntValue;
  char  mCharValue;

};

typedef Memory::GenericFactory<IFoo> IFooFactory;

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

typedef E::WeakPtr<IFoo, E::Threads::Atomic<U32>> IFooInstance;   // Thread-safe reference count
typedef E::WeakPtr<FooA, E::Threads::Atomic<U32>> FooAInstance;   // Thread-safe reference count
typedef E::WeakPtr<FooB, E::Threads::Atomic<U32>> FooBInstance;   // Thread-safe reference count

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::WeakPtr::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::WeakPtr::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::WeakPtr::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::WeakPtr::RunFunctionalityTest()
{
  std::cout << "[Test::WeakPtr::RunFunctionalityTest]" << std::endl;

  // Standard SharedPtr usage:
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA;
    spFooA = pFooA;
    E::WeakPtr<FooA> spWeakFooA;
    spWeakFooA = spFooA;
    spWeakFooA->Print();
  }

  // WeakPtr validity:
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA;
    spFooA = pFooA;
    E::WeakPtr<FooA> spWeakFooA;
    spWeakFooA = spFooA;
    spWeakFooA->Print();
    spFooA.Reset();
    E_ASSERT(!spWeakFooA.IsValid());
  }

  // Direct construction 
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA(pFooA);
    E::WeakPtr<FooA> spWeakFooA(spFooA);
  }

  // Copy construction SharedPtr usage:
  {
    FooA* pFooA = E_NEW(FooA);
    E::SharedPtr<FooA> spFooA;
    spFooA = pFooA;
    E::WeakPtr<FooA> spWeakFooA;
    spWeakFooA = spFooA;
    spWeakFooA->Print();
    E::WeakPtr<FooA> spAnotherWeakFooA(spWeakFooA);
    spAnotherWeakFooA->Print();
  }

  // Standard WeakPtr usage: trigger SharedPtr destruction
  {
    FooA* pFooA = E_NEW(FooA);

    E::SharedPtr<IFoo> spIFoo;
    E::SharedPtr<FooA> spFooA;

    spFooA = pFooA;
    spIFoo = spFooA;

    E::WeakPtr<FooA> spWeakFooA;
    spWeakFooA = spFooA;
    spWeakFooA->Print();
  }
  
  // Standard WeakPtr usage: trigger WeakPtr destruction
  {
    FooA* pFooA = E_NEW(FooA);

    E::WeakPtr<FooA> spWeakFooA;
    E::SharedPtr<IFoo> spIFoo;
    E::SharedPtr<FooA> spFooA;

    spFooA = pFooA;
    spIFoo = spFooA;
    spWeakFooA = spFooA;
    spWeakFooA->Print();
  }

  // WeakPtr to WeakPtr conversion
  {
    FooA* pFooA = E_NEW(FooA);

    E::WeakPtr<FooA> spWeakFooA, spWeakAnotherFooA;
    E::WeakPtr<IFoo> spWeakIFoo;
    E::SharedPtr<IFoo> spIFoo;
    E::SharedPtr<FooA> spFooA;

    spFooA = pFooA;
    spIFoo = spFooA;
    spWeakFooA = spFooA;
    spWeakFooA->Print();
    spWeakIFoo = spIFoo;
    spWeakIFoo->Print();
    spWeakIFoo = spWeakFooA;
    spWeakIFoo->Print();
    spWeakAnotherFooA = spWeakIFoo;
    spWeakAnotherFooA->Print();
  }

  // SharedPtr to WeakPtr conversion
  {
    FooA* pFooA = E_NEW(FooA);

    E::WeakPtr<FooA> spWeakFooA, spWeakAnotherFooA;
    E::WeakPtr<IFoo> spWeakIFoo;
    E::SharedPtr<IFoo> spIFoo;
    E::SharedPtr<FooA> spFooA;

    spFooA = pFooA;
    spIFoo = spFooA;
    spWeakIFoo = spFooA;
    spWeakIFoo->Print();
    spWeakFooA = spIFoo;
    spWeakFooA->Print();
  }

  // Custom WeakPtr
  {
    FooA* pFooA = E_NEW(FooA);
    E::WeakPtr<IFoo, A32> spWeakIFoo;
    E::WeakPtr<FooA, A32> spWeakFooA;
    E::SharedPtr<IFoo, A32> spIFoo;
    E::SharedPtr<FooA, A32> spFooA;

    spFooA = pFooA;
    spIFoo = spFooA;
    spWeakIFoo = spFooA;
    spWeakIFoo->Print();
    spWeakFooA = spIFoo;
    spWeakFooA->Print();
  }

  // Custom WeakPtr II
  {
    FooA* pFooA = E_NEW(FooA);
    E::WeakPtr<IFoo, A32> spWeakIFoo;
    E::WeakPtr<FooA, A32> spWeakFooA;
    E::SharedPtr<IFoo, A32, Memory::Deleter<IFoo>> spIFoo;
    E::SharedPtr<FooA, A32, Memory::Deleter<IFoo>> spFooA;

    spFooA = pFooA;
    spIFoo = spFooA;
    spWeakIFoo = spFooA;
    spWeakIFoo->Print();
    spWeakFooA = spIFoo;
    spWeakFooA->Print();
  }

  return true;
}


bool Test::WeakPtr::RunPerformanceTest()
{
  std::cout << "[Test::SharedPtr::RunPerformanceTest]" << std::endl;

  return true;
}
