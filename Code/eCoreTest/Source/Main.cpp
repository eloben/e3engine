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

/** @file Main.cpp
This file defines the application main entry point.
*/

#include <CoreTestPch.h>
#include <SafeCast.h>


using namespace E;

void SafeCastTest()
{
  struct A
  {
    virtual ~A() {}
  };

  struct B : public A
  {
    int b;
  };

  struct C : public A
  {
    int c;
  };

  C* c = new C();
  A* a = c;

  c = E::SafeCast<C>(a);     // ok
  B* b = static_cast<B*>(a);  // passes
  b = E::SafeCast<B>(a);     // does not pass
}

void main()
{
	try
  {
    Test::Array::Run();
    Test::DynamicArray::Run();
    Test::List::Run();
    Test::String::Run();
    Test::StringBuffer::Run();
    Test::Hash::Run();
    Test::Map::Run();
    Test::Queue::Run();
    Test::Stack::Run();
    Test::Time::Run();
    Test::Vector::Run();
    Test::Matrix::Run();
    Test::Quaternion::Run();
    Test::Serialization::Run();
    Test::Thread::Run();
    Test::Event::Run();
    Test::SharedPtr::Run();
    Test::IntrusivePtr::Run();
    Test::Factory::Run();
    Test::Allocator::Run();
    Test::Algorithm::Run();
    Test::File::Run();
    Test::WeakPtr::Run();
    Test::GarbageCollection::Run();
    Test::ConditionVariable::Run();

    // VLD leak test (comment out to catch actual memory leaks)
    //int* pVldLeakTest = E_NEW(int);

    //SafeCastTest();

    struct Foo
    {
      int a;
    };

    Containers::List<Foo> fooList;
    fooList.PushBack(Foo());

    try
    {
      throw Exception(Exception::eExceptionTypeCustom);
    }
    catch(Exception& testException)
    {
      E_ASSERT(testException.type == Exception::eExceptionTypeCustom);
      std::cout << "Custom exception [" << testException.type << "] test Ok!" << std::endl;
    }

    //std::cin.get();    
	}
	catch(Exception& ex)
	{
		std::cout << "Woooops! Something needs to be debugged: " << ex.description.GetPtr() << std::endl;
	}

}
