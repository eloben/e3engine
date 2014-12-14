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

// Created 08-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file E::Containers::Map.cpp
This file defines E::Containers::Map test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace E::Containers;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

#ifndef TEST_SMALL_SIZE
#define TEST_SMALL_SIZE 32
#endif

#ifndef TEST_BIG_SIZE
#define TEST_BIG_SIZE 4096
#endif

#ifndef TEST_SIZE
#define TEST_SIZE TEST_BIG_SIZE
#endif

F32 TimeFind(U32 count, const E::Containers::Map<U32, U32>& intMap);
F32 TimeFindValue(U32 count, const E::Containers::Map<U32, U32>& intMap);
F32 TimeFind(U32 count, const std::map<U32, U32>& intMap);
F32 TimeInsert(U32 count, E::Containers::Map<U32, U32>& intMap);
F32 TimeInsert(U32 count, std::map<U32, U32>& intMap);
F32 TimeRemoveBoth(U32 count, std::map<U32, U32>& intMapStd, E::Containers::Map<U32, U32>& intMap);
F32 TimeRemove(U32 count, E::Containers::Map<U32, U32>& intMap);
F32 TimeRemove(U32 count, std::map<U32, U32>& intMap);

#ifdef E_DEBUG
void CompareMap(const E::Containers::Map<U32, U32>& map, const std::map<U32, U32>& stdMap)
{
  E_ASSERT(map.GetCount() == stdMap.size());
  for (auto it = begin(map); it != end(map); ++it)
  {
    auto stdIt = stdMap.find((*it).first);
    E_ASSERT(stdIt != end(stdMap));
    E_ASSERT((*stdIt).second == (*it).second);
  }
  for (auto stdIt = begin(stdMap); stdIt != end(stdMap); ++stdIt)
  {
    auto it = map.Find((*stdIt).first);
    E_ASSERT(it != end(map));
    E_ASSERT((*stdIt).second == (*it).second);
  }
}
#else
void CompareMap(const E::Containers::Map<U32, U32>&, const std::map<U32, U32>&) {}
#endif

/*----------------------------------------------------------------------------------------------------------------------
TestMap methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Map::Run()
{
  try
  {
    std::cout << "[Test::Map::Run]" << std::endl;
    E::Time::Timer t;

    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Map::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Map::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

struct MapFoo
{
  MapFoo(I32 v) : v(v) {}
  I32 v;
};

bool Test::Map::RunFunctionalityTest()
{
  try
  {
    std::cout << "[Test::Map::RunFunctionalityTest]" << std::endl;

    // Map template hasher class test
    E::Containers::Map<I32, I32> intMap;
    intMap.Insert(0, 0);

    E::Containers::Map<U32, U32> uintMap;
    uintMap.Insert(0, 0);

    E::Containers::Map<I64, I64> int64Map;
    int64Map.Insert(0, 0);

    E::Containers::Map<U64, U64> uint64Map;
    uint64Map.Insert(0, 0);

    MapFoo f1(1);
    E::Containers::Map<MapFoo*, I32> ptrMap;
    ptrMap.Insert(&f1, 0);

    E::Containers::Map<E::String, I64> strMap;
    strMap.Insert("SomeStr", 45);

    // Map();
    //~Map();
    E::Containers::Map<U32, U32> map;

    /*-----------------------------------------------------------------
    Operators
    -----------------------------------------------------------------*/
    //const ValueType& operator [] (KeyType key) const;
    //ValueType& operator [] (KeyType key);

    /*-----------------------------------------------------------------
    Accessors
    -----------------------------------------------------------------*/
    //Iterator      GetBegin() const;
    //U32           GetCount() const;
    //Iterator      GetEnd() const;
    //F32           GetGrowthPercentage() const;
    //KeyType       GetKey(ConstIterator cit) const;
    //KeyType       GetKey(Iterator it) const;
    //ConstIterator GetNext(ConstIterator cit) const;
    //Iterator      GetNext(Iterator it) const;
    //U32           GetSize() const;
    //ValueType     GetValue(ConstIterator cit) const;
    //ValueType     GetValue(Iterator it) const;
    //bool          HasKey(KeyType key) const;
    //bool          IsEmpty() const;
    //bool          IsValid(ConstIterator cit) const;
    //bool          IsValid(Iterator it) const;
    //void          SetGrowthFactor(F32 v); (already tested)

    U32 uniqueKeyCount = 0;
    std::vector<U32> uniqueKeys;
    for (U32 i = 0; i < TEST_SMALL_SIZE; ++i)
    {
      U32 key = Math::Global::GetRandom().GetU32(1000);
      std::vector<U32>::const_iterator cit = std::find(uniqueKeys.begin(), uniqueKeys.end(), key);
      if (cit == uniqueKeys.end())
      {
        uniqueKeyCount++;
        uniqueKeys.push_back(key);
        map.Insert(key, i);
        //std::cout << "Map Insert [" << key << "] is: " << i << std::endl;
      }
    }
    E_ASSERT(map.GetCount() == uniqueKeyCount && map.GetSize() >= map.GetCount() && !map.IsEmpty());
    std::cout << "Map count: " << map.GetCount() << " size: " << map.GetSize() << std::endl;

    U32 hitCount = 0;
    for (E::Containers::Map<U32, U32>::Iterator it = map.GetBegin(); it != map.GetEnd(); ++it)
    {
      hitCount ++;
      //std::cout << "Map Browse [" << (*it).first << "] is: " << (*it).second  << std::endl;
      E_ASSERT(map.IsValid(it) && map.HasKey((*it).first));
    }
    E_ASSERT(hitCount == map.GetCount());

    hitCount = 0;
    for (E::Containers::Map<U32, U32>::ConstIterator cit = map.GetBegin(); cit != map.GetEnd(); ++cit)
    {
      hitCount ++;
      //(*it).second = 0; // Invalid
      //std::cout << "STD Map Browse [" << (*cit).first << "] is: " << (*cit).second  << std::endl;
      E_ASSERT(map.IsValid(cit) && map.HasKey((*cit).first));
    }
    E_ASSERT(hitCount == map.GetCount());
    E_ASSERT(!map.IsValid(map.GetEnd()));

    hitCount = 0;
    for (auto it = begin(map); it != end(map); ++it)
    {
      hitCount ++;
      //(*it).second = 0; // Invalid
      //std::cout << "STD Map Browse [" << (*it).first << "] is: " << (*it).second  << std::endl;
      E_ASSERT(map.IsValid(it) && map.HasKey((*it).first));
    }
    E_ASSERT(hitCount == map.GetCount());
    E_ASSERT(!map.IsValid(map.GetEnd()));

    /*-----------------------------------------------------------------
    Methods
    -----------------------------------------------------------------*/
 
    // Methods
    //void          Clear();
    //void          Compact();
    //Iterator      Find(KeyType key) const;
    //ValueType*    FindValue(KeyType key) const;
    //bool          Insert(KeyType key, ValueType value);
    //void          Remove(ConstIterator cit);
    //void          Remove(Iterator it);
    //void          Resize(U32 size);

    hitCount = 0;
    for (std::vector<U32>::const_iterator cit = uniqueKeys.begin(); cit != uniqueKeys.end(); ++cit)
    {
      E_ASSERT(map.IsValid(map.Find(*cit)));
      E_ASSERT((*map.FindPair(*cit)).second == hitCount);
      hitCount++;
    }

    while (uniqueKeys.size() != 10)
    {
      U32 key = *uniqueKeys.rbegin();
      uniqueKeys.pop_back();
      map.Remove(map.Find(key));
    }
    map.Resize(16);
    map.Compact();
    E_ASSERT(map.GetSize() == 16);

    //map.Resize(8); // Will E_ASSERT: map size has to be greater than the element count
    //map.Resize(20); // Will E_ASSERT: map size has to be power of 2
    map.Resize(64);
    hitCount = 0;
    for (std::vector<U32>::const_iterator cit = uniqueKeys.begin(); cit != uniqueKeys.end(); ++cit)
    {
      E_ASSERT(map.IsValid(map.Find(*cit)));
      E_ASSERT((*map.FindPair(*cit)).second == hitCount);
      hitCount++;
    }

    E_ASSERT(map.Insert(uniqueKeys[5], 13));
    E::Containers::Map<U32, U32>::Iterator mapFindIt = map.Find(uniqueKeys[5]);
    E_ASSERT(map.IsValid(mapFindIt));
    E_ASSERT((*map.FindPair(uniqueKeys[5])).second == 13);


    map.Remove(mapFindIt);
    mapFindIt = map.Find(uniqueKeys[5]);
    E_ASSERT(!map.IsValid(mapFindIt));
    E_ASSERT(!map.RemoveIf(uniqueKeys[5]));

    E_ASSERT(map.Insert(uniqueKeys[5], 13));
    mapFindIt = map.Find(uniqueKeys[5]);
    E_ASSERT(map.IsValid(mapFindIt));

    map.Clear();
    E_ASSERT(map.GetCount() == 0);
    map.Resize(0);
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool Test::Map::RunPerformanceTest()
{
  try
  {
    std::cout << "[Test::Map::RunPerformanceTest]" << std::endl;
 
    U32 maxOperationCount = 1 << 13;  // 10       13  15  18  20
    U32 iterationCount =  1;          // 10000    10  1     
    F32 insertTimeStd = 0;
    F32 findTimeStd = 0;
    F32 removeTimeStd = 0;
    F32 insertTime = 0;
    F32 findTime = 0;
    F32 findTimeValue = 0;
    F32 removeTime = 0;

    /*
    // Debug both maps at the same time
    std::cout << "Iterations: " << iterationCount << " Max ops: " << maxOperationCount << std::endl << std::endl;
    for (U32 j = 0; j < iterationCount; ++j)
    {
      for (U32 i = 2; i <= maxOperationCount; i*=2)
      {
        E::Time::Timer t;
        std::map<U32, U32> map;
        Map<U32, U32> map2;
        TimeInsert(i, map);
        TimeInsert(i, map2);

        TimeRemoveBoth(i, map, map2);
        std::cout << "STD Map [" << i << " ops] element count: " << map.size()  << " elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms" << std::endl;
      }
    }
    */
    std::cout << "Iterations: " << iterationCount << " Max ops: " << maxOperationCount << std::endl << std::endl;
    for (U32 j = 0; j < iterationCount; ++j)
    {
      std::map<U32, U32> stdMap;
      E::Containers::Map<U32, U32> map;
      for (U32 i = 2; i <= maxOperationCount; i*=2)
      {
        E::Time::Timer t;
        stdMap.clear();
        insertTimeStd +=TimeInsert(i, stdMap);
        findTimeStd +=TimeFind(i, stdMap);
        removeTimeStd +=TimeRemove(i, stdMap);
        //std::cout << "STD Map [" << i << " ops] element count: " << stdMap.size()  << " elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms" << std::endl;
      }
      for (U32 i = 2; i <= maxOperationCount; i*=2)
      {
        E::Time::Timer t;
        map.Clear();
        //map.SetMaxOccupancyPercentage(50);
        insertTime += TimeInsert(i, map);
        findTime +=TimeFind(i, map);
        findTimeValue +=TimeFindValue(i, map);
        removeTime +=TimeRemove(i, map);
        map.Compact(); // Tests Compact & Clear
        //std::cout << "Map [" << i << " ops] element count: " << map.GetCount() << " size: " << " elapsed time: " << t.GetElapsed().GetMilliseconds() << " ms" << std::endl;
      }
      CompareMap(map, stdMap);
 
    }
    std::cout << "Iteration: " << iterationCount << " max ops: " << maxOperationCount << std::endl;
    std::cout << "Insert time [" << insertTime << " / " << insertTimeStd << "]\t" << (insertTimeStd / insertTime * 100.0) - 100.0 << "% faster" << std::endl;
    std::cout << "Find   time [" << findTime << " / " << findTimeStd << "]\t" << (findTimeStd / findTime * 100.0) - 100.0 << "% faster" << std::endl;
    std::cout << "Find V time [" << findTimeValue << " / " << findTimeStd << "]\t" << (findTimeStd / findTimeValue * 100.0) - 100.0 << "% faster" << std::endl;
    std::cout << "Remove time [" << removeTime << " / " << removeTimeStd << "]\t" << (removeTimeStd / removeTime * 100.0) - 100.0 << "% faster" << std::endl;
    /* 
    Pre-iterator (GetNext method):

    Iteration: 1 max ops: 32768
    Insert time [5.184 / 291.781]   5528.49% faster
    Find   time [0.897 / 9.122]     916.945% faster
    Remove time [3.814 / 590.199]   15374.5% faster
    Succeeded! Elapsed time: 996.743 ms [Test::Map::RunPerformanceTest]

    Iterator implementation:

    Iteration: 10000 max ops: 1024
    Insert time [1508.78 / 57332.9] 3699.95% faster
    Find   time [1016.81 / 1690.95] 66.2984% faster
    Find V time [378.788 / 1690.95] 346.409% faster
    Remove time [1173.06 / 12562.9] 970.954% faster
    Succeeded! Elapsed time: 81073.9 ms [Test::Map::RunPerformanceTest]

    Iteration: 1 max ops: 32768
    Insert time [4.66 / 290.45]     6132.83% faster
    Find   time [3.522 / 9.051]     156.985% faster
    Find V time [0.896 / 9.051]     910.156% faster
    Remove time [3.811 / 594.45]    15498.3% faster
    Succeeded! Elapsed time: 929.645 ms [Test::Map::RunPerformanceTest]

    Iteration: 1 max ops: 262144
    Insert time [34.915 / 1458.67]  4077.79% faster
    Find   time [31.054 / 93.524]   201.166% faster
    Find V time [6.986 / 93.524]    1238.73% faster
    Remove time [31.075 / 2203]     6989.31% faster
    Succeeded! Elapsed time: 3895.33 ms [Test::Map::RunPerformanceTest]

    Iteration: 1 max ops: 1048576
    Insert time [131.901 / 4088.95] 3000.01% faster
    Find   time [124.447 / 402.537] 223.461% faster
    Find V time [27.471 / 402.537]  1365.32% faster
    Remove time [90.816 / 2573.35]  2733.59% faster
    Succeeded! Elapsed time: 7476.97 ms [Test::Map::RunPerformanceTest]
    */ 
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

static I32 gNumber = 65535;

F32 TimeFind(U32 count, const E::Containers::Map<U32, U32>& intMap)
{
  Math::Global::GetRandom().SetSeed(456456);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intMap.Find(Math::Global::GetRandom().GetI32(0, gNumber));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeFindValue(U32 count, const E::Containers::Map<U32, U32>& intMap)
{
  Math::Global::GetRandom().SetSeed(456456);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intMap.FindPair(Math::Global::GetRandom().GetI32(0, gNumber));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeFind(U32 count, const std::map<U32, U32>& intMap)
{
  Math::Global::GetRandom().SetSeed(456456);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
    intMap.find(Math::Global::GetRandom().GetI32(0, gNumber));

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeInsert(U32 count, E::Containers::Map<U32, U32>& intMap)
{
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
  {
    intMap.Insert(Math::Global::GetRandom().GetI32(0, gNumber), 0);
  }

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeInsert(U32 count, std::map<U32, U32>& intMap)
{
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
  {
    intMap.insert(std::make_pair(Math::Global::GetRandom().GetI32(0, gNumber), 0));
  }

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}


static bool bValidIt = false;
static bool bValidItStd = false;

F32 TimeRemoveBoth(U32 count, std::map<U32, U32>& intMapStd, Map<U32, U32>& intMap)
{
  Math::Global::GetRandom().SetSeed(1012324);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
  {
    I32 n = Math::Global::GetRandom().GetI32(0, gNumber);
    std::map<U32, U32>::iterator itStd = intMapStd.find(n);
    bValidItStd = itStd != intMapStd.end();

    Map<U32, U32>::Iterator it = intMap.Find(n);
    bValidIt = it != intMap.GetEnd();

    if (bValidItStd && bValidIt)
    {
      intMapStd.erase(itStd);
      intMap.Remove(it);
    }
    else
    {
      E_ASSERT(bValidItStd == bValidIt);
    }
  }

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeRemove(U32 count, E::Containers::Map<U32, U32>& intMap)
{
  Math::Global::GetRandom().SetSeed(1012324);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
  {
    E::Containers::Map<U32, U32>::Iterator it = intMap.Find(Math::Global::GetRandom().GetI32(0, gNumber));
    if (it != intMap.GetEnd())
      intMap.Remove(it);
  }

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}

F32 TimeRemove(U32 count, std::map<U32, U32>& intMap)
{
  Math::Global::GetRandom().SetSeed(1012324);
  E::Time::Timer t;
  for (U32 i = 0; i < count; ++i)
  { 
    std::map<U32, U32>::iterator it = intMap.find(Math::Global::GetRandom().GetI32(0, gNumber));
    if (it != intMap.end())
      intMap.erase(it);
  }

  return static_cast<F32>(t.GetElapsed().GetMilliseconds());
}
