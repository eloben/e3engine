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

// Created 22-Feb-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file RandomNumberGenerator.h
This file declares the RandomNumberGenerator class for random number generation.
*/

#ifndef E3_RANDOM_H
#define E3_RANDOM_H

#include <Base.h>
#include <Time/Time.h>
#include <Base.h>

namespace E 
{
namespace Math
{
// Forward declarations
class RandomNumberGenerator;

/*----------------------------------------------------------------------------------------------------------------------
Math::Global methods
----------------------------------------------------------------------------------------------------------------------*/
namespace Global
{
  E_API RandomNumberGenerator& GetRandom();
}

/*----------------------------------------------------------------------------------------------------------------------
RandomNumberGenerator

RandomNumberGenerator implements the Mersenne Twister pseudo-random number generator developed in 1997 by Makoto 
Matsumoto and Takuji Nishimura. More info at: http:// en.wikipedia.org/wiki/Mersenne_twister
----------------------------------------------------------------------------------------------------------------------*/
class RandomNumberGenerator
{
public:
  RandomNumberGenerator();

  // Accessors
  bool	GetBool();
  F32		GetF32();
  F32		GetF32(F32 min, F32 max);
  I32		GetI32(I32 min, I32 max);
  U32		GetU32(U32 range);
  void	SetSeed(U32 v);
  U32		GetSeed() const;

  // Methods
  void	RandomizeSeed();

private:
  static const U32 kN = 624;
  static const U32 kM = 397;
  static const U32 kUndefinedIndexValue = 625;
  static const U64 kMatrixA = 0x9908b0df;         // Constant vector a
  static const U64 kUpperMask = 0x80000000;       // Most significant w-r bits
  static const U64 kLowerMask = 0x7fffffff;       // Least significant r bits 
  static const U64 kTemperingMaskB = 0x9d2c5680;  // Most significant w-r bits
  static const U64 kTemperingMaskC = 0xefc60000;  // Least significant r bits
  static const U64 kMaxValue = 0xffffffff; 
  static const U64 kOne = 0x1; 
  static const U64 kZero = 0x0; 
  static const U32 kDefaultInitialSeed = 4357;
  static const U32 kDefaultSeedFactor = 69069;

  U64 	mMt[624];	// the Mersenne twister state vector
  I32		mMtIndex;	// the Mersenne twister state vector index
  U32		mSeed;

  E_DISABLE_COPY_AND_ASSSIGNMENT(RandomNumberGenerator)
};

/*----------------------------------------------------------------------------------------------------------------------
RandomNumberGenerator initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

/** Default constructor.
@throw nothing.
*/
inline RandomNumberGenerator::RandomNumberGenerator()
  : mMtIndex(kUndefinedIndexValue) // This value means that the state vector mMt has not been yet initialized
  , mSeed(0)
{
}

/*----------------------------------------------------------------------------------------------------------------------
RandomNumberGenerator accessors
----------------------------------------------------------------------------------------------------------------------*/	

inline F32 RandomNumberGenerator::GetF32()
{
  F32 random = static_cast<F32> (GetU32(kMaxValue));
  random /= kMaxValue;

  return random;
}

inline F32 RandomNumberGenerator::GetF32(F32 min, F32 max)
{
  F32 random = min + GetF32() * (max - min);

  return random;
}

inline bool RandomNumberGenerator::GetBool()
{
  return (GetU32(2) == 1);
}

inline I32 RandomNumberGenerator::GetI32(I32 min, I32 max)
{
  return static_cast<I64>(min) + static_cast<I64>((static_cast<I64>(max) - static_cast<I64>(min)) * GetF32());
}

/** Generates an U32 random number between 0 and the specified value (not included).
@param range the specified random number generation range.
@return an U32.
@throw nothing.
*/
inline U32 RandomNumberGenerator::GetU32(U32 range)
{
  U32 random = 0;

  if (range != 0)
  {
    U64 y = 0;
    static U64 mag01[2] = {kZero, kMatrixA}; // mag01[x] = x * kMatrixA

    if (mMtIndex >= kN) 
    { 
      I32 kk;

      if (mMtIndex == kUndefinedIndexValue)
        SetSeed(kDefaultInitialSeed);

      for (kk = 0; kk < kN - kM; ++kk) 
      {
        y = (mMt[kk] & kUpperMask) | (mMt[kk + 1] & kLowerMask);
        mMt[kk] = mMt[kk + kM] ^ (y >> 1) ^ mag01[y & kOne];
      }
      for (; kk < kN - 1; ++kk)
      {
        y = (mMt[kk] & kUpperMask) | (mMt[kk + 1] & kLowerMask);
        mMt[kk] = mMt[kk + (kM - kN)] ^ (y >> 1) ^ mag01[y & kOne];
      }

      y = (mMt[kN - 1] & kUpperMask) | (mMt[0] & kLowerMask);
      mMt[kN - 1] = mMt[kM - 1] ^ (y >> 1) ^ mag01[y & kOne];

      mMtIndex = 0;
    }

    y = mMt[mMtIndex++];
    y ^= (y >> 11);
    y ^= (y << 7) & kTemperingMaskB;
    y ^= (y << 15) & kTemperingMaskC;
    y ^= (y >> 18);

    random = (y % range);
  }

  return random;
}

inline U32 RandomNumberGenerator::GetSeed() const
{
  return mSeed;
}

inline void RandomNumberGenerator::SetSeed(U32 v) 
{
  mMt[0] = v & kMaxValue;
  for (mMtIndex = 1; mMtIndex < kN; ++ mMtIndex)
    mMt[mMtIndex] = (kDefaultSeedFactor * mMt[mMtIndex - 1]) & kMaxValue;

  mSeed = v;
}

/*----------------------------------------------------------------------------------------------------------------------
RandomNumberGenerator methods
----------------------------------------------------------------------------------------------------------------------*/

inline void RandomNumberGenerator::RandomizeSeed()
{
  SetSeed(static_cast<U32>(Time::GetCurrentTime().GetMilliseconds()));
}
}
}

#endif
