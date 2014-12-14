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

// Created 04-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Algorithm.cpp
This file defines Algorithm test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary method declaration
----------------------------------------------------------------------------------------------------------------------*/
void SortPerformanceComparison(I32 minValue, I32 maxValue, U32 maxArraySize, U32 iterationCount);

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Algorithm::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Algorithm::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Algorithm::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

template <typename T>
void CheckSortedArray(const E::Containers::DynamicArray<T>& a, bool printValuesFlag = false)
{
  T val = E::Math::NumericLimits<T>::Min();
  for (U32 i = 0; i < a.GetSize(); ++i)
  {
    E_ASSERT(val <= a[i]);
    val = a[i];
    if (printValuesFlag)
      std::cout << a[i] << " ";
  }
  if (printValuesFlag)
    std::cout << std::endl;
}

#ifdef E_DEBUG
template <typename T>
void CompareSortedArray(const E::Containers::DynamicArray<T>& a, const std::vector<T>& b)
{
  E_ASSERT(a.GetSize() == b.size());
  for (U32 i = 0; i < a.GetSize(); ++i)
  {
    E_ASSERT(b[i] == a[i]);
  }
}
#else
template <typename T>
void CompareSortedArray(const E::Containers::DynamicArray<T>&, const std::vector<T>&) {}
#endif

bool Test::Algorithm::RunFunctionalityTest()
{
  std::cout << "[Test::Algorithm::RunFunctionalityTest]" << std::endl << std::endl;
  
    /*
  E::Containers::DynamicArray<I32> a(4), b(4);
  std::vector<I32> v(4);
  a[0] = v[0] = 5;
  a[1] = v[1] = 1;
  a[2] = v[2] = 6;
  a[3] = v[3] = 2;
  */

  E::Containers::DynamicArray<I32> a(8), b(8);
  std::vector<I32> v(8);
  a[0] = v[0] = 5;
  a[1] = v[1] = 6;
  a[2] = v[2] = 1;
  a[3] = v[3] = 7;
  a[4] = v[4] = 5;
  a[5] = v[5] = 3;
  a[6] = v[6] = 6;
  a[7] = v[7] = 8;
  
  std::sort(v.begin(), v.end());

  b.Copy(a.GetPtr(), a.GetSize());
  E::Math::Sorting<I32>::HeapSort(&b[0], 4, 5);
  E::Math::Sorting<I32>::HeapSort(&b[0], 2, 4);
  E::Math::Sorting<I32>::HeapSort(&b[0], 0, 2);
  E::Math::Sorting<I32>::HeapSort(&b[0], 2, 4);
  E::Math::Sorting<I32>::HeapSort(&b[0], 1, 5);
  E::Math::Sorting<I32>::HeapSort(b.GetPtr(), b.GetSize());
  CompareSortedArray(b, v);

  b.Copy(a.GetPtr(), a.GetSize());
  E::Math::Sorting<I32>::QuickSort(b.GetPtr(), b.GetSize());
  CompareSortedArray(b, v);


  return true;
}

bool Test::Algorithm::RunPerformanceTest()
{
  std::cout << "[Test::Algorithm::RunPerformanceTest]" << std::endl << std::endl;

  U32 maxArraySize = 1 << 10;   //  10  15  18
  U32 iterationCount = 1 << 15; //  15  15  20
  I32 minValue = 0; Math::NumericLimits<I32>::Min();
  I32 maxValue = Math::NumericLimits<I32>::Max();

  E::Math::Global::GetRandom().RandomizeSeed();

  SortPerformanceComparison(minValue, maxValue, maxArraySize, iterationCount);
  
  minValue = 0;
  maxValue = 100;
  
  SortPerformanceComparison(minValue, maxValue, maxArraySize, iterationCount);
 
  return true;
}

I32 compare_ints(const void* a, const void* b)   // comparison function
{
  I32 arg1 = *reinterpret_cast<const I32*>(a);
  I32 arg2 = *reinterpret_cast<const I32*>(b);
  if(arg1 < arg2) return -1;
  if(arg1 > arg2) return 1;
  return 0;
}

void SortPerformanceComparison(I32 minValue, I32 maxValue, U32 maxArraySize, U32 iterationCount)
{
  /*-------------------------------------------------------------------------------
  STD vs QuickSort vs Sort with big value range
  -------------------------------------------------------------------------------*/
  std::cout << "Parameters: " << std::endl
            << "- Max array size      [" << maxArraySize << "]" << std::endl
            << "- Value range         [" << maxValue - minValue << "]" << std::endl
            << "- Max iteration count [" << iterationCount << "]"  << std::endl << std::endl;

  E::Time::Timer t;
  U32 weightedIterationCount = 0;
  F32 totalSortTimeStd = 0;
  F32 totalQuickSortTime = 0;
  F32 totalQuickSortDPTime = 0;
  F32 totalQuickSort3WTime = 0;
  F32 totalIntroSortTime = 0;

  for (U32 i = 2; i <= maxArraySize; i*=2)
  {
    F32 sortTimeStd = 0;
    F32 quickSortTime = 0;
    F32 quickSortDPTime = 0;
    F32 quickSort3WTime = 0;
    F32 introSortTime = 0;

    weightedIterationCount = iterationCount / i;
    for (U32 j = 0; j < weightedIterationCount; ++j)
    {
      E::Containers::DynamicArray<I32> intArray(i), intArray2(i), intArray3(i), intArray4(i);
      std::vector<I32> intArrayStd(i);
      for (U32 k = 0; k < i; ++k)
      {
        /*
        I32 value = k;
        if (k % 8 == 0)
          value = Math::Global::GetRandom().GetI32(minValue, maxValue);
        */
        I32 value = Math::Global::GetRandom().GetI32(minValue, maxValue);
        intArrayStd[k] = intArray[k] = intArray2[k] = intArray3[k] = intArray4[k] = value;
      }

      t.Reset();
      std::sort(intArrayStd.begin(), intArrayStd.end());
      //std::qsort(&intArrayStd[0], intArrayStd.size(), sizeof(I32), compare_ints);
      //std::make_heap(intArrayStd.begin(), intArrayStd.end());
      //std::sort_heap(intArrayStd.begin(), intArrayStd.end());
      sortTimeStd += static_cast<F32>(t.GetElapsed().GetMilliseconds());
    
      t.Reset();
      Math::Sorting<I32>::QuickSort(intArray.GetPtr(), intArray.GetSize());
      quickSortTime += static_cast<F32>(t.GetElapsed().GetMilliseconds());
      CheckSortedArray(intArray);
      CompareSortedArray(intArray, intArrayStd);
      
      t.Reset();
      Math::Sorting<I32>::QuickSort3Way(intArray3.GetPtr(), intArray3.GetSize());
      quickSort3WTime += static_cast<F32>(t.GetElapsed().GetMilliseconds());
      CheckSortedArray(intArray3);
      CompareSortedArray(intArray3, intArrayStd);

      t.Reset();
      Math::Sorting<I32>::QuickSortDualPivot(intArray2.GetPtr(), intArray2.GetSize());
      quickSortDPTime += static_cast<F32>(t.GetElapsed().GetMilliseconds());
      CheckSortedArray(intArray2);
      CompareSortedArray(intArray2, intArrayStd);
 
      t.Reset();
      Math::Sorting<I32>::IntroSort(intArray4.GetPtr(), intArray4.GetSize());
      introSortTime += static_cast<F32>(t.GetElapsed().GetMilliseconds());
      CheckSortedArray(intArray4);
      CompareSortedArray(intArray4, intArrayStd);
    }

    if (quickSortTime > sortTimeStd)
    {
      F32 percentage = (sortTimeStd / quickSortTime * 100.0f) - 100.0f;
      std::cout << "!QuickSort - array size " << i << " / " << weightedIterationCount << " iterations\t" << percentage << "% faster (+" << quickSortTime - sortTimeStd << " ms)" << std::endl;
    }
    if (quickSort3WTime > sortTimeStd)
    {
      F32 percentage = (sortTimeStd / quickSort3WTime * 100.0f) - 100.0f;
      std::cout << "!QS 3-Way  - array size " << i << " / " << weightedIterationCount << " iterations\t" << percentage << "% faster (+" << quickSort3WTime - sortTimeStd << " ms)" <<std::endl;
    }
    if (quickSortDPTime > sortTimeStd)
    {
      F32 percentage = (sortTimeStd / quickSortDPTime * 100.0f) - 100.0f;
      std::cout << "!QS DualP  - array size " << i << " / " << weightedIterationCount << " iterations\t" << percentage << "% faster (+" << quickSortDPTime - sortTimeStd << " ms)" <<std::endl;
    }
    if (introSortTime > sortTimeStd)
    {
      F32 percentage = (sortTimeStd / introSortTime * 100.0f) - 100.0f;
      std::cout << "!IntroSort - array size " << i << " / " << weightedIterationCount << " iterations\t" << percentage << "% faster (+" << introSortTime - sortTimeStd << " ms)" <<std::endl;
    }

    totalSortTimeStd += sortTimeStd;
    totalQuickSortTime += quickSortTime;
    totalQuickSortDPTime += quickSortDPTime;
    totalQuickSort3WTime += quickSort3WTime;
    totalIntroSortTime += introSortTime;
  }

  std::cout << std::endl << "Last weighted iterations count: " << weightedIterationCount << std::endl
            << "QuickSort time [" << totalQuickSortTime << " / " << totalSortTimeStd << " ms]\t" << (totalSortTimeStd / totalQuickSortTime * 100.0f) - 100.0f << "% faster" << std::endl
            << "QS 3-Way  time [" << totalQuickSort3WTime << " / " << totalSortTimeStd << " ms]\t" << (totalSortTimeStd / totalQuickSort3WTime * 100.0f) - 100.0f << "% faster" << std::endl
            << "QS DualP  time [" << totalQuickSortDPTime << " / " << totalSortTimeStd << " ms]\t" << (totalSortTimeStd / totalQuickSortDPTime * 100.0f) - 100.0f << "% faster" << std::endl
            << "IntroSort time [" << totalIntroSortTime << " / " << totalSortTimeStd << " ms]\t" << (totalSortTimeStd / totalIntroSortTime * 100.0f) - 100.0f << "% faster" << std::endl
            << std::endl;
  
  /*
  Test results:

  0. QuickSort, QuickSort Dual Pivot and IntroSort perform far better than std::sort with pseudo-sorted arrays.
  1. QuickSort Dual-Pivot has the best overall performance in all conditions.
  2. IntroSort has the best overall performance with relatively small arrays (decreases performance from 2048).
  3. QuickSort loses performance with a high amount of repeated elements.
  4. QuickSort 3-Way has better performance on arrays with a high amount of repeated elements beating QuickSort.
  5. Using a 3-way median pivot increases significantly overall algorithms performance with pseudo-sorted arrays.

  ----------------------------------------------------------------------------------------------------------------------
  Small test (10 / 18)
  ----------------------------------------------------------------------------------------------------------------------

  Parameters:                                               Parameters:
  - Max array size      [1024]                              - Max array size      [1024]
  - Value range         [2147483647]                        - Value range         [100]
  - Max iteration count [262144]                            - Max iteration count [262144]

                                                            !QuickSort - array size 32 / 8192 iterations    -2.85529% faster (+0.145986 ms)
  
  Last weighted iterations count: 256                       Last weighted iterations count: 256
  QuickSort time [82.329 / 93.5018 ms]    13.5709% faster   QuickSort time [81.5222 / 89.439 ms]    9.71122% faster
  QS 3-Way  time [86.2759 / 93.5018 ms]   8.37534% faster   QS 3-Way  time [83.007 / 89.439 ms]     7.74871% faster
  QS DualP  time [80.5352 / 93.5018 ms]   16.1005% faster   QS DualP  time [78.8342 / 89.439 ms]    13.4521% faster
  IntroSort time [79.2032 / 93.5018 ms]   18.053% faster    IntroSort time [77.8924 / 89.439 ms]    14.8238% faster

  Succeeded! Elapsed time: 6062.52 ms [Test::Algorithm::RunPerformanceTest]

  ----------------------------------------------------------------------------------------------------------------------
  Big test (15 / 20)
  ----------------------------------------------------------------------------------------------------------------------

  Parameters:                                                                     Parameters:
  - Max array size      [32768]                                                   - Max array size      [32768]
  - Value range         [2147483647]                                              - Value range         [100]
  - Max iteration count [1048576]                                                 - Max iteration count [1048576]

  !QuickSort - array size 32 / 32768 iterations   -3.92916% faster (+0.806551 ms) !QuickSort - array size 16 / 65536 iterations   -0.0263303% faster (+0.00591278 ms)
                                                                                  !QuickSort - array size 32 / 32768 iterations   -3.47552% faster (+0.709681 ms)
                                                                                  !QuickSort - array size 2048 / 512 iterations   -6.41003% faster (+2.09101 ms)
                                                                                  !IntroSort - array size 2048 / 512 iterations   -5.00061% faster (+1.60705 ms)
                                                                                  !QuickSort - array size 4096 / 256 iterations   -15.2797% faster (+5.17798 ms)
                                                                                  !IntroSort - array size 4096 / 256 iterations   -14.1344% faster (+4.72599 ms)
                                                                                  !QuickSort - array size 8192 / 128 iterations   -18.3709% faster (+6.21599 ms)
                                                                                  !IntroSort - array size 8192 / 128 iterations   -18.129% faster (+6.11599 ms)
                                                                                  !QuickSort - array size 16384 / 64 iterations   -21.2004% faster (+7.202 ms)
                                                                                  !IntroSort - array size 16384 / 64 iterations   -20.5243% faster (+6.913 ms)
                                                                                  !QuickSort - array size 32768 / 32 iterations   -22.8118% faster (+7.777 ms)
                                                                                  !IntroSort - array size 32768 / 32 iterations   -24.6895% faster (+8.627 ms)

  Last weighted iterations count: 32                                              Last weighted iterations count: 32
  QuickSort time [537.312 / 645.227 ms]   20.0843% faster                         QuickSort time [497.222 / 495.681 ms]   -0.309968% faster
  QS 3-Way  time [601.756 / 645.227 ms]   7.22398% faster                         QS 3-Way  time [469.172 / 495.681 ms]   5.65017% faster
  QS DualP  time [529.304 / 645.227 ms]   21.9012% faster                         QS DualP  time [443.178 / 495.681 ms]   11.847% faster
  IntroSort time [523.526 / 645.227 ms]   23.2465% faster                         IntroSort time [482.007 / 495.681 ms]   2.83683% faster
  
  Succeeded! Elapsed time: 26747.8 ms [Test::Algorithm::RunPerformanceTest]

  ----------------------------------------------------------------------------------------------------------------------
  Big test (15 / 18 - pseudo-sorted array: every 8 elements a random is introduced)
  ----------------------------------------------------------------------------------------------------------------------

  Parameters:                                                  Parameters:
  - Max array size      [32768]                                - Max array size      [32768]
  - Value range         [2147483647]                           - Value range         [100]
  - Max iteration count [262144]                               - Max iteration count [262144]

  (...)

  Last weighted iterations count: 8                            Last weighted iterations count: 8
  QuickSort time [82.6293 / 106.955 ms]   29.4395% faster      QuickSort time [87.2893 / 106.049 ms]   21.4917% faster
  QS 3-Way  time [96.0573 / 106.955 ms]   11.3449% faster      QS 3-Way  time [100.455 / 106.049 ms]   5.56842% faster
  QS DualP  time [77.1256 / 106.955 ms]   38.6763% faster      QS DualP  time [86.8242 / 106.049 ms]   22.1425% faster
  IntroSort time [81.3454 / 106.955 ms]   31.4825% faster      IntroSort time [86.2494 / 106.049 ms]   22.9565% faster

  Succeeded! Elapsed time: 3227.32 ms [Test::Algorithm::RunPerformanceTest]
  */
}
