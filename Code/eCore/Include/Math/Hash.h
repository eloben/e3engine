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

// Created 13-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Hash.h
This file defines the several utility hash functions for 32 and 64 bit values as well as strings.
*/

#ifndef E3_HASH_H
#define E3_HASH_H

#include <Base.h>
#include <Text/String.h>

namespace E
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Djb2

Hash function using the DJB2 algorithm by Professor Daniel J. Bernstein. DJB2 algorithm shown first to the world on
the Usenet newsgroup. This algorithm is very efficient for string hashing.
----------------------------------------------------------------------------------------------------------------------*/
template <typename StringType>
struct Djb2 { static U32 Hash(const StringType& key); };

/*----------------------------------------------------------------------------------------------------------------------
Fnv32

Computes the FNV hash algorithm (Fowler–Noll–Vo), a non-cryptographic hash function for strings created by Glenn 
Fowler, Landon Curt Noll, and Phong Vo. This algorithm is a little slower than Djb2Hash but offers less collision.
----------------------------------------------------------------------------------------------------------------------*/
template <typename StringType>
struct Fnv32 { static U32 Hash(const StringType& key); };

/*----------------------------------------------------------------------------------------------------------------------
Murmur3

General hash-based lookup hash function with good random distribution of keys
Implementation based on: http://code.google.com/p/smhasher/wiki/MurmurHash3
----------------------------------------------------------------------------------------------------------------------*/
template <typename IntegerType>
struct Murmur3 { static IntegerType Hash(IntegerType key); };

/*----------------------------------------------------------------------------------------------------------------------
SimpleHash

Basic hash-based lookup hash function based on the product of a numerical value and a high prime number seed clamped
to a power of 2 range. For this reason the result of this function MUST be modulus operated with a power of 2 value 
minus 1. Note that instead of the % operator the bitwise operator & can be used as an optimization:

U32 hash = SimpleHash<U32>::Hash(key) & (powerOf2Value - 1);

The explanation for the hash computation is simple: it is based on reducing the number of collisions by making the 
constant used in the hash and the number of buckets co-prime. However hash-table implementations have no control 
over the items put into them. For that reason using a power of 2 value for the number of buckets is convenient: we 
save having to preselect a prime number of the right magnitude by relying on the hash function to not use even 
multipliers (2654435761U).
----------------------------------------------------------------------------------------------------------------------*/
template <typename IntegerType>
struct SimpleHash { static IntegerType Hash(IntegerType key); };

/*----------------------------------------------------------------------------------------------------------------------
Djb2 specializations
----------------------------------------------------------------------------------------------------------------------*/

template <>
struct Djb2<String>
{
  static U32 Hash(const String& key)
  {
    U32 hash = 5381;
    for (U32 i = 0; i < key.GetLength(); ++i) hash = ((hash << 5) + hash) + key[i];
    return hash;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
Fnv32 specializations
----------------------------------------------------------------------------------------------------------------------*/

template <>
struct Fnv32<String>
{
  static U32 Hash(const String& key)
  {
    U32 hash = 2166136261;
    for (U32 i = 0; i < key.GetLength(); ++i) hash = (hash ^ key[i]) * 16777619;
    return hash;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
Murmur3 specializations
----------------------------------------------------------------------------------------------------------------------*/

template <>
struct Murmur3<U32>
{
  static U32 Hash(U32 key)
  {
    key ^= key >> 16;
    key *= 0x85ebca6b;
    key ^= key >> 13;
    key *= 0xc2b2ae35;
    key ^= key >> 16;
    return key;
  }
};

template <>
struct Murmur3<U64>
{
  static U64 Hash(U64 key)
  {
    key ^= key >> 33;
    key *= 0xff51afd7ed558ccd;
    key ^= key >> 33;
    key *= 0xc4ceb9fe1a85ec53;
    key ^= key >> 33;
    return key;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
SimpleHash specializations
----------------------------------------------------------------------------------------------------------------------*/

template <>
struct SimpleHash<U32>
{
  static U32 Hash(U32 key)
  {
    return static_cast<U32>(key * 2654435761U);
  }
};

template <>
struct SimpleHash<U64>
{
  static U64 Hash(U64 key)
  {
    return key * 2654435761U;
  }
};
}
}

#endif
