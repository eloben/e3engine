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

/** @file DynamicArray.cpp
This file defines DynamicArray test functions.
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

struct ArrayPod
{
  char c;
  int i;
  float f;
};

E_DECLARE_POD(ArrayPod)

struct ArrayNonPod
{
  char c;
  int i;
  float f;
};

/*----------------------------------------------------------------------------------------------------------------------
TestArray methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::DynamicArray::Run()
{
  try
  {
    std::cout << "[Test::DynamicArray::Run] using array size of " << TEST_SIZE << std::endl;
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::DynamicArray::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::DynamicArray::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::DynamicArray::RunFunctionalityTest()
{
  try
  {
    std::cout << "[Test::DynamicArray::RunFunctionalityTest]" << std::endl;

    /*
    DynamicArray();
    DynamicArray(const DynamicArray& other);
    explicit DynamicArray(size_t size);
    DynamicArray(const T* pData, size_t count);
    ~DynamicArray();

    DynamicArray&            operator=(const DynamicArray& other);
    const T&          operator [] (size_t index) const;
    T&                operator [] (size_t index);
    */
    E::Containers::DynamicArray<I32> intArray(TEST_SIZE);
    E::Containers::DynamicArray<I32> intArray2;

    // T& operator [] (U32 index);
    for (U32 i = 0; i < TEST_SIZE; ++i)
      intArray[i] = i;

    intArray2 = intArray;

    // const T& operator [] (U32 index) const;
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(intArray[i] == intArray2[i]);
    }

    E::Containers::DynamicArray<I32> intArray3(intArray.GetPtr(), intArray.GetSize());
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(intArray[i] == intArray3[i]);
    }
    E_ASSERT(intArray3 == intArray);

    E::Containers::DynamicArray<I32> intArray4(intArray2);
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(intArray2[i] == intArray4[i]);
    }
    E_ASSERT(intArray4 == intArray2);

    E::Containers::DynamicArray<I32> intArray5(TEST_SIZE_SQRT);
    intArray5.SetZero();
    intArray4 = intArray5;
    E_ASSERT(intArray4.GetSize() == intArray5.GetSize());
    for (U32 i = 0; i < intArray4.GetSize(); ++i)
    {
      E_ASSERT(intArray4[i] == intArray5[i]);
    }
    E_ASSERT(intArray4 == intArray5);

    intArray.SetAllocator(Memory::Global::GetAllocator());
    intArray4 = intArray;
    E_ASSERT(intArray4.GetSize() == intArray.GetSize());
    for (U32 i = 0; i < intArray4.GetSize(); ++i)
    {
      E_ASSERT(intArray4[i] == intArray[i] && intArray4[i] == (I32)i);
    }
    E_ASSERT(intArray4.GetAllocator() == Memory::Global::GetAllocator());
    E_ASSERT(intArray4 == intArray);

    /*
    bool              operator==(const DynamicArray<T>& other) const; (already tested)
    bool              operator==(const T* pPtr) const;    
    bool              operator!=(const DynamicArray<T>& other) const;
    bool              operator!=(const T* pPtr) const;
    */
    I32* ptr1 = nullptr;
    E_ASSERT(intArray != ptr1);
    ptr1 = new I32[TEST_SIZE];
    E_ASSERT(intArray != ptr1);
    Memory::Copy(ptr1, intArray.GetPtr(), TEST_SIZE);
    E_ASSERT(intArray == ptr1);
    delete[] ptr1;
    ptr1 = nullptr;
    E_ASSERT(intArray != ptr1);
    E::Containers::DynamicArray<I32>().Swap(intArray);
    E_ASSERT(intArray == ptr1);
    if (intArray != ptr1)
    {
      throw Exception("Bad alloc");
    }
     
    /*
    const IAllocator* GetAllocator() const;
    size_t		        GetByteSize() const;
    const T*	        GetPtr() const;       (already tested)
    T*			          GetPtr();             (already tested)
    size_t            GetSize() const;      (already tested)
    void              SetAllocator(IAllocator* p);
    void              SetZero();
    */
    
    intArray = intArray2;
    E_ASSERT(intArray.GetByteSize() == TEST_SIZE * sizeof(intArray[0]));

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

    /*
    void              Copy(const T* pData, size_t count, size_t startIndex = 0);
    void              Reserve(size_t size);   (already tested)
    void              Swap(DynamicArray& other);     (already tested)
    */
    //  void      Copy(const T* pData, U32 size, U32 startIndex = 0);
    
    // Manual copy
    intArray.Reserve(intVector.size());
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      intArray[i] = intVector[i];
    }
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(intArray[i] == intVector[i]);
    }
    
    // Constructor copy
    E::Containers::DynamicArray<I32>().Swap(intArray);
    intArray = E::Containers::DynamicArray<I32>(&intVector[0], TEST_SIZE);
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(intArray[i] == intVector[i]);
    }

    // Copy
    intArray.Reserve(intVector.size());
    intArray.Copy(&intVector[0], intVector.size());
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(intArray[i] == intVector[i]);
    }
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool Test::DynamicArray::RunPerformanceTest()
{
  try
  {
    std::cout << "[Test::DynamicArray::RunPerformanceTest]" << std::endl;
  
    E::Containers::DynamicArray<ArrayPod> podArray(TEST_SIZE);
    E::Containers::DynamicArray<ArrayNonPod> nonPodArray(TEST_SIZE);

    ArrayPod pods[TEST_SIZE];
    ArrayNonPod nonPods[TEST_SIZE];

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
    Test::PrintTimeAndReset(t, "nonPodArray.Copy");

    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(nonPodArray[i].c == nonPods[i].c);
      E_ASSERT(nonPodArray[i].i == nonPods[i].i);
      E_ASSERT(nonPodArray[i].f == nonPods[i].f);
    }

    t.Reset();
    podArray.Copy(pods, TEST_SIZE);
    Test::PrintTimeAndReset(t, "podArray.Copy");

    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E_ASSERT(podArray[i].c == pods[i].c);
      E_ASSERT(podArray[i].i == pods[i].i);
      E_ASSERT(podArray[i].f == pods[i].f);
    }

    t.Reset();
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      I32* p = new I32[i];
      delete[] p;
    }
    Test::PrintTimeAndReset(t, "Manual allocation");
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E::Containers::DynamicArray<I32> a(i);
    }
    Test::PrintTimeAndReset(t, "DynamicArray allocation");
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      I32* p = new I32[i];
      I32* p2 = new I32[i];
      I32* pTmp = p;
      p = p2;
      p2 = pTmp;
      pTmp = nullptr;
      delete[] p;
      delete[] p2;
    }
    Test::PrintTimeAndReset(t, "Manual allocation & swap");
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      E::Containers::DynamicArray<I32> a(i);
      E::Containers::DynamicArray<I32> b(i);
      a.Swap(b);
    }
    Test::PrintTimeAndReset(t, "DynamicArray allocation & swap");
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
