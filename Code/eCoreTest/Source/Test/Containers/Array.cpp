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

/** @file Array.cpp
This file defines Array test functions.
*/

#include <CoreTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

#ifndef TEST_SMALL_SIZE
#define TEST_SMALL_SIZE 256
#endif

#ifndef TEST_SMALL_SIZE_SQRT
#define TEST_SMALL_SIZE_SQRT 16
#endif

#ifndef TEST_BIG_SIZE
#define TEST_BIG_SIZE 4096
#endif

#ifndef TEST_BIG_SIZE_SQRT
#define TEST_BIG_SIZE_SQRT 64
#endif

#ifndef TEST_SIZE
#define TEST_SIZE TEST_BIG_SIZE
#endif

#ifndef TEST_SIZE_SQRT
#define TEST_SIZE_SQRT TEST_BIG_SIZE_SQRT
#endif

bool RunCopyPerformanceTest();

struct StaticArrayPod
{
  char c;
  int i;
  float f;
};

E_DECLARE_POD(StaticArrayPod)

struct StaticArrayNonPod
{
  char c;
  int i;
  float f;
};

/*----------------------------------------------------------------------------------------------------------------------
TestArray methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Array::Run()
{
  try
  {
    std::cout << "[Test::Array::Run] using array size of " << TEST_SIZE << std::endl;
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Array::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Array::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Array::RunFunctionalityTest()
{
  try
  {
    std::cout << "[Test::Array::RunFunctionalityTest]" << std::endl;

    E::Containers::Array<I32, TEST_SIZE> intArray;
    E::Containers::Array<E::Containers::Array<I32, TEST_SIZE_SQRT>, TEST_SIZE_SQRT> intArray2D;

    // T& operator [] (U32 index);
    for (U32 i = 0; i < TEST_SIZE; ++i)
      intArray[i] = i;

    for (U32 i = 0; i < TEST_SIZE_SQRT; ++i)
      for (U32 j = 0; j < TEST_SIZE_SQRT; ++j)
        intArray2D[i][j] = i + j;

    // const T& operator [] (U32 index) const;
    for (U32 i = 0; i < TEST_SIZE_SQRT; ++i)
    {
      for (U32 j = 0; j < TEST_SIZE_SQRT; ++j)
      {  
        E_ASSERT(intArray[i + j] == intArray2D[i][j]);
      }
    }

    //U32			  GetByteSize() const;
    E_ASSERT(intArray.GetByteSize() == TEST_SIZE * sizeof(I32));

    // const T*	GetPtr() const;
    std::vector<I32> intVector;
    intVector.resize(TEST_SIZE);
    E::Memory::Copy(&intVector[0], intArray.GetPtr(), TEST_SIZE);

    // T*			  GetPtr();
    for (U32 i = 0; i < TEST_SIZE; ++i)
      intArray.GetPtr()[i] = TEST_SIZE - i;

    // U32       GetSize() const;
    E_ASSERT(intArray.GetSize() == TEST_SIZE);

    // void      SetZero();
    intArray.SetZero();
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {      
      E_ASSERT(intArray[i] == 0);
    }

    //  void      Copy(const T* pData, U32 size, U32 startIndex = 0);
    intArray.Copy(&intVector[0], intVector.size());
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(intArray[i] == intVector[i]);
    }

    //  U32       TryCopy(const T* pData, U32 size, U32 startIndex = 0);
    intArray.SetZero();
    E_ASSERT(intArray.TryCopy(&intVector[0], intVector.size()) == TEST_SIZE);
    E_ASSERT(intArray.TryCopy(&intVector[0], intVector.size(), TEST_SIZE) == 0);
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool Test::Array::RunPerformanceTest()
{
  try
  {
    std::cout << "[Test::Array::RunPerformanceTest]" << std::endl;
  
    E::Containers::Array<StaticArrayPod, TEST_SIZE> podArray;
    E::Containers::Array<StaticArrayNonPod, TEST_SIZE> nonPodArray;

    StaticArrayPod pods[TEST_SIZE];
    StaticArrayNonPod nonPods[TEST_SIZE];

    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      pods[i].c = (char) i;
      pods[i].i = i;
      pods[i].f = 13.0f * i;

      nonPods[i].c = (char) i;
      nonPods[i].i = i;
      nonPods[i].f = 13.0f * i;
    }

    E::Time::Timer t;
    nonPodArray.Copy(nonPods, TEST_SIZE);
    //nonPodArray.Copy(nonPods, TEST_SIZE); // will not compile
    Test::PrintTimeAndReset(t, "nonPodArray.Copy");

    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(nonPodArray[i].c == nonPods[i].c);
      E_ASSERT(nonPodArray[i].i == nonPods[i].i);
      E_ASSERT(nonPodArray[i].f == nonPods[i].f);
    }

    podArray.Copy(pods, TEST_SIZE);
    Test::PrintTimeAndReset(t, "podArray.Copy");

    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(podArray[i].c == pods[i].c);
      E_ASSERT(podArray[i].i == pods[i].i);
      E_ASSERT(podArray[i].f == pods[i].f);
    } 
  }
  catch (...)
  {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/
