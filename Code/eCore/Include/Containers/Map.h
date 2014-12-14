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

// Created 17-Sep-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Map.h
This file defines the Map class. Map implements linear probing on a power of 2 sized array. This characteristic in 
combination with the modulus operator (performed on the hashed key value by the array size minus one) improves the pair 
distribution over the array, minimizing collision of keys and therefore optimizing the probing. This implementation is 
performs really well in terms of speed with moderate memory usage (1.25 times the map count by default) however Map can 
only be used only with POD types.
Implementation based on: http://preshing.com/20130107/this-hash-table-is-faster-than-a-judy-array/
*/

#ifndef E3_MAP2_H
#define E3_MAP2_H

#include "Pair.h"
#include <Containers/DynamicArray.h>
#include <Math/Hash.h>
#include <Math/Comparison.h>
#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
Map assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_MAP_COUNT_VALUE          "Size (%d) must be equal or greater than element count (%d)"
#define E_ASSERT_MSG_MAP_ITERATOR_VALUE       "Iterator must be valid"
#define E_ASSERT_MSG_MAP_KEY_VALUE            "Key must be valid"
#define E_ASSERT_MSG_MAP_COUNT_MAX_VALUE      "Count second cannot be greater than (%d)"
#define E_ASSERT_MSG_MAP_MAX_OCCUPANCY_VALUE  "Max occupancy percentage value must be between 1 and 100"

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (E_INTERNAL_SETTING_MAP_MAX_OCCUPANCY_PERCENTAGE)

Please note that this macro has the following usage contract:

1. This macro defines a default growth percentage for List.
2. This is a global library setting macro which can be predefined by the user.
3. This macro value MUST be an natural greater than zero.
4. This macro is internal and CANNOT be defined outside the library as it is used as a template parameter for the 
Map class, which is used by other library classes as an argument in some of their methods. You may redefine it at the 
beginning of Base.h instead.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef E_INTERNAL_SETTING_MAP_MAX_OCCUPANCY_PERCENTAGE
#define E_INTERNAL_SETTING_MAP_MAX_OCCUPANCY_PERCENTAGE 75
#endif

namespace E
{
namespace Containers
{
/*----------------------------------------------------------------------------------------------------------------------
MapHasher

Please note that this class has the following usage contract: 

1. MapHasher allows can hash 32 or 64 bit POD types including pointers and String.
2. MapHasher specializations MUST implement the Hash function returning a 32 or 64 bit hash value.
3. MapHasher is public and can be further specialized to include other hashable map types.

Note that this template is just that: a template. Specializations may differ in method definition (Which is legal).
Note also that the Hash method always returns a size_t type (even in specializations). This is due to the fact that
the array will use a 32 or 64 bit pointer depending on the platform, so the index size_t will be 32 or 64 bit also 
depending on the platform, therefore it does not make sense for the Hash method to return an U64 if it is going to
be translated to a 32 bit index array in a 32 bit platform.
----------------------------------------------------------------------------------------------------------------------*/
template <typename KeyType, size_t KeySize = sizeof(KeyType)>
struct MapHasher 
{
  static size_t Hash(const KeyType& key);
  static void Invalidate(KeyType& key);
  static bool IsEqual(const KeyType& key1, const KeyType& key2);
  static bool IsValid(const KeyType& key);
};

/*----------------------------------------------------------------------------------------------------------------------
Map

Please note that this class has the following usage contract: 

1. Valid iterators should be passed to methods using an Iterator as argument.
2. Clear, Compact, Remove and Resize methods invalidate iterators.
3. Resize method requires a power of 2 size value.
4. Resize preserves existing data and will E_ASSERT_MSG when the size is smaller than the current count.
5. Resize does not respect growth factor while Compact does.
6. Resize with a size of 0 calls Clear and deallocates the map memory.
7. Compact on an empty map has the same effects a Resize(0).
8. The maximum (prudential) number of elements is 0x7fffffff (max value for I32).
9. The maximum occupancy percentage must be a value between 1 and 100.
10. The operator [] does NOT insert, they just retrieve the object if existent (use Insert for that purpose).
11. The minimum allocation size is defined by kDefaultMinSize
12. Insert returns true when a new pair is created otherwise returns false (only value modification).
13. RemoveIf returns true on success and false otherwise.
14. Every time a key is invalidated its pair value destructor is called.
15. The class does not let specializing HasherClass from outside:
'template<typename = KeyType> class HasherClass = MapHasher' vs 'typename HasherClass = MapHasher<KeyType>'

Note that FindPair is faster than Find as it does not require iterator instancing. This map is optimized for speed
while also allowing iteration. For this reason it is recommended to use iterators when iteration is needed.
----------------------------------------------------------------------------------------------------------------------*/
template <
  typename KeyType, 
  typename ValueType = KeyType, 
  template<typename KeyType, size_t = sizeof(KeyType)> class HasherClass = MapHasher, 
  U8 MaxOccupancyPercentage = E_INTERNAL_SETTING_MAP_MAX_OCCUPANCY_PERCENTAGE>
class Map
{
public:
  typedef Pair<KeyType, ValueType>  Pair;
  typedef HasherClass<KeyType>      Hasher;

  /*----------------------------------------------------------------------------------------------------------------------
  ConstIterator
  ----------------------------------------------------------------------------------------------------------------------*/
  class ConstIterator
  {
  public:
    typedef ConstIterator   ThisType;
    typedef const Pair&     Reference;
    typedef ptrdiff_t       DifferenceType;

    ConstIterator(Pair* pCurrent, const Pair* pEnd) : mpCurrent(pCurrent), mpEnd(pEnd) {}
    inline Reference  operator*() const { return *mpCurrent; }
    inline bool       operator==(const ThisType& other) const { return mpCurrent == other.mpCurrent; }
    inline bool       operator!=(const ThisType& other) const { return mpCurrent != other.mpCurrent; }
    inline bool       operator<(const ThisType& other) const  { return mpCurrent < other.mpCurrent; }
    inline ThisType&  operator++()                            { while (++mpCurrent != mpEnd && !Hasher::IsValid(mpCurrent->first)) continue; return *this; }
    DifferenceType    operator-(const ThisType& other) const  { return mpCurrent - other.mpCurrent; }

  protected:
    Pair*             mpCurrent;
    const Pair*       mpEnd;
  };

  /*----------------------------------------------------------------------------------------------------------------------
  Iterator
  ----------------------------------------------------------------------------------------------------------------------*/
  class Iterator : public ConstIterator
  {
  public:
    typedef Iterator  ThisType;
    typedef Pair&     Reference;

    Iterator(Pair* pCurrent, const Pair* pEnd) : ConstIterator(pCurrent, pEnd) {}
    inline Reference  operator*() const { return *mpCurrent; }
  };

  explicit Map(size_t initialSize = kDefaultMinSize);
  ~Map();

  const ValueType&          operator[](KeyType key) const;
  ValueType&                operator[](KeyType key);

  // Basic operations
  const Memory::IAllocator* GetAllocator() const;
  ConstIterator             GetBegin() const;
  Iterator                  GetBegin();
  size_t                    GetCount() const;
  ConstIterator             GetEnd() const;
  Iterator                  GetEnd();
  U8                        GetMaxOccupancyPercentage() const;
  size_t                    GetSize() const;
  bool                      HasKey(KeyType key) const;
  bool                      IsEmpty() const;
  bool                      IsValid(ConstIterator cit) const;
  void                      SetAllocator(Memory::IAllocator* p);

  void                      Clear();
  void                      Compact();
  ConstIterator             Find(KeyType key) const;
  Iterator                  Find(KeyType key);
  const Pair*               FindPair(KeyType key) const;
  Pair*                     FindPair(KeyType key);
  Pair*                     Insert(KeyType key, ValueType value);
  void                      Remove(Iterator it);
  bool                      RemoveIf(KeyType key);
  void                      RemovePair(Pair* pPair);
  void                      Resize(size_t size);

private:
  DynamicArray<Pair>        mData;
  size_t                    mCount;
  static const U32          kMaxElementCount = 0x7fffffff; // Max I32 value
  static const U8           kDefaultMinSize = 8;

  Pair*                     GetBeginPair() const;
  Pair*                     GetIdealPair(KeyType key) const;
  Pair*                     GetEndPair() const;
  Pair*                     GetNextPair(Pair* pPair) const;
  size_t                    GetPairDistance(Pair* pLeft, Pair* pRight) const;
};

/*----------------------------------------------------------------------------------------------------------------------
Map initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/
template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Map(size_t initialSize)
  : mData(initialSize)
  , mCount(0)
{
  // Initialize regular cells
  E_ASSERT_MSG(Math::IsPower2(initialSize), E_ASSERT_MSG_MATH_POWER_OF_TWO_VALUE);
  for (U32 i = 0; i < mData.GetSize(); ++i) Hasher::Invalidate(mData[i].first);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::~Map() {}

/*----------------------------------------------------------------------------------------------------------------------
Map operators
----------------------------------------------------------------------------------------------------------------------*/
template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline const ValueType& Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::operator[](KeyType key) const
{
  const ValueType* pValue = FindValue(key);
  E_ASSERT_MSG(pValue, E_ASSERT_MSG_MAP_KEY_VALUE);
  return *pValue;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline ValueType& Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::operator[](KeyType key)
{
  return const_cast<ValueType&>(static_cast<const Map*>(this)->operator[](key));
}

/*----------------------------------------------------------------------------------------------------------------------
Map accessors
----------------------------------------------------------------------------------------------------------------------*/
template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline const Memory::IAllocator* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetAllocator() const
{
  return mData.GetAllocator();
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::ConstIterator Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetBegin() const
{
  return ConstIterator(GetBeginPair(), GetEndPair());
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Iterator Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetBegin()
{
  return Iterator(GetBeginPair(), GetEndPair());
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline size_t Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetCount() const
{
  return mCount;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::ConstIterator Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetEnd() const
{
  Pair* pEnd = GetEndPair();
  return ConstIterator(pEnd, pEnd);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Iterator Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetEnd()
{
  Pair* pEnd = GetEndPair();
  return Iterator(pEnd, pEnd);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline U8 Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetMaxOccupancyPercentage() const
{
  return MaxOccupancyPercentage;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline size_t Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetSize() const
{
  return mData.GetSize();
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline bool Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::HasKey(KeyType key) const
{
  return FindPair(key) != nullptr;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline bool Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::IsEmpty() const
{
  return (mCount == 0);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline bool Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::IsValid(ConstIterator cit) const
{
  return (!(cit < GetBegin()) && cit != GetEnd());
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline void Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::SetAllocator(Memory::IAllocator* p)
{
  mData.SetAllocator(p);
}

/*----------------------------------------------------------------------------------------------------------------------
Map methods
----------------------------------------------------------------------------------------------------------------------*/
template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline void Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Clear()
{
  for (U32 i = 0; i < mData.GetSize(); ++i)
  {
    if (Hasher::IsValid(mData[i].first))
    {
      Memory::Destruct(&mData[i].second);
      Hasher::Invalidate(mData[i].first);
    }
  }
  mCount = 0;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline void Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Compact()
{
  Resize(mCount ? Math::CeilPowerOf2((mCount * 100 + MaxOccupancyPercentage) / MaxOccupancyPercentage) : 0);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::ConstIterator Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Find(KeyType key) const
{
  Pair* pPair = const_cast<Pair*>(FindPair(key));
  Pair* pEnd = GetEndPair();
  if (!pPair) pPair = pEnd;
  return ConstIterator(pPair, pEnd);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Iterator Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Find(KeyType key)
{
  Pair* pPair = const_cast<Pair*>(FindPair(key));
  Pair* pEnd = GetEndPair();
  if (!pPair) pPair = pEnd;
  return Iterator(pPair, pEnd);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename const Pair<KeyType, ValueType>* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::FindPair(KeyType key) const
{
  E_ASSERT_MSG(Hasher::IsValid(key), E_ASSERT_MSG_MAP_KEY_VALUE);
  if (mData.GetSize() && Hasher::IsValid(key))
  {
    for (Pair* pPair = GetIdealPair(key); Hasher::IsValid(pPair->first); pPair = GetNextPair(pPair))
    {
      if (Hasher::IsEqual(pPair->first, key)) return pPair;
    }
  }
  return nullptr;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Pair<KeyType, ValueType>* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::FindPair(KeyType key)
{
  return const_cast<Pair*>(const_cast<const Map*>(this)->FindPair(key));
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Pair<KeyType, ValueType>* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Insert(KeyType key, ValueType value)
{
  E_ASSERT_MSG(Hasher::IsValid(key), E_ASSERT_MSG_MAP_KEY_VALUE);
  E_ASSERT_MSG(mCount <= kMaxElementCount, E_ASSERT_MSG_MAP_COUNT_MAX_VALUE, kMaxElementCount);
  if (mData.GetSize() == 0) Resize(kDefaultMinSize);

  for (;;)
  {
    for (Pair* pPair = GetIdealPair(key); ; pPair = GetNextPair(pPair))
    {
      // Existing key
      if (Hasher::IsEqual(pPair->first, key))
      {
        pPair->second = value;
        return pPair;
      }
      if (!Hasher::IsValid(pPair->first))
      {
        // Check current size
        if ((mCount + 1) * 100 >= mData.GetSize() * MaxOccupancyPercentage)
        {
          Resize(mData.GetSize() * 2);
          break;
        }
        // Insert new pair
        ++mCount;
        pPair->first = key;
        pPair->second = value;
        return pPair;
      }
    }
  }
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline void Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Remove(Iterator it)
{
  E_ASSERT_MSG(IsValid(it), E_ASSERT_MSG_MAP_ITERATOR_VALUE);
  RemovePair(&*it);
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline bool Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::RemoveIf(KeyType key)
{
  Pair* pPair = FindPair(key);
  if (pPair)
  {
    RemovePair(pPair);
    return true;
  }
  return false;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline void Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::RemovePair(Pair* pPair)
{
  // Remove from regular cells
  E_ASSERT(pPair >= mData.GetPtr() && static_cast<size_t>(pPair - mData.GetPtr()) < mData.GetSize());
  E_ASSERT(Hasher::IsValid(pPair->first));

  // Remove this pPair by shuffling neighboring cells so there are no gaps in anyone's probe chain
  for (Pair* neighbor = GetNextPair(pPair);; neighbor = GetNextPair(neighbor))
  {
    if (!Hasher::IsValid(neighbor->first))
    {
      // There's nobody to swap with. Go ahead and clear this pPair, then return
      Memory::Destruct(&pPair->second);
      Hasher::Invalidate(pPair->first);
      pPair->second = 0;
      mCount--;
      return;
    }
    Pair* ideal = GetIdealPair(neighbor->first);
    if (GetPairDistance(ideal, pPair) < GetPairDistance(ideal, neighbor))
    {
      // Swap with neighbor, then make neighbor the new pPair to remove.
      *pPair = *neighbor;
      pPair = neighbor;
    }
  }
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline void Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Resize(size_t size)
{
  if (size == 0)
  {
    mData.Resize(0);
    mCount = 0;
    return;
  }
  E_ASSERT_MSG(Math::IsPower2(size), E_ASSERT_MSG_MATH_POWER_OF_TWO_VALUE);
  E_ASSERT_MSG(size >= mCount, E_ASSERT_MSG_MAP_COUNT_VALUE, size, mCount);
  if (size != mData.GetSize())
  {
    // Swap current array with an array of the new size
    DynamicArray<Pair> temp(size);
    mData.Swap(temp);
    for (U32 i = 0; i < mData.GetSize(); ++i) Hasher::Invalidate(mData[i].first);

    // Iterate through old array
    Pair* oldBegin = temp.GetPtr();
    Pair* oldEnd = oldBegin + temp.GetSize();
    for (Pair* oldPair = oldBegin; oldPair != oldEnd; ++oldPair)
    {
      if (Hasher::IsValid(oldPair->first))
      {
        // Insert oldPair into current array
        for (Pair* pPair = GetIdealPair(oldPair->first); ; pPair = GetNextPair(pPair))
        {
          if (!Hasher::IsValid(pPair->first))
          {
            *pPair = *oldPair;
            break;
          }
        }
      }
    }
  }
}

/*----------------------------------------------------------------------------------------------------------------------
Map private methods
----------------------------------------------------------------------------------------------------------------------*/

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Pair* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetBeginPair() const
{
  Pair* pBegin = const_cast<Pair*>(mData.GetPtr());
  while (pBegin != GetEndPair() && !Hasher::IsValid(pBegin->first)) ++pBegin;
  return pBegin;
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Pair* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetEndPair() const
{
  return const_cast<Pair*>(mData.GetPtr()) + mData.GetSize();
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Pair* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetIdealPair(KeyType key) const
{
  return const_cast<Pair*>(mData.GetPtr() + (Hasher::Hash(key) & (mData.GetSize() - 1)));
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Pair* Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetNextPair(Pair* pPair) const
{
  return (pPair + 1 != GetEndPair()) ? pPair + 1 : const_cast<Pair*>(mData.GetPtr());
}

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, U8 MaxOccupancyPercentage>
inline size_t Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::GetPairDistance(Pair* pLeft, Pair* pRight) const 
{
  return (pLeft < pRight) ? pRight + mData.GetSize() - pLeft : pRight - pLeft;
}

/*----------------------------------------------------------------------------------------------------------------------
MapHasher specializations
----------------------------------------------------------------------------------------------------------------------*/
template <typename KeyType>
struct MapHasher<KeyType, sizeof(U32)>
{
  inline static size_t  Hash(KeyType key)                         { return Math::Murmur3<U32>::Hash(key); }
  inline static void    Invalidate(KeyType& key)                  { key = KeyType(-1); }
  inline static bool    IsEqual(KeyType key1, KeyType key2)       { return key1 == key2; }
  inline static bool    IsValid(KeyType key)                      { return key != KeyType(-1);  }
};

template <typename KeyType>
struct MapHasher<KeyType, sizeof(U64)>
{
  inline static size_t  Hash(KeyType key)                         { return static_cast<size_t>(Math::Murmur3<U64>::Hash(key)); }
  inline static void    Invalidate(KeyType& key)                  { key = KeyType(-1); }
  inline static bool    IsEqual(KeyType key1, KeyType key2)       { return key1 == key2; }
  inline static bool    IsValid(KeyType key)                      { return key != KeyType(-1);  }
};

template <typename KeyType>
struct MapHasher<KeyType*, sizeof(U32)>
{
  typedef KeyType* PointerType;
  inline static size_t  Hash(PointerType key)                       { return Math::Murmur3<U32>::Hash((U32)key); }
  inline static void    Invalidate(PointerType& key)                { key = PointerType(-1); }
  inline static bool    IsEqual(PointerType key1, PointerType key2) { return key1 == key2; }
  inline static bool    IsValid(PointerType key)                    { return key != PointerType(-1);  }
};

template <typename KeyType>
struct MapHasher<KeyType*, sizeof(U64)>
{
  typedef KeyType* PointerType;
  inline static size_t  Hash(PointerType key)                       { return Math::Murmur3<U64>::Hash((U64)key); }
  inline static void    Invalidate(PointerType& key)                { key = PointerType(-1); }
  inline static bool    IsEqual(PointerType key1, PointerType key2) { return key1 == key2; }
  inline static bool    IsValid(PointerType key)                    { return key != PointerType(-1);  }
};

template <>
struct MapHasher<String>
{
  inline static size_t  Hash(const String& key)                               { return Math::Djb2<String>::Hash(key); }
  inline static void    Invalidate(String& key)                               { key.Clear(); }
  inline static bool    IsEqual(const String& key1, const String& key2) { return key1 == key2; }
  inline static bool    IsValid(const String& key)                            { return key.GetLength() != 0; }
};

/*----------------------------------------------------------------------------------------------------------------------
STD begin and end expressions for range for loop

In order to make C++11 for range loop begin and end functions must be defined either as:
1. member functions.
2. free functions in the same namespace as the class.
3. std namespace specializations (not to be used by STL types).

For style consistency the second option is chosen making also accessible the auto iteration version of the for loop:

for (auto it = begin(mList); it != end(mList); ++it) { ... }

----------------------------------------------------------------------------------------------------------------------*/
template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, E::U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::ConstIterator 
begin(const Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>& map) { return map.GetBegin(); }

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, E::U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Iterator 
begin(Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>& map) { return map.GetBegin(); }

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, E::U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::ConstIterator 
end(const Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>& map) { return map.GetEnd(); }

template <typename KeyType, typename ValueType, template<typename, size_t> class HasherClass, E::U8 MaxOccupancyPercentage>
inline typename Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>::Iterator 
end(Map<KeyType, ValueType, HasherClass, MaxOccupancyPercentage>& map) { return map.GetEnd(); }
}
}

#endif
