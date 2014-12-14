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
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR FooA PARTICULAR 
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

/** @file Factory.cpp
This file defines Factory test functions.
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
    eC,
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

class FooC : public FooB
{
public:
  FooC()
  {
    mFloatValue = 4.0f;
    mIntValue = 2000;
    mCharValue = 'c';
  }

  void Print()
  {
    std::cout << "I am FooC" << std::endl;
  }
};

typedef Memory::GenericFactory<IFoo> IFooFactory;

class FooFactoryRegistrar
{
public:
  FooFactoryRegistrar(IFooFactory& factory)
    : mFactory(factory)
  {
    mFactory.Register(&mAFactory, IFoo::eA);
    mFactory.Register(&mBFactory, IFoo::eB);
    mFactory.Register(&mCFactory, IFoo::eC);
  }

  ~FooFactoryRegistrar()
  {
    mFactory.Unregister(&mCFactory);
    mFactory.Unregister(&mBFactory);
    mFactory.Unregister(&mAFactory);
  }

private:
  IFooFactory&							  mFactory;
  Memory::AbstractFactory<IFoo, FooA>	mAFactory;
  Memory::AbstractFactory<IFoo, FooB>	mBFactory;
  Memory::AbstractFactory<IFoo, FooC>	mCFactory;

  E_DISABLE_COPY_AND_ASSSIGNMENT(FooFactoryRegistrar);
};

typedef SharedPtr<IFoo> IFooPtr;
typedef SharedPtr<FooA> FooAPtr;

class FooManager
{
public:
  FooManager() : mFooRegistrar(mFooFactory) {}

  IFooFactory& GetFooFactory() 
  {
    return mFooFactory;
  }

  IFooPtr Create(IFoo::Type id)
  {
    return IFooPtr(mFooFactory.Create(id));
  }
  void Destroy(IFooPtr& ptr)
  {
    E_ASSERT_MSG(ptr.IsUnique(), "Attempting to destroy a referenced resource. Ensure smart pointer is unique.");
    mFooFactory.Destroy(&*ptr);
    ptr.RemoveOwnership();
  }
private:
  IFooFactory mFooFactory;
  FooFactoryRegistrar mFooRegistrar;
};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Factory::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Factory::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Factory::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Factory::RunFunctionalityTest()
{
  std::cout << "[Test::Factory::RunFunctionalityTest]" << std::endl;
  
  try
  {
    IFooFactory fooFactory;
    FooFactoryRegistrar fooRegistrar(fooFactory);

    IFoo* pA = fooFactory.Create(IFoo::eA);
    pA->Print();

    IFoo* pB = fooFactory.Create(IFoo::eB);
    pB->Print();

    IFoo* pC = fooFactory.Create(IFoo::eC);
    pC->Print();

    fooFactory.Destroy(pC);
    fooFactory.Destroy(pB);
    fooFactory.Destroy(pA);
 
    for (U32 i = 0; i < 100; ++i)
    {
      fooFactory.Create(Math::Global::GetRandom().GetU32(IFoo::eTypeCount))->Print();
    }

    fooFactory.CleanUp();

    // Manager test (SharedPtr in combination with GenericFactory test)
    FooManager fooManager;

    // Test
    IFooPtr sp1 = fooManager.Create(IFoo::eA);
    IFooPtr sp2 = sp1;
    //fooManager.Destroy(sp1);  // Comment out: it will assert because sp1 is not unique!
    sp1 = nullptr;
    //sp2 = nullptr;               // Comment out: it will assert as sp2 is void!
    fooManager.Destroy(sp2);

    // Style 1 extended using intermediate child type smart pointers
    IFooPtr sp3 = fooManager.Create(IFoo::eA);
    FooAPtr sp4 = sp3;
    sp3 = nullptr;
    sp3 = sp4;
    sp4 = nullptr;
    fooManager.Destroy(sp3);

    // Style 2 extended using directly child type smart pointers
    FooAPtr sp5 = fooManager.Create(IFoo::eA);
    IFooPtr sp6(sp5);
    sp5 = nullptr;
    fooManager.Destroy(sp6);
  }
  catch (const E::Exception& e)
  {
    std::cout << e.description.GetPtr() << std::endl;

    return false;
  }

  return true;
}

bool Test::Factory::RunPerformanceTest()
{
  std::cout << "[Test::Factory::RunPerformanceTest]" << std::endl;

  return true;
}
