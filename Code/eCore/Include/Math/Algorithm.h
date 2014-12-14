/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 07-Jul-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Algorithm.h
This file defines some useful math algorithms.
*/

#ifndef E_MATH_ALGORITHM_H
#define E_MATH_ALGORITHM_H

#include <Base.h>
#include <Math/Comparison.h>

/*----------------------------------------------------------------------------------------------------------------------
Memory assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE    "DynamicArray start index (%d) must be smaller than array end index (%d)"
#define E_ASSERT_MSG_ALGORITHM_SORTING_NETWORK_VALUE  "Sorting network doe not support a size of (%d)"

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Comparer

Please note that this class has the following usage contract: 

1. Comparer defines a customizable comparison template. Comparer is not intended for numerical comparison only but 
for any class type comparison. For this reason this struct is part of Algorithm.h (an not of Comparison.h).
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct Comparer
{
  inline static bool IsEqual(const T& a, const T& b) { return a == b; }
  inline static bool IsLess(const T& a, const T& b) { return a < b; }
};

/*----------------------------------------------------------------------------------------------------------------------
Sorting

Please note that this class has the following usage contract: 

Note: bear in mind that in this class methods are optimized for speed rather than code repetition.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, template<typename = T> class ComparerClass = Comparer>
class Sorting
{
public:
  static void         HeapSort(T* ptr, size_t startIndex, size_t endIndex);
  static void         HeapSort(T* ptr, size_t size);
  static void         InsertionSort(T* ptr, size_t startIndex, size_t endIndex);
  static void         InsertionSort(T* ptr, size_t size);
  static void         IntroSort(T* ptr, size_t startIndex, size_t endIndex);
  static void         IntroSort(T* ptr, size_t size);
  static void         QuickSort(T* ptr, size_t startIndex, size_t endIndex);
  static void         QuickSort(T* ptr, size_t size);
  static void         QuickSort3Way(T* ptr, size_t startIndex, size_t endIndex);
  static void         QuickSort3Way(T* ptr, size_t size);
  static void         QuickSortDualPivot(T* ptr, size_t startIndex, size_t endIndex);
  static void         QuickSortDualPivot(T* ptr, size_t size);

private:
  static const size_t kMinDistanceSize = 13;
  static const size_t kSmallArrayLength = 32;

  static void         IntroSortDepth(T* ptr, size_t startIndex, size_t endIndex, size_t depth);
  static size_t       Median3(T* ptr, size_t startIndex, size_t endIndex);
  static size_t       Partition(T* ptr, size_t startIndex, size_t endIndex);
  inline static void  Swap(T& a, T& b) { if (Comparer<T>::IsEqual(a, b)) return; T tmp = a; a = b; b = tmp; }
  inline static void  SwapIfSmaller(T& a, T& b) { if (!Comparer<T>::IsLess(a, b)) return; T tmp = a; a = b; b = tmp; }
};

/*----------------------------------------------------------------------------------------------------------------------
Sorting methods
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
HeapSort

Implementation based on http://www.codecodex.com/wiki/Heapsort
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::HeapSort(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  // Start "heapifying" the right half of the array
  size_t leftIndex = (endIndex + startIndex + 1) / 2;
  size_t rightIndex = endIndex + 1;
  size_t parentIndex, childIndex;
  T root;

  for (;;)
  {
    // First phase: until the whole array is a heap
    if (leftIndex > startIndex)
    {
      leftIndex--;
      root = ptr[leftIndex];
    }
    // Second phase: sort the array
    else
    {
      if (--rightIndex == startIndex) return;
      // Cache the right-most element to root
      root = ptr[rightIndex];
      // Assign the first element (greatest) to the right-most
      ptr[rightIndex] = ptr[startIndex];
    }  

    // Heapify
    parentIndex = leftIndex;
    childIndex = leftIndex * 2 - startIndex + 1;
    // While there is a child
    while (childIndex < rightIndex)
    {  
      // If there is second child greater than the first one select it
      if (childIndex + 1 < rightIndex  && ComparerClass<T>::IsLess(ptr[childIndex], ptr[childIndex + 1])) childIndex++;
      // If root is smaller than the child candidate
      if (ComparerClass<T>::IsLess(root, ptr[childIndex]))
      {
        // Reorganize the family (promote the chosen child) and use the selected child as new parent
        ptr[parentIndex] = ptr[childIndex];
        parentIndex = childIndex;
        childIndex = parentIndex * 2 - startIndex + 1;
      }
      else
      {
        break;
      }
    }
    // Assign parent to root
    ptr[parentIndex] = root;
  }
}

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::HeapSort(T* ptr, size_t size)
{
  E_ASSERT_MSG(size > 1, E_ASSERT_MSG_MATH_GREATER_THAN_ONE_VALUE);
  HeapSort(ptr, 0, size - 1);
}

/*----------------------------------------------------------------------------------------------------------------------
InsertionSort

Custom implementation improved from http://algs4.cs.princeton.edu/21elementary/
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::InsertionSort(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  for (size_t i = startIndex + 1 ; i <= endIndex; ++i)
  {
    T pivot = ptr[i];
    size_t j = i;
    for (; j > 0 && ComparerClass<T>::IsLess(pivot, ptr[j - 1]); --j) ptr[j] = ptr[j - 1];
    ptr[j] = pivot;
  }
}

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::InsertionSort(T* ptr, size_t size)
{
  E_ASSERT_MSG(size > 1, E_ASSERT_MSG_MATH_GREATER_THAN_ONE_VALUE);
  InsertionSort(ptr, 0, size - 1);
}

/*----------------------------------------------------------------------------------------------------------------------
IntroSort
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::IntroSort(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  // Compute maximum depth aas 2 * log2(length)
  size_t log2 = 0;
  size_t length = endIndex - startIndex + 1;
  for (size_t i = length; i != 0; i >>= 1, ++log2);
  IntroSortDepth(ptr, startIndex, endIndex, 2 * log2);
}

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::IntroSort(T* ptr, size_t size)
{
  E_ASSERT_MSG(size > 1, E_ASSERT_MSG_MATH_GREATER_THAN_ONE_VALUE);
  IntroSort(ptr, 0, size - 1);
}

/*----------------------------------------------------------------------------------------------------------------------
QuickSort

Implementation based on http://algs4.cs.princeton.edu/23quicksort/ and http://iaroslavski.narod.ru/quicksort

1. The algorithm is improved by using Insertion IntroSort in small arrays / partitions
2. The algorithm uses a 3 median pivot selection to improve its efficiency on pseudo-sorted arrays.
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::QuickSort(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  // Switch to insertion sort if size is not greater than the defined size for small arrays
  if ((endIndex - startIndex) < kSmallArrayLength)
  {
    InsertionSort(ptr, startIndex, endIndex);
    return;
  }
  size_t rightIndex = Partition(ptr, startIndex, endIndex);
  // Sort left part if its size is big enough (at least 2 elements)
  if (rightIndex > startIndex + 1) QuickSort(ptr, startIndex, rightIndex - 1);
  // Sort right part if its size is big enough (at least 2 elements)
  if (endIndex > rightIndex + 1) QuickSort(ptr, rightIndex + 1, endIndex); 
}  

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::QuickSort(T* ptr, size_t size)
{
  E_ASSERT_MSG(size > 1, E_ASSERT_MSG_MATH_GREATER_THAN_ONE_VALUE);
  QuickSort(ptr, 0, size - 1);
}

/*----------------------------------------------------------------------------------------------------------------------
QuickSort3Way (3-way partition quick sort)

Implementation based on http://algs4.cs.princeton.edu/23quicksort/ and http://iaroslavski.narod.ru/quicksort

1. The algorithm is improved by using Insertion IntroSort in small arrays / partitions
2. The algorithm uses a 6 median pivot selection to improve its efficiency on pseudo-sorted arrays.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::QuickSort3Way(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  // Switch to insertion sort if size is not greater than the defined size for small arrays
  if ((endIndex - startIndex) < kSmallArrayLength)
  {
    InsertionSort(ptr, startIndex, endIndex);
    return;
  }
  T pivot = ptr[Median3(ptr, startIndex, endIndex)];
  size_t leftIndex = startIndex;
  size_t rightIndex = endIndex;    
  size_t i = startIndex;
  while (i <= rightIndex) 
  {
    if (ComparerClass<T>::IsLess(ptr[i], pivot))
    {
      Swap(ptr[leftIndex++], ptr[i++]);
    }
    else if (ComparerClass<T>::IsLess(pivot, ptr[i]))
    {
      Swap(ptr[rightIndex--], ptr[i]);
    }
    else
    {
      i++;
    }
  }
  // Sort left part if its size is big enough (at least 2 elements)
  if (leftIndex > startIndex + 1) QuickSort3Way(ptr, startIndex, leftIndex - 1);
  // Sort right part if its size is big enough (at least 2 elements)
  if (endIndex > rightIndex + 1) QuickSort3Way(ptr, rightIndex + 1, endIndex);
}

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::QuickSort3Way(T* ptr, size_t size)
{
  E_ASSERT_MSG(size > 1, E_ASSERT_MSG_MATH_GREATER_THAN_ONE_VALUE);
  QuickSort3Way(ptr, 0, size - 1);
}

/*----------------------------------------------------------------------------------------------------------------------
QuickSortDualPivot (dual pivot quick sort)

Implementation based on http://iaroslavski.narod.ru/quicksort

1. The algorithm is improved by using Insertion IntroSort in small arrays / partitions
2. The algorithm uses a 6 median pivot selection to improve its efficiency on pseudo-sorted arrays.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::QuickSortDualPivot(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  // Switch to insertion sort if size is not greater than the defined size for small arrays
  if ((endIndex - startIndex) < kSmallArrayLength)
  {
    InsertionSort(ptr, startIndex, endIndex);
    return;
  }
  // Calculate median indexes
  size_t length = endIndex - startIndex + 1;
  size_t sixth = length / 6;
  size_t m1 = startIndex + sixth;
  size_t m2 = m1 + sixth;
  size_t m3 = m2 + sixth;
  size_t m4 = m3 + sixth;
  size_t m5 = m4 + sixth;
  // Select the pivots using a 5-element sorting network
  SwapIfSmaller(ptr[m2], ptr[m1]);
  SwapIfSmaller(ptr[m5], ptr[m4]);
  SwapIfSmaller(ptr[m3], ptr[m1]);
  SwapIfSmaller(ptr[m3], ptr[m2]);
  SwapIfSmaller(ptr[m4], ptr[m1]);
  SwapIfSmaller(ptr[m4], ptr[m3]);
  SwapIfSmaller(ptr[m5], ptr[m2]);
  SwapIfSmaller(ptr[m3], ptr[m2]);
  SwapIfSmaller(ptr[m5], ptr[m4]);
  T pivot1 = ptr[m2];
  T pivot2 = ptr[m4];
  ptr[m2] = ptr[startIndex];
  ptr[m4] = ptr[endIndex];
  bool diffPivots = pivot1 != pivot2;    
  // Partitioning
  size_t leftIndex = startIndex + 1;
  size_t rightIndex = endIndex - 1;
  if (diffPivots)
  {
    for (size_t k = leftIndex; k <= rightIndex; k++)
    {
      if (ptr[k] < pivot1)
      {
        Swap(ptr[k], ptr[leftIndex++]);
      }
      else if (ptr[k] > pivot2)
      {
        while (ptr[rightIndex] > pivot2 && k < rightIndex) rightIndex--;
        Swap(ptr[k], ptr[rightIndex--]);
        if (ptr[k] < pivot1) Swap(ptr[k], ptr[leftIndex++]);
      }
    }
  }
  else 
  {
    for (size_t k = leftIndex; k <= rightIndex; k++)
    {
      if (ptr[k] == pivot1) continue;
      if (ptr[k] < pivot1)
      {
        Swap(ptr[k], ptr[leftIndex++]);
      }
      else
      {
        while (ptr[rightIndex] > pivot2 && k < rightIndex) rightIndex--;
        Swap(ptr[k], ptr[rightIndex--]);
        if (ptr[k] < pivot1) Swap(ptr[k], ptr[leftIndex++]);
      }
    }
  }
  // Swap pivots
  ptr[startIndex] = ptr[leftIndex - 1];
  ptr[leftIndex - 1] = pivot1;
  ptr[endIndex] = ptr[rightIndex + 1];
  ptr[rightIndex + 1] = pivot2;
  // Sort left part if its size is big enough (at least 2 elements)
  if (leftIndex > startIndex + 2) QuickSortDualPivot(ptr, startIndex, leftIndex - 2);
  // Sort right part if its size is big enough (at least 2 elements)
  if (endIndex > rightIndex + 2) QuickSortDualPivot(ptr, rightIndex + 2, endIndex);
  // IsEqual elements
  if (rightIndex - leftIndex > length - kMinDistanceSize && diffPivots)
  {
    for (size_t k = leftIndex; k <= rightIndex; k++)
    {
      if (ptr[k] == pivot1)
      {
        Swap(ptr[k], ptr[leftIndex++]);
      }
      else if (ptr[k] == pivot2)
      {
        Swap(ptr[k], ptr[rightIndex--]);
        if (ptr[k] == pivot1) Swap(ptr[k], ptr[leftIndex++]);
      }
    }
  }
  // Sort center part if its size is big enough (at least 2 elements)
  if (diffPivots && rightIndex > leftIndex) QuickSortDualPivot(ptr, leftIndex, rightIndex);
}

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::QuickSortDualPivot(T* ptr, size_t size)
{
  E_ASSERT_MSG(size > 1, E_ASSERT_MSG_MATH_GREATER_THAN_ONE_VALUE);
  QuickSortDualPivot(ptr, 0, size - 1);
}   

/*----------------------------------------------------------------------------------------------------------------------
Sorting private methods
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
IntroSort (based on STL)

The June 2000 SGI C++ Standard Template Library stl_algo.h implementation of unstable sort uses the Musser 
introsort approach with the recursion depth to switch to heapsort passed as a parameter, median-of-3 pivot 
selection and the Sedgewick final insertion sort pass. The element threshold for switching to the simple insertion
sort was 16.
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, template<typename> class ComparerClass>
inline void Sorting<T, ComparerClass>::IntroSortDepth(T* ptr, size_t startIndex, size_t endIndex, size_t depth)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  if ((endIndex - startIndex) < kSmallArrayLength)
  {
    InsertionSort(ptr, startIndex, endIndex);
    return;
  }      
  if (depth == 0)
  {
    HeapSort(ptr, startIndex, endIndex);
    return;
  }
  // QuickSort
  size_t rightIndex = Partition(ptr, startIndex, endIndex);
  if (rightIndex > startIndex + 1)
  {
    IntroSortDepth(ptr, startIndex, rightIndex - 1, depth - 1);
  }
  if (endIndex > rightIndex + 1)
  {
    IntroSortDepth(ptr, rightIndex + 1, endIndex, depth - 1);
  } 
}

/*----------------------------------------------------------------------------------------------------------------------
Median3

Implementation based on http://www.csse.monash.edu.au/~lloyd/tildeAlgDS/
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, template<typename> class ComparerClass>
inline size_t Sorting<T, ComparerClass>::Median3(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  // Median indexes
  size_t third = (endIndex - startIndex) / 3;
  size_t m1 = startIndex + third;
  size_t m2 = m1 + third;
  size_t m3 = m2 + third;

  // 3 -element sorting network
  SwapIfSmaller(ptr[m2], ptr[m1]);
  SwapIfSmaller(ptr[m3], ptr[m1]);
  SwapIfSmaller(ptr[m3], ptr[m2]);
  return m2;
}

/*----------------------------------------------------------------------------------------------------------------------
Partition

Implementation based on http://algs4.cs.princeton.edu/23quicksort/
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, template<typename> class ComparerClass>
inline size_t Sorting<T, ComparerClass>::Partition(T* ptr, size_t startIndex, size_t endIndex)
{
  E_ASSERT_MSG(endIndex > startIndex, E_ASSERT_MSG_ALGORITHM_ARRAY_INDICES_VALUE, startIndex, endIndex);
  Swap(ptr[Median3(ptr, startIndex, endIndex)], ptr[startIndex]);
  T pivot = ptr[startIndex];
  size_t leftIndex = startIndex;
  size_t rightIndex = endIndex + 1;
   //while (leftIndex < rightIndex)
  for (;;)
  { 
    // Find item on startIndex to swap
    while (ComparerClass<T>::IsLess(ptr[++leftIndex], pivot)) if (leftIndex == endIndex) break;
    // Find item on endIndex to swap
    while (ComparerClass<T>::IsLess(pivot, ptr[--rightIndex])) continue;
    // Check if left and right indices cross
    if (leftIndex >= rightIndex) break;
    // Swap left and right elements
    Swap(ptr[leftIndex], ptr[rightIndex]);
  }
  // Put partitioning item pivot at rightIndex
  Swap(ptr[startIndex], ptr[rightIndex]);

  return rightIndex;
}
}
}

#endif
