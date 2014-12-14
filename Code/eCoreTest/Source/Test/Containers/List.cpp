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

/** @file E::Containers::List.cpp
This file defines E::Containers::List test functions.
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

F32 TimeFind(U32 count, const E::Containers::List<I32>& intList);
F32 TimeFind(U32 count, const std::vector<I32>& intList);
F32 TimeInsert(U32 count, E::Containers::List<I32>& intList);
F32 TimeInsert(U32 count, std::vector<I32>& intList);
F32 TimeRemove(U32 count, E::Containers::List<I32>& intList);
F32 TimeRemove(U32 count, std::vector<I32>& intList);
F32 TimeRemoveFast(U32 count, E::Containers::List<I32>& intList);

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::List::Run()
{
  try
  {
    std::cout << "[Test::List::Run] using array size of " << TEST_SIZE << std::endl;
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::List::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::List::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

class FooList
{
public:
  static FooList& GetInstance()
  {
    static FooList sFoo;
    return sFoo;
  }

  const E::Containers::List<I32, 8>& GetList()
  {
    return mList;
  }

private:
  E::Containers::List<I32, 8> mList;

  FooList()
    : mList(16, 8)
  {
    mList.PushBack(0);
  }
};

bool Test::List::RunFunctionalityTest()
{
  try
  {
    std::cout << "[Test::List::RunFunctionalityTest]" << std::endl;

    /*-----------------------------------------------------------------
    Construction
    -----------------------------------------------------------------*/
    // E::Containers::List();
    E::Containers::List<I32> intList;
    E_ASSERT(intList.GetSize() == 0 && intList.GetCount() == 0);
    E::Containers::List<E::Containers::List<I32>> intList2D;

    // explicit E::Containers::List(U32 size);
    E::Containers::List<I32> preSizedIntList(TEST_SIZE);
    E_ASSERT(preSizedIntList.GetSize() == TEST_SIZE && preSizedIntList.GetCount() == 0);
   
    // E::Containers::List(U32 size, U32 granularity, F32 growthFactor);
    E::Containers::List<I32> customIntList(TEST_SIZE);
    E_ASSERT(customIntList.GetSize() == TEST_SIZE && customIntList.GetGranularity() == 1 && customIntList.GetGrowthPercentage() == 50);

    // E::Containers::List(U32 size, const T& value);
    E::Containers::List<I32> zeroList(TEST_SIZE, 0);
    for (U32 i = 0; i < zeroList.GetSize(); ++i)
    {
      E_ASSERT(zeroList[i] == 0);
    }
    E_ASSERT(zeroList.GetSize() == TEST_SIZE && zeroList.GetCount() == zeroList.GetSize());
    
    //  E::Containers::List(const E::Containers::List<T, AllocatorClass>& other);
    E::Containers::List<I32> copyIntList(zeroList);
    for (U32 i = 0; i < copyIntList.GetSize(); ++i)
    {
      E_ASSERT(copyIntList[i] == 0);
    }
    E_ASSERT(copyIntList.GetSize() == zeroList.GetSize() && copyIntList.GetCount() == zeroList.GetCount());

    // E::Containers::List(const T* pData, U32 count);
    std::vector<I32> intVector(10, 5);
    E::Containers::List<I32> anotherCopyIntList(&intVector[0], intVector.size());
    for (U32 i = 0; i < anotherCopyIntList.GetSize(); ++i)
    {
      E_ASSERT(anotherCopyIntList[i] == intVector[i]);
    }

    /*-----------------------------------------------------------------
    Operators
    -----------------------------------------------------------------*/
    /*
    std::vector<I32> a(10, 5);
    std::vector<I32> b(20, 5);
    std::vector<I32> c(30, 5);

    a = b;
    c = a;
    */
    // E::Containers::List<T, AllocatorClass>& operator=(const E::Containers::List<T, AllocatorClass>& other);
    //E::Containers::List<I32, 8> a(10, 1); // Alignment assertion
    E::Containers::List<I32, 8> a(16, 1);
    E::Containers::List<I32, 8> b(24, 2);
    E::Containers::List<I32, 8> c(32, 3);

    b = FooList::GetInstance().GetList();
    E_ASSERT(b.GetCount() == FooList::GetInstance().GetList().GetCount());
    E_ASSERT(b.GetSize() == FooList::GetInstance().GetList().GetSize());

    b = a;
    E_ASSERT(b.GetCount() == a.GetCount());
    E_ASSERT(b.GetSize() == a.GetSize());
    E_ASSERT(b.GetGranularity() == a.GetGranularity());
    E_ASSERT(b.GetGrowthPercentage() == a.GetGrowthPercentage());
    for (U32 i = 0; i < a.GetSize(); ++i)
    {
      E_ASSERT(a[i] == b[i]);
    }

    c = b;
    for (U32 i = 0; i < c.GetSize(); ++i)
    {
      E_ASSERT(c[i] == b[i]);
    }

    /*-----------------------------------------------------------------
    Aligned Compact
    -----------------------------------------------------------------*/
    c.PopBack(2);
    c.Compact();
    E_ASSERT(c.GetSize() == 16);
    c.PopBack(10);
    c.Compact();
    E_ASSERT(c.GetSize() == 8);

    // T& operator [] (U32 index);
    // const T& operator [] (U32 index) const;
    intList.Reserve(TEST_SIZE);
    for (U32 i = 0; i < intList.GetSize(); ++i)
    {
      intList.PushBack(i);
      E_ASSERT(intList[i] == static_cast<I32>(i));
    }

    /*-----------------------------------------------------------------
    Accessors
    -----------------------------------------------------------------*/

    // Iterator      GetBegin() const;
    // U32           GetCount() const; (already tested)
    // Iterator      GetEnd() const;
    U32 intListElementCount = 0;
    for (E::Containers::List<I32>::ConstIterator cit = intList.GetBegin(); cit != intList.GetEnd(); ++cit)
    {
        E_ASSERT(*cit == intList[intListElementCount]);
        intListElementCount ++;
    }
    E_ASSERT(intListElementCount == intList.GetSize());

    // U32           GetGranularity() const; (already tested)
    // F32           GetGrowthPercentage() const; (already tested)

    // ConstIterator GetNext(ConstIterator cit) const;
    // Iterator      GetNext(Iterator it) const;
    // T             GetValue(Iterator it) const;
    // T             GetValue(ConstIterator cit) const;
    for (E::Containers::List<I32>::Iterator it = intList.GetBegin(); it != intList.GetEnd(); ++it)
    {
      *it = intListElementCount;
      E_ASSERT(*it == static_cast<I32>(intListElementCount));
      intListElementCount --;
    }
    
    for (E::Containers::List<I32>::ConstIterator cit = intList.GetBegin(); cit != intList.GetEnd(); ++cit)
    {
      E_ASSERT(*cit == static_cast<I32>(intList.GetCount() - intListElementCount));
      intListElementCount ++;
    }
    
    // const T*      GetPtr() const; (already tested)
    // T*            GetPtr(); (already tested)
    // U32           GetSize() const; (already tested)
       
    // bool          Has(const T& value) const;
    E_ASSERT(intList.HasValue(intListElementCount - TEST_SIZE / 2));
    E_ASSERT(!intList.HasValue(0));
    
    // bool         IsEmpty() const;
    E_ASSERT(!intList.IsEmpty());    // we are using intList as an array (only [] use, no push back)
    E_ASSERT(!zeroList.IsEmpty());  // zero list has been filled
    E_ASSERT(preSizedIntList.IsEmpty());  // preSizedIntList list has been allocated but not filled

    // bool          IsValid(ConstIterator cit) const;
    // bool          IsValid(Iterator it) const;
    // bool          IsValid(U32 index) const;
    E_ASSERT(intList.IsValid(intList.GetBegin()));
    E_ASSERT(!intList.IsValid(intList.GetEnd()));
    E_ASSERT(!intList.IsValid(intList.GetSize()));

    // void          SetGranularity(U32 v);
    // void          SetGrowthFactor(F32 v);
    customIntList.Resize(0);

    customIntList.PushBack(0);
    customIntList.PushBack(1);
    customIntList.PushBack(2);
    E_ASSERT(customIntList.GetSize() == 3);
    customIntList.PushBack(3);
    customIntList.PushBack(4);
    customIntList.PushBack(5);
    customIntList.PushBack(6);
    E_ASSERT(customIntList.GetSize() == 9); // 6 * 150 / 100

    /*-----------------------------------------------------------------
    Methods
    -----------------------------------------------------------------*/
    //void          Clear();
    //void          Compact();
    customIntList.Compact();
    E_ASSERT(customIntList.GetSize() == 7 && customIntList.GetCount() == customIntList.GetSize());
    customIntList.Clear();
    E_ASSERT(customIntList.GetSize() == 7 && customIntList.GetCount() == 0);

    // void          Copy(const T* pData, U32 size, U32 startIndex = 0);
    // void          CopyPod(const T* pData, U32 size, U32 startIndex = 0);
    preSizedIntList.Copy(b.GetPtr(), b.GetSize());
    E_ASSERT(preSizedIntList.GetSize() == TEST_SIZE && preSizedIntList.GetCount() == b.GetSize());
    preSizedIntList.Copy(b.GetPtr(), b.GetSize(), preSizedIntList.GetCount());
    E_ASSERT(preSizedIntList.GetSize() == TEST_SIZE && preSizedIntList.GetCount() == b.GetSize() * 2);
    
    // void          EnsureSize(U32 size);
    customIntList.EnsureSize(13);
    E_ASSERT(customIntList.GetSize() >= 13);

    // void          Fill(const T& value, U32 startIndex = 0);
    customIntList.PushBack(0);
    customIntList.PushBack(1);
    customIntList.PushBack(2);
    customIntList.PushBack(3);
    customIntList.PushBack(4);
    customIntList.PushBack(5);
    customIntList.PushBack(6);
  
    customIntList.Fill(7, customIntList.GetCount());
    for (U32 i = 7; i < customIntList.GetSize(); ++i)
    {
      E_ASSERT(customIntList[i] == 7);
    }

    // ConstIterator Find(const T& value) const;
    // Iterator Find(const T& value);
    // U32           FindIndex(const T& value) const;
    // T*            FindValue(const T& value) const;
    E_ASSERT(*customIntList.Find(7) == customIntList[7] && *customIntList.FindValue(6) == customIntList[6] && customIntList.FindIndex(6) == 6);

    // void          Insert(const T& value, U32 index);
    customIntList.InsertAt(8, 8);
    E_ASSERT(customIntList[8] == 8);

    // void          PopBack(U32 count = 1);
    // void          PushBack(const T& value); (already tested)
    // void          PushBack(const T* pData, U32 count);
    // void          PushBack(const E::Containers::List<T, AllocatorClass>& other, U32 count);
    while (customIntList.GetCount() > 8)
      customIntList.PopBack();
    E_ASSERT(customIntList.GetCount() ==  8);
    customIntList.PopBack(4);
    E_ASSERT(customIntList.GetCount() ==  4);

    intList.Trim(4);
    intList.Compact();
    intList.PushBack(customIntList);
    E_ASSERT(intList.GetCount() == 8);
    intList.PushBack(customIntList.GetPtr(), customIntList.GetCount());
    E_ASSERT(intList.GetCount() == 12);
    intList.PushBack(customIntList.GetPtr(), 1);
    E_ASSERT(intList.GetCount() == 13);
    intList.Trim(8);


    // void          Remove(ConstIterator cit);
    // void          Remove(Iterator it);
    // void          Remove(U32 index);
    // void          RemoveFast(ConstIterator cit);
    // void          RemoveFast(Iterator it);
    // void          RemoveFast(U32 index);
    // bool          RemoveValue(const T& value);
    // bool          RemoveValueFast(const T& value);

    intList.Remove(intList.GetBegin());
    E_ASSERT(intList[0] == TEST_SIZE - 1 && intList.GetCount() == 7);
    intList.RemoveFast(intList.GetBegin());
    E_ASSERT(intList[0] == 3 && intList.GetCount() == 6);
    intList.RemoveIndex(1);
    E_ASSERT(intList[1] == TEST_SIZE - 3 && intList.GetCount() == 5);
    intList.RemoveIndexFast(0);
    E_ASSERT(intList[0] == 2 && intList.GetCount() == 4);
    E_ASSERT(!intList.IsValid(intList.Find(3)));
    intList.Remove(intList.Find(1));
    E_ASSERT(intList[2] == 0 && intList.GetCount() == 3);
    intList.Compact();

    intList = zeroList;
    E_ASSERT(intList.GetSize() == TEST_SIZE && intList.GetCount() == TEST_SIZE);
    zeroList.Trim(3);
    zeroList.Compact();
    E_ASSERT(zeroList.GetSize() == 3 && zeroList.GetCount() == 3);

    zeroList.Remove(zeroList.GetBegin(), 3);
    E_ASSERT(zeroList.IsEmpty());
    for (U32 i = 0; i < TEST_SIZE; ++i)
    {
      zeroList.PushBack(i);
    }
    zeroList.Remove(zeroList.GetBegin() + TEST_SIZE / 2, TEST_SIZE / 4);
    E_ASSERT(zeroList[TEST_SIZE / 2] == TEST_SIZE / 4 * 3);
    zeroList.Remove(zeroList.GetBegin(), TEST_SIZE / 2);
    E_ASSERT(*zeroList.GetBegin() == TEST_SIZE / 4 * 3);
   
    // void          Reserve(U32 size); (already tested)
    // void          Resize(U32 size); (already tested)
    
    // void          Swap(E::Containers::List<T, AllocatorClass>& other);
    // void          Trim(U32 size)

    zeroList = customIntList;
    E_ASSERT(zeroList.GetGranularity() == 1);
    E_ASSERT(customIntList.GetGranularity() == 1);
    
    customIntList.Clear();
    customIntList.PushBack(1);
    customIntList.PushBack(2);
    customIntList.PushBack(3);
    customIntList.PushBack(4);
    customIntList.PushBack(5);

    // Iteration
    I32 elementCounter = 0;
    for (E::Containers::List<I32>::ConstIterator cit = customIntList.GetBegin(); cit != customIntList.GetEnd(); ++cit)
    {
      elementCounter ++;
      E_ASSERT(*cit == elementCounter);
    }
    E_ASSERT(elementCounter == 5);
    for (E::Containers::List<I32>::ConstIterator cit = customIntList.GetBack(); customIntList.IsValid(cit); --cit)
    {
      E_ASSERT(*cit == elementCounter);
      elementCounter --;
    }
    E_ASSERT(elementCounter == 0);
    for (E::Containers::List<I32>::Iterator it = customIntList.GetBegin(); it != customIntList.GetEnd(); ++it)
    {
      elementCounter ++;
      E_ASSERT(*it == elementCounter);
    }
    E_ASSERT(elementCounter == 5);
    for (E::Containers::List<I32>::Iterator it = customIntList.GetBack(); customIntList.IsValid(it); --it)
    {
      E_ASSERT(*it == elementCounter);
      elementCounter --;
    }
    E_ASSERT(elementCounter == 0);
    customIntList.Clear();
    for (E::Containers::List<I32>::Iterator it = customIntList.GetBegin(); it != customIntList.GetEnd(); ++it)
    {
      elementCounter ++;
    }
    E_ASSERT(elementCounter == 0);
    for (E::Containers::List<I32>::Iterator it = customIntList.GetBack(); customIntList.IsValid(it); --it)
    {
      elementCounter --;
    }
    E_ASSERT(elementCounter == 0);
    for (auto it = begin(customIntList); it != end(customIntList); ++it)
    {
      elementCounter ++;
      E_ASSERT(*it == elementCounter);
    }
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool Test::List::RunPerformanceTest()
{
  try
  {
    std::cout << "[Test::sList::RunPerformanceTest]" << std::endl;
 
    U32 maxOperationCount = 1 << 13; // 13  15  18
    U32 iterationCount = 1; // 1  25
    F32 insertTime = 0;
    F32 insertTimeStd = 0;
    F32 findTime = 0;
    F32 findTimeStd = 0;
    F32 removeTime = 0;
    F32 removeTimeFast = 0;
    F32 removeTimeStd = 0;

    std::cout << "Iterations: " << iterationCount << " Max ops: " << maxOperationCount << std::endl << std::endl;
    for (U32 j = 0; j < iterationCount; ++j)
    {
      for (U32 i = 2; i <= maxOperationCount; i*=2)
      {
        E::Time::Timer t;
        std::vector<I32> list;
        insertTimeStd +=TimeInsert(i, list);
        findTimeStd +=TimeFind(i, list);
        removeTimeStd +=TimeRemove(i, list);
        std::cout << "STD vector [" << i << " ops] element count: " << list.size()  << " elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms" << std::endl;
      }

      for (U32 i = 2; i <= maxOperationCount; i*=2)
      {
        E::Time::Timer t;
        E::Containers::List<I32> list;
        insertTime += TimeInsert(i, list);
        findTime +=TimeFind(i, list);
        removeTime +=TimeRemove(i, list);
        std::cout << "E::Containers::List [" << i << " ops] element count: " << list.GetCount()  << " elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms" << std::endl;
      }

      for (U32 i = 2; i <= maxOperationCount; i*=2)
      {
        E::Time::Timer t;
        E::Containers::List<I32> list;
        TimeInsert(i, list);
        TimeFind(i, list);
        removeTimeFast +=TimeRemoveFast(i, list);
        list.Compact();
        std::cout << "E::Containers::List (fast) [" << i << " ops] element count: " << list.GetCount()  << " elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms" << std::endl;
      }

      std::cout << "Iteration: " << j + 1 << " max ops: " << maxOperationCount << std::endl;
      std::cout << "Push time [" << insertTime << " / " << insertTimeStd << "]\t" << (insertTimeStd / insertTime * 100.0) - 100.0 << "% faster" << std::endl;
      std::cout << "Find time [" << findTime << " / " << findTimeStd << "]\t" << (findTimeStd / findTime * 100.0) - 100.0 << "% faster" << std::endl;
      std::cout << "Rmv time [" << removeTime << " / " << removeTimeStd << "]\t" << (removeTimeStd / removeTime * 100.0) - 100.0 << "% faster" << std::endl;
      std::cout << "RmvFst time [" << removeTimeFast << " / " << removeTimeStd << "]\t" << (removeTimeStd / removeTimeFast * 100.0) - 100.0 << "% faster" << std::endl << std::endl;
      
      /*
      PC: 
      
      Windows 7 Enterprise 64-bit SP1
      Intel Core i7 3770 @ 3.40GHz
      16.0GB Dual-Channel DDR3 @ 799MHz

      x86 vs x64:

      Iteration: 25 max ops: 8192                          Iteration: 25 max ops: 8192
      Push time [20.065 / 20.498]     2.15791% faster      Push time [14.413 / 15.386]     6.75088% faster
      Find time [5.73199 / 1188.08]   20627.1% faster      Find time [4.917 / 1195.05]     24204.4% faster
      Rmv time [836.574 / 831.911]    -0.557439% faster    Rmv time [727.506 / 729.364]    0.255394% faster
      RmvFst time [755.252 / 831.911] 10.1501% faster      RmvFst time [746.938 / 729.364] -2.35277% faster
      Succeeded! Elapsed time: 6348.16 ms                  Succeeded! Elapsed time: 6089.03 ms

      Iteration: 25 max ops: 32768                         Iteration: 25 max ops: 32768
      Push time [74.5901 / 75.7371]   1.53774% faster      Push time [59.256 / 60.245]     1.66905% faster
      Find time [21.811 / 17458.2]    79943.1% faster      Find time [18.833 / 17406.6]    92325.8% faster
      Rmv time [13071.6 / 13072.7]    0.00818807% faster   Rmv time [11113.8 / 11113.6]    -0.00246167% faster
      RmvFst time [11032.7 / 13072.7] 18.4903% faster      RmvFst time [11010.2 / 11113.6] 0.939274% faster
      Succeeded! Elapsed time: 58138.1 ms                  Succeeded! Elapsed time: 53991 ms
      */
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

F32 TimeFind(U32 count, const E::Containers::List<I32>& intList)
{
  Math::Global::GetRandom().SetSeed(456456);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intList.FindIndex(Math::Global::GetRandom().GetI32(-65535, 65535));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeFind(U32 count, const std::vector<I32>& intList)
{
  Math::Global::GetRandom().SetSeed(456456);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    std::find(intList.begin(), intList.end(), Math::Global::GetRandom().GetI32(-65535, 65535));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeInsert(U32 count, E::Containers::List<I32>& intList)
{
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intList.PushBack(Math::Global::GetRandom().GetI32(-65535, 65535));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeInsert(U32 count, std::vector<I32>& intList)
{
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intList.push_back(Math::Global::GetRandom().GetI32(-65535, 65535));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeRemove(U32 count, E::Containers::List<I32>& intList)
{
  Math::Global::GetRandom().SetSeed(1012324);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intList.RemoveIf(Math::Global::GetRandom().GetI32(-65535, 65535));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeRemove(U32 count, std::vector<I32>& intList)
{
  Math::Global::GetRandom().SetSeed(1012324);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
  { 
    std::vector<I32>::const_iterator cit = std::find(intList.begin(), intList.end(), Math::Global::GetRandom().GetI32(-65535, 65535));
    if (cit != intList.end())
      intList.erase(cit);
    //intList.erase(std::remove(intList.begin(), intList.end(), Math::Global::GetRandom().GetI32(-65535, 65535)), intList.end());
  }

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeRemoveFast(U32 count, E::Containers::List<I32>& intList)
{
  Math::Global::GetRandom().SetSeed(1012324);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intList.RemoveIfFast(Math::Global::GetRandom().GetI32(-65535, 65535));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}
