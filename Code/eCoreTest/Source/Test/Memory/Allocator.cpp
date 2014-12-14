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

// Created 17-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Allocator.cpp
This file defines Allocator test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

class MyAllocator : public Memory::IAllocator
{
public:
  inline MyAllocator::MyAllocator() : mAllocationCount(0) {}

  inline void* Allocate(size_t size, const Tag = IAllocator::eTagNew)
  {
    if (size)
    {
      mAllocationCount++;
    }
    
    return E::Memory::Heap::Allocate(size);
  }

  inline void Deallocate(void* p, const Tag = IAllocator::eTagDelete)
  {
    if (p)
    {
      E::Memory::Heap::Deallocate(p);
      mAllocationCount--;
    }
  }

  U32 GetAllocationCount()
  {
    return mAllocationCount;
  }

private:
  U32 mAllocationCount;

  E_DECLARE_SINGLETON_FRIENDLY(MyAllocator)
};

typedef E::Singleton<MyAllocator> GMyAllocator;


/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Allocator::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Allocator::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Allocator::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Allocator::RunFunctionalityTest()
{
  std::cout << "[Test::Allocator::RunFunctionalityTest]" << std::endl;
  
  try
  {
    E::Memory::Global::SetAllocator(&GMyAllocator::GetInstance()); // Comment to use the default global allocator
    {
      E::Threads::ThreadPool pool;
      E::Serialization::XmlSerializer xmlSerializer;
      I32* p = E::Memory::Create<I32>();
      std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
      *p = 1;
      E::Containers::DynamicArray<I32> a(1);
      std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
      I32* p2 = reinterpret_cast<I32*>(GMyAllocator::GetInstance().Allocate(sizeof(I32)));
      std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
      *p2 = 2;
      I32* p3 = reinterpret_cast<I32*>(E::Memory::Global::GetAllocator()->Allocate(sizeof(I32), Memory::IAllocator::eTagNew));
      std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
      *p3 = 3;

      GMyAllocator::GetInstance().Deallocate(p2);
      std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
      E::Memory::Global::GetAllocator()->Deallocate(p3, Memory::IAllocator::eTagDelete);
      std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
      E::Memory::Destroy(p);
      std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
    }
    std::cout << "MyAllocator allocations: " << GMyAllocator::GetInstance().GetAllocationCount() << std::endl;
    E::Memory::Global::SetDefaultAllocator();
  }
  catch (const E::Exception& e)
  {
    std::cout << e.description.GetPtr() << std::endl;

    return false;
  }

  return true;
}

bool Test::Allocator::RunPerformanceTest()
{
  std::cout << "[Test::Allocator::RunPerformanceTest]" << std::endl;

  return true;
}
