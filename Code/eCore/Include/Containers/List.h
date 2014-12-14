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

// Created 16-Sep-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file List.h
This file defines the List class. List is implemented on top of DynamicArray (base dynamic sequential container)
*/

#ifndef E3_LIST_H
#define E3_LIST_H

#include "DynamicArray.h"
#include <Math/Comparison.h>
#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
List assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_LIST_COUNT_VALUE         "Count value (%d) cannot be greater than size (%d)"
#define E_ASSERT_MSG_LIST_FILL_INDEX_VALUE    "Fill index value must be smaller or equal to the current count"
#define E_ASSERT_MSG_LIST_INDEX_VALUE         "Index value (%d) must be smaller than size (%d)"
#define E_ASSERT_MSG_LIST_ITERATOR_VALUE      "List iterator must be valid"
#define E_ASSERT_MSG_LIST_REMOVE_COUNT_VALUE  "Remove count value must be smaller or equal to the current count"
#define E_ASSERT_MSG_LIST_SIZE_VALUE          "Size value (%d) must respect granularity (%d)"

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (E_INTERNAL_SETTING_LIST_GROWTH_PERCENTAGE)

Please note that this macro has the following usage contract:

1. This macro defines a default growth percentage for List.
2. This is a global library setting macro which can be predefined by the user.
3. This macro value MUST be an natural greater than zero.
4. This macro is internal and CANNOT be defined outside the library as it is used as a template parameter for the 
List class, which is used by other library classes as an argument in some of their methods. You may redefine it at the 
beginning of Base.h instead.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef E_INTERNAL_SETTING_LIST_GROWTH_PERCENTAGE
#define E_INTERNAL_SETTING_LIST_GROWTH_PERCENTAGE 50
#endif

namespace E
{
namespace Containers
{
/*----------------------------------------------------------------------------------------------------------------------
ListBase

ListBase groups all non-template dependent methods from DynamicArray to reduce template code bloat.
----------------------------------------------------------------------------------------------------------------------*/
class ListBase
{
public:
  size_t                    GetCountInternal() const;
  bool                      IsEmptyInternal() const;
  void                      SetCountInternal(size_t count);

protected:
  size_t                    mCount;
};

/*----------------------------------------------------------------------------------------------------------------------
List

Please note that this class has the following usage contract: 

1. The maximum number of elements is 0xffffffff.
2. Valid index values should be passed to methods using an index as argument.
3. Valid iterators should be passed to methods using an iterator as argument.
4. Resize & EnsureSize allocate and copy data while Reserve just allocates memory.
5. Granularity is always respected.
6. Resize always tries to preserves existing data (even when the size value is smaller than the current count).
7. Resize(0) deallocates memory while Clear just sets the element count to 0.
8. Single value PushBack and InsertAt use the growth factor on reallocation while the range value PushBack does not.
9. All addition methods (Copy, InsertAt, PushBack) update the element count (Copy only if the copy count is greater).
10. Operator[] does NOT update the element count (it just provides an array-like access to the list).
11. Fast version of Remove alters the element order in the list.
12. GetBack asserts on empty list.
13. Explicit constructor just allocates memory (in opposition to std::vector)
14. When the list Grow if the usage GrowPercentage incurs in the same size, the size is increased by 1.
15. Clear, PopBack, SetCount and Trim do not modify memory allocation.
16. On resize array elements are always default initialized.
17. Trim checks that given count is smaller than the current.
18. PopBack requires that parameter count is not greater than the current count.
19. Trim and PopBack are complementary.
20. SetCount modifies the current count. SetCount count parameter cannot be greater than the list size. Take note that
SetCount does not remove items. If you want to reduce the count and properly destroy items use Trim or PopBack instead;
otherwise the destruction of trimmed items will be delayed until the destruction of the list instance.

Note: Do not confuse granularity (number of elements which the list size is multiple) with alignment (number of bytes
which the list memory is multiple). The first is controlled by the list, the second by the Memory::IAllocator class used.
However alignment can be defined with granularity in a byte buffer (List<Byte>) when using a non-aligned-memory 
allocator (such the DefaultAllocator class).

Note 2: List is based on DynamicArray which automatically allocates / deallocates the required memory also calling 
items constructor / destructor for non-POD types. This fact ensures that memory is always properly managed every time 
the list resized. On the other hand List removal methods also call the destructor on non-POD types. This behavior is 
correct and necessary. The combination of these facts may incur in a destructor of non-POD items being called twice 
(for list removed objects: first on removal, second on list destruction due to DynamicArray destructor). This double 
call cost is worth the safe memory management and allows specific control methods such as SetCount to be used without 
risking the heap allocation while ensuring proper item destruction.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, size_t Granularity = 1, U8 GrowthPercentage = E_INTERNAL_SETTING_LIST_GROWTH_PERCENTAGE>
class List
{
public:
  // Types
  typedef typename T* Iterator;
  typedef typename const T* ConstIterator;

  // Constants
  static const size_t  kInvalidIndex = static_cast<size_t>(-1); // 0xffffffff

  List();
  explicit List(size_t size);
  List(size_t size, const T& value);
  List(const T* pData, size_t count);
  ~List();

  // Operators
  const T&                  operator [] (size_t index) const;
  T&                        operator [] (size_t index);

  // Accessors
  const Memory::IAllocator* GetAllocator() const;
  ConstIterator             GetBack() const;
  Iterator                  GetBack();
  ConstIterator             GetBegin() const;
  Iterator                  GetBegin();
  size_t                    GetCount() const;
  ConstIterator             GetEnd() const;
  Iterator                  GetEnd();
  size_t                    GetGranularity() const;
  U8                        GetGrowthPercentage() const;
  const T*                  GetPtr() const;
  T*                        GetPtr();
  size_t                    GetSize() const;
  bool                      HasValue(const T& value) const;
  bool                      IsEmpty() const;
  bool                      IsValid(ConstIterator cit) const;
  bool                      IsValid(size_t index) const;
  void                      SetAllocator(Memory::IAllocator* p);
  void                      SetCount(size_t count);

  // Methods
  void                      Clear();
  void                      Compact();
  void                      Copy(const T* pData, size_t count, size_t startIndex = 0);
  void                      EnsureSize(size_t size);
  void                      Fill(const T& value, size_t startIndex = 0);
  ConstIterator             Find(const T& value) const;
  Iterator                  Find(const T& value);
  size_t                    FindIndex(const T& value) const;
  T*                        FindValue(const T& value);
  void                      InsertAt(const T& value, size_t index);
  void                      PopBack(size_t count = 1);
  void                      PushBack(const T& value);
  void                      PushBack(const T* pData, size_t count);
  void                      PushBack(const List& other);
  void                      Remove(Iterator it, size_t count = 1);
  void                      RemoveIndex(size_t index);
  void                      RemoveFast(Iterator cit);
  void                      RemoveIndexFast(size_t index);
  bool                      RemoveIf(const T& value);
  bool                      RemoveIfFast(const T& value);
  void                      Reserve(size_t size);
  void                      Resize(size_t size);
  void                      Trim(size_t count);

private:
  DynamicArray<T>           mData;
  size_t                    mCount;

  void                      Grow();

  // Relying on default copy constructor and assignment operator
};

/*----------------------------------------------------------------------------------------------------------------------
List initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline List<T, Granularity, GrowthPercentage>::List()
  : mCount(0)
{
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline List<T, Granularity, GrowthPercentage>::List(size_t size)
  : mData(Math::CeilMultiple(size, Granularity))
  , mCount(0)
{
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline List<T, Granularity, GrowthPercentage>::List(size_t size, const T& value)
  : mData(Math::CeilMultiple(size, Granularity))
  , mCount(0)
{
  Fill(value);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline List<T, Granularity, GrowthPercentage>::List(const T* pData, size_t count)
  : mData(Math::CeilMultiple(count, Granularity))
  , mCount(count)
{
  mData.Copy(pData, mCount);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline List<T, Granularity, GrowthPercentage>::~List() {}

/*----------------------------------------------------------------------------------------------------------------------
List operators
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline const T& List<T, Granularity, GrowthPercentage>::operator[](size_t index) const
{
  E_ASSERT_MSG(IsValid(index), E_ASSERT_MSG_LIST_INDEX_VALUE, index, mData.GetSize());
  return mData[index];
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline T& List<T, Granularity, GrowthPercentage>::operator[](size_t index)
{
  // No need to check index value (DynamicArray will already do)
  return mData[index];
}

/*----------------------------------------------------------------------------------------------------------------------
List accessors
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline const Memory::IAllocator* List<T, Granularity, GrowthPercentage>::GetAllocator() const
{
  return mData.GetAllocator();
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::ConstIterator List<T, Granularity, GrowthPercentage>::GetBack() const
{
  return (mCount == 0) ? GetEnd() : ConstIterator(&mData[mCount - 1]);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::Iterator List<T, Granularity, GrowthPercentage>::GetBack()
{
  return const_cast<Iterator>(const_cast<const List*>(this)->GetBack());
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::ConstIterator List<T, Granularity, GrowthPercentage>::GetBegin() const
{
  // GetPtr() is used in favor of &mData[0] to avoid calling non-const DynamicArray::operator [] on an empty array (which would assert).
  return mData.GetPtr();
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::Iterator List<T, Granularity, GrowthPercentage>::GetBegin()
{
  return const_cast<Iterator>(const_cast<const List*>(this)->GetBegin());
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline size_t List<T, Granularity, GrowthPercentage>::GetCount() const
{
  return mCount;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::ConstIterator List<T, Granularity, GrowthPercentage>::GetEnd() const
{
  // GetPtr() is used in favor of &mData[0] to avoid calling non-const DynamicArray::operator [] on an empty array (which would assert).
  return mData.GetPtr() + mCount;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::Iterator List<T, Granularity, GrowthPercentage>::GetEnd()
{
  return const_cast<Iterator>(const_cast<const List*>(this)->GetEnd());
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline size_t List<T, Granularity, GrowthPercentage>::GetGranularity() const
{
  return Granularity;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline U8 List<T, Granularity, GrowthPercentage>::GetGrowthPercentage() const
{
  return GrowthPercentage;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline const T* List<T, Granularity, GrowthPercentage>::GetPtr() const
{
  return mData.GetPtr();
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline T* List<T, Granularity, GrowthPercentage>::GetPtr()
{
  return mData.GetPtr();
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline size_t List<T, Granularity, GrowthPercentage>::GetSize() const
{
  return mData.GetSize();
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline bool List<T, Granularity, GrowthPercentage>::HasValue(const T& value) const
{
  return (Find(value) != GetEnd());
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline bool List<T, Granularity, GrowthPercentage>::IsEmpty() const
{
  return (mCount == 0);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline bool List<T, Granularity, GrowthPercentage>::IsValid(ConstIterator cit) const
{
  return (!(cit < GetBegin()) && cit != GetEnd());
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline bool List<T, Granularity, GrowthPercentage>::IsValid(size_t index) const
{
  return (mCount == 0 ? false : index < mCount);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::SetAllocator(Memory::IAllocator* p)
{
  mData.SetAllocator(p);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::SetCount(size_t count)
{
  E_ASSERT_MSG(mData.GetSize() >= count, E_ASSERT_MSG_LIST_COUNT_VALUE, count, mData.GetSize());
  mCount = count;
}

/*----------------------------------------------------------------------------------------------------------------------
List methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Clear()
{
  Memory::Destruct(mData.GetPtr(), mCount);
  mCount = 0;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Compact()
{
  Resize(mCount);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Copy(const T* pData, size_t count, size_t startIndex)
{
  EnsureSize(startIndex + count);
  mData.Copy(pData, count, startIndex);
  mCount = Math::Max(mCount, startIndex + count);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::EnsureSize(size_t size)
{
  if (mData.GetSize() < size) Resize(size);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Fill(const T& value, size_t startIndex)
{
  // Check index value, otherwise we may have undefined values
  E_ASSERT_MSG (mCount >= startIndex, E_ASSERT_MSG_LIST_FILL_INDEX_VALUE);
  mCount = startIndex;
  for (size_t i = mCount; i < mData.GetSize(); ++i) PushBack(value);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::ConstIterator List<T, Granularity, GrowthPercentage>::Find(const T& value) const
{
  size_t i = 0;
  for (i; i < mCount && mData[i] != value; ++i) continue;
  return mData.GetPtr() + i;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::Iterator List<T, Granularity, GrowthPercentage>::Find(const T& value)
{
  return const_cast<Iterator>(const_cast<const List*>(this)->Find(value));
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline size_t List<T, Granularity, GrowthPercentage>::FindIndex(const T& value) const
{
  ConstIterator cit = Find(value);
  return (cit != GetEnd()) ? static_cast<size_t>(cit - GetBegin()) : kInvalidIndex;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline T* List<T, Granularity, GrowthPercentage>::FindValue(const T& value)
{
  Iterator it = Find(value);
  return (it != GetEnd()) ? &*it : nullptr;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::InsertAt(const T& value, size_t index)
{
  if (mCount == mData.GetSize())
  {
    Grow();
  }
  if (index > mCount)
  {
    index = mCount;
  }
  for (size_t i = mCount; i > index; --i)
  {
    mData[i] = mData[i - 1];
  }
  mData[index] = value;
  mCount++;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::PopBack(size_t count)
{
  E_ASSERT_MSG(mCount >= count, E_ASSERT_MSG_LIST_REMOVE_COUNT_VALUE);
  mCount -= count;
  Memory::Destruct(GetEnd(), count);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::PushBack(const T& value)
{
  if (mCount == mData.GetSize())
  {
    Grow();
  }
  mData[mCount++] = value;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::PushBack(const T* pData, size_t count)
{
  EnsureSize(mCount + count);
  for (size_t i = 0; i < count; ++i)
  {
    mData[mCount++] = pData[i];
  }
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::PushBack(const List<T, Granularity, GrowthPercentage>& other)
{
  PushBack(other.GetPtr(), other.mCount);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Remove(Iterator it, size_t count /* = 1 */)
{
  // Note that default Remove uses Iterator as parameter as it modifies the content of the iterator.
  E_ASSERT_MSG(IsValid(it), E_ASSERT_MSG_LIST_ITERATOR_VALUE);
  E_ASSERT_MSG(mCount >= count, E_ASSERT_MSG_LIST_REMOVE_COUNT_VALUE);
  mCount -= count;
  Memory::Move(it, it + count, GetEnd() - it);
  Memory::Destruct(GetEnd(), count);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::RemoveIndex(size_t index)
{
  Remove(&mData[index]);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::RemoveFast(Iterator it)
{
  E_ASSERT_MSG(IsValid(it), E_ASSERT_MSG_LIST_ITERATOR_VALUE);
  *it = mData[--mCount];
  Memory::Destruct(GetEnd());
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::RemoveIndexFast(size_t index)
{
  RemoveFast(&mData[index]);
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline bool List<T, Granularity, GrowthPercentage>::RemoveIf(const T& value)
{
  Iterator it = Find(value);
  if (it != GetEnd())
  {
    Remove(it);
    return true;
  }
  return false;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline bool List<T, Granularity, GrowthPercentage>::RemoveIfFast(const T& value)
{
  Iterator it = Find(value);
  if (it != GetEnd())
  {
    RemoveFast(it);
    return true;
  }
  return false;
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Reserve(size_t size)
{
  mCount = 0;
  mData.Reserve(Math::CeilMultiple(size, Granularity));
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Resize(size_t size)
{
  if (size == 0) 
  {
    mCount = 0;
    mData.Resize(0);
  }
  else
  {
    size = Math::CeilMultiple(size, Granularity);
    if (size  != mData.GetSize())
    {
      DynamicArray<T> temp(size);
      // GetPtr() is used in favor of &mData[0] to avoid calling non-const DynamicArray::operator [] on an empty array (which would assert).
      temp.Copy(mData.GetPtr(), Math::Min(mCount, size));
      mData.Swap(temp);
    }
  }
}

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Trim(size_t count)
{
  if (count < mCount) PopBack(mCount - count);
}

/*----------------------------------------------------------------------------------------------------------------------
List private methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename T, size_t Granularity, U8 GrowthPercentage>
inline void List<T, Granularity, GrowthPercentage>::Grow()
{
  size_t growSize = static_cast<size_t>(mData.GetSize() * (GrowthPercentage + 100) / 100);
  Resize(growSize == mData.GetSize() ? growSize + 1 : growSize);
}

/*----------------------------------------------------------------------------------------------------------------------
STD begin and end expressions for range for loop

In order to make C++11 for range loop begin and end functions must be defined either as:
1. member functions.
2. free functions in the same namespace as the class.
3. std namespace specializations (not to be used by STL types).

For style consistency the second option is chosen making also accessible the auto iteration version of the for loop:

  for (auto it = begin(mList); it != end(mList); ++it) { ... }

----------------------------------------------------------------------------------------------------------------------*/
template <typename T, size_t Granularity, E::U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::ConstIterator begin(const List<T, Granularity, GrowthPercentage>& list) { return list.GetBegin(); }

template <typename T, size_t Granularity, E::U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::Iterator begin(List<T, Granularity, GrowthPercentage>& list) { return list.GetBegin(); }

template <typename T, size_t Granularity, E::U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::ConstIterator end(const List<T, Granularity, GrowthPercentage>& list) { return list.GetEnd(); }

template <typename T, size_t Granularity, E::U8 GrowthPercentage>
inline typename List<T, Granularity, GrowthPercentage>::Iterator end(List<T, Granularity, GrowthPercentage>& list) { return list.GetEnd(); }
}
}

#endif
