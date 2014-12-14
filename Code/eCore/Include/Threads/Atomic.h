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

// Created 08-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Atomic.h
This file defines an atomic template class for thread-safe operations.
*/

#ifndef E3_ATOMIC_H
#define E3_ATOMIC_H

#include "Mutex.h"
#include "Lock.h"

#ifdef E_COMPILER_MSVC
#include "Msvc/AtomicImpl.h"
#endif

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
Atomic

Please note that this class has the following usage contract: 

1. Template argument type must offer the required arithmetic operators.
2. Only the operations provided through Atomic are thread-safe.
3. U32 and U64 specializations implement lock-free methods to maximize performance.
----------------------------------------------------------------------------------------------------------------------*/	
template <typename T>
class Atomic
{
public:
  Atomic() : mX() {}
  Atomic(const T x) : mX(x) {}

  Atomic&       operator=(const Atomic& other)        { Lock l(mMutex); mX = other.mX; return *this; }
  Atomic&       operator=(const T& x)                 { Lock l(mMutex); mX = x; return *this; }
  bool          operator==(const Atomic& other) const { Lock l(mMutex); return mX == other.mX; }
  bool          operator==(const T& x) const          { Lock l(mMutex); return mX == x; }
  bool          operator!=(const Atomic& other) const { Lock l(mMutex); return mX != other.mX; }
  bool          operator!=(const T& x) const          { Lock l(mMutex); return mX != x; }
  T             operator+=(const T& x)                { Lock l(mMutex); mX += x; return mX; }
  T&            operator++()                          { Lock l(mMutex); mX += 1; return mX; }
  T             operator++(I32)                       { Lock l(mMutex); mX += 1; return mX; }
  T             operator-=(const T& x)                { Lock l(mMutex); mX -= x; return mX; }
  T&            operator--()                          { Lock l(mMutex); mX -= 1; return mX; }
  T             operator--(I32)                       { Lock l(mMutex); mX -= 1; return mX;  }
  T             Get() const                           { Lock l(mMutex); return mX; }
  T             Set(const T& x) const                 { Lock l(mMutex); mX = x; }

private:
  mutable Mutex mMutex;
  typename T mX;
};

/*----------------------------------------------------------------------------------------------------------------------
Atomic<U32> specialization
----------------------------------------------------------------------------------------------------------------------*/	
template <>
class Atomic<U32>
{
public:
  Atomic() : mX(0) {}
  Atomic(const U32 x) : mX(x) {}

  Atomic&       operator=(const Atomic& other)        { Impl::StoreRelaxed32(&mX, other.mX); return *this; }
  Atomic&       operator=(const U32 x)                { Impl::StoreRelaxed32(&mX, x); return *this; }
  bool          operator==(const Atomic& other) const { return Impl::LoadRelaxed32(const_cast<U32*>(&mX)) == Impl::LoadRelaxed32(const_cast<U32*>(&other.mX)); }
  bool          operator==(const U32 x) const         { return Impl::LoadRelaxed32(const_cast<U32*>(&mX)) == x; }
  bool          operator!=(const Atomic& other) const { return Impl::LoadRelaxed32(const_cast<U32*>(&mX)) != Impl::LoadRelaxed32(const_cast<U32*>(&other.mX)); }
  bool          operator!=(const U32 x) const         { return Impl::LoadRelaxed32(const_cast<U32*>(&mX)) != x; }
  U32			      operator+=(const U32 x)               { return Impl::AddRelaxed32(&mX, x) + x; }
  U32			      operator++()                          { return Impl::AddRelaxed32(&mX, 1) + 1; }
  U32			      operator++(int)                       { return Impl::AddRelaxed32(&mX, 1) + 1; }
  U32			      operator-=(const U32 x)               { return Impl::AddRelaxed32(&mX, -static_cast<I32>(x)) - 1; }
  U32			      operator--()                          { return Impl::AddRelaxed32(&mX, -1) - 1; }
  U32			      operator--(I32)                       { return Impl::AddRelaxed32(&mX, -1) - 1; }
  U32			      Get() const                           { return Impl::LoadRelaxed32(const_cast<U32*>(&mX)); }
  void          Set(const U32 x)                      { Impl::StoreRelaxed32(&mX, x); }

private:
  U32 mX;
};

/*----------------------------------------------------------------------------------------------------------------------
Atomic<U64> specialization
----------------------------------------------------------------------------------------------------------------------*/	
template <>
class Atomic<U64>
{
public:
  Atomic() : mX(0) {}
  Atomic(const U64 x) : mX(x) {}

  Atomic&       operator=(const Atomic& other)        { Impl::StoreRelaxed64(&mX, other.mX); return *this; }
  Atomic&       operator=(const U64 x)                { Impl::StoreRelaxed64(&mX, x); return *this; }
  bool          operator==(const Atomic& other) const { return Impl::LoadRelaxed64(const_cast<U64*>(&mX)) == Impl::LoadRelaxed64(const_cast<U64*>(&other.mX)); }
  bool          operator==(const U64 x) const         { return Impl::LoadRelaxed64(const_cast<U64*>(&mX)) == x; }
  bool          operator!=(const Atomic& other) const { return Impl::LoadRelaxed64(const_cast<U64*>(&mX)) != Impl::LoadRelaxed64(const_cast<U64*>(&other.mX)); }
  bool          operator!=(const U64 x) const         { return Impl::LoadRelaxed64(const_cast<U64*>(&mX)) != x; }
  U64			      operator+=(const U64 x)               { return Impl::AddRelaxed64(&mX, x) + x; }
  U64			      operator++()                          { return Impl::AddRelaxed64(&mX, 1) + 1; }
  U64			      operator++(int)                       { return Impl::AddRelaxed64(&mX, 1) + 1; }
  U64			      operator-=(const U64 x)               { return Impl::AddRelaxed64(&mX, -static_cast<I64>(x)) - 1; }
  U64			      operator--()                          { return Impl::AddRelaxed64(&mX, -1) - 1; }
  U64			      operator--(I32)                       { return Impl::AddRelaxed64(&mX, -1) - 1; }
  U64			      Get() const                           { return Impl::LoadRelaxed64(const_cast<U64*>(&mX)); }
  void          Set(const U64 x)                      { Impl::StoreRelaxed64(&mX, x); }

private:
  U64 mX;
};
}
/*----------------------------------------------------------------------------------------------------------------------
Atomic types
----------------------------------------------------------------------------------------------------------------------*/
typedef Threads::Atomic<U32> A32;
typedef Threads::Atomic<U64> A64;
}

#endif

