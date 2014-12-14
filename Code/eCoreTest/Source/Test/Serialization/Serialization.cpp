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

/** @file Serialization.cpp
This file defines Serialization test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace E::Serialization;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Class definitions
----------------------------------------------------------------------------------------------------------------------*/

struct XmlNodeA
{
  I32 i;
  F32 f;
  E::String s;
  
  inline friend ISerializer& operator<<(ISerializer& target, const XmlNodeA& source)
  {
    E_FRIEND_SERIALIZE(i);
    E_FRIEND_SERIALIZE(f);
    E_FRIEND_SERIALIZE(s);
    return target;
  }

  inline friend ISerializer& operator>>(ISerializer& source, XmlNodeA& target)
  {
    E_FRIEND_DESERIALIZE(i);
    E_FRIEND_DESERIALIZE(f);
    E_FRIEND_DESERIALIZE(s);
    return source;
  }
};

struct XmlNodeB
{
  XmlNodeA a;
  U8 c;
  E::StringBuffer s;
  bool b;

  inline friend ISerializer& operator<<(ISerializer& target, const XmlNodeB& source)
  {
    E_FRIEND_SERIALIZE(a);
    E_FRIEND_SERIALIZE(c);
    E_FRIEND_SERIALIZE(s);
    E_FRIEND_SERIALIZE(b);
    return target;
  }

  inline friend ISerializer& operator>>(XmlSerializer& source, XmlNodeB& target)
  {
    E_FRIEND_DESERIALIZE(a);
    E_FRIEND_DESERIALIZE(c);
    E_FRIEND_DESERIALIZE(s);
    E_FRIEND_DESERIALIZE(b);
    return source;
  }
};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Serialization::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Serialization::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Serialization::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Serialization::RunFunctionalityTest()
{
  std::cout << "[Test::Serialization::RunFunctionalityTest]" << std::endl;

  std::cout << std::endl;
  E::Containers::List<F32> fList;
  for (U32 i = 0; i < 10; ++i)
  {
    F32 v = Math::Global::GetRandom().GetF32();
    fList.PushBack(v);
    std::cout << v << std::endl;
  }
 
  /*----------------------------------------------------------------------------------------------------------------------
  ByteSerializer
  ----------------------------------------------------------------------------------------------------------------------*/
  // Sequential serialization 
  ByteSerializer ar;
  std::cout << std::endl;
  for (E::Containers::List<F32>::ConstIterator cit = fList.GetBegin(); cit != fList.GetEnd(); ++cit)
  {
    ar << *cit;
  }

  for (U32 i = 0; i < 10; ++i)
  {
    F32 v = 0.0f;
    ar >> v;
    std::cout << v << std::endl;
  }

  // Chained serialization 
  std::cout << std::endl;

  ByteSerializer ar2;
  ar2 << fList[0] << fList[1] << fList[2] << fList[3] << fList[4];
 
  for (U32 i = 0; i < 5; ++i)
  {
    F32 v = 0.0f;
    ar2 >> v;
    std::cout << v << std::endl;
  }
  
  // Type serialization
  std::cout << std::endl;
  I8 i8   = (I8)Math::Global::GetRandom().GetI32(Math::NumericLimits<I8>::Min(), Math::NumericLimits<I8>::Max());
  U8 u8   = (U8)Math::Global::GetRandom().GetU32(Math::NumericLimits<U8>::Max());
  I16 i16 = (I16)Math::Global::GetRandom().GetI32(Math::NumericLimits<I16>::Min(), Math::NumericLimits<I16>::Max());
  U16 u16 = (U16)Math::Global::GetRandom().GetU32(Math::NumericLimits<U16>::Max());
  I32 i32 = Math::Global::GetRandom().GetI32(Math::NumericLimits<I32>::Min(), Math::NumericLimits<I32>::Max());
  U32 u32 = Math::Global::GetRandom().GetU32(Math::NumericLimits<U32>::Max());
  I64 i64 = (I64)Math::Global::GetRandom().GetI32(Math::NumericLimits<I32>::Min(), Math::NumericLimits<I32>::Max()) * 84089L;
  U64 u64 = (U64)Math::Global::GetRandom().GetU32(Math::NumericLimits<U32>::Max()) * 84089UL;

  std::cout << i8 << std::endl;
  std::cout << u8 << std::endl;
  std::cout << i16 << std::endl;
  std::cout << u16 << std::endl;
  std::cout << i32 << std::endl;
  std::cout << u32 << std::endl;
  std::cout << i64 << std::endl;
  std::cout << u64 << std::endl;

  ByteSerializer ar3;

  ar3 << true;
  ar3 << "A wonderful string!";
  ar3 << i8 << u8 << i16 << u16;
  ar3 << i32 << u32 << i64 << u64;
  
  bool flag;
  E::StringBuffer s;

  ar3.SetBegin();
  ar3 >> flag;
  ar3 >> s;
  ar3 >> i8;
  ar3 >> u8;
  ar3 >> i16;
  ar3 >> u16;
  ar3 >> i32;
  ar3 >> u32;
  ar3 >> i64;
  ar3 >> u64;
    
  std::cout << std::endl;
  std::cout << flag << std::endl;
  std::cout << s.GetPtr() << std::endl;
  std::cout << i8 << std::endl;
  std::cout << u8 << std::endl;
  std::cout << i16 << std::endl;
  std::cout << u16 << std::endl;
  std::cout << i32 << std::endl;
  std::cout << u32 << std::endl;
  std::cout << i64 << std::endl;
  std::cout << u64 << std::endl;

  // Interface test
  ISerializer* pAr = &ar;
  *pAr << "Amazing thing to use the ISerializer interface!";
  *pAr >> s;
  std::cout << s.GetPtr() << std::endl;

  /*----------------------------------------------------------------------------------------------------------------------
  StringSerializer
  ----------------------------------------------------------------------------------------------------------------------*/
  
  // Type serialization
  std::cout << std::endl;
  i8   = (I8)Math::Global::GetRandom().GetI32(Math::NumericLimits<I8>::Min(), Math::NumericLimits<I8>::Max());
  u8   = (U8)Math::Global::GetRandom().GetU32(Math::NumericLimits<U8>::Max());
  i16 = (I16)Math::Global::GetRandom().GetI32(Math::NumericLimits<I16>::Min(), Math::NumericLimits<I16>::Max());
  u16 = (U16)Math::Global::GetRandom().GetU32(Math::NumericLimits<U16>::Max());
  i32 = Math::Global::GetRandom().GetI32(Math::NumericLimits<I32>::Min(), Math::NumericLimits<I32>::Max());
  u32 = Math::Global::GetRandom().GetU32(Math::NumericLimits<U32>::Max());
  i64 = (I64)Math::Global::GetRandom().GetI32(Math::NumericLimits<I32>::Min(), Math::NumericLimits<I32>::Max()) * 84089L;
  u64 = (U64)Math::Global::GetRandom().GetU32(Math::NumericLimits<U32>::Max()) * 84089UL;

  std::cout << i8 << std::endl;
  std::cout << u8 << std::endl;
  std::cout << i16 << std::endl;
  std::cout << u16 << std::endl;
  std::cout << i32 << std::endl;
  std::cout << u32 << std::endl;
  std::cout << i64 << std::endl;
  std::cout << u64 << std::endl;

  E::Serialization::StringSerializer ss;

  ss << true;
  ss << "A wonderful string!";
  ss << i8 << u8 << i16 << u16;
  ss << i32 << u32 << i64 << u64;

  std::cout << std::endl;
  std::cout << "[" << ss.GetPtr() << "]" << std::endl;

  ss >> flag;
  ss >> s;
  ss >> i8;
  ss >> u8;
  ss >> i16;
  ss >> u16;
  ss >> i32;
  ss >> u32;
  ss >> i64;
  ss >> u64;

  std::cout << std::endl;
  std::cout << flag << std::endl;
  std::cout << s.GetPtr() << std::endl;
  std::cout << i8 << std::endl;
  std::cout << u8 << std::endl;
  std::cout << i16 << std::endl;
  std::cout << u16 << std::endl;
  std::cout << i32 << std::endl;
  std::cout << u32 << std::endl;
  std::cout << i64 << std::endl;
  std::cout << u64 << std::endl;
  std::cout << std::endl;
  
  /*----------------------------------------------------------------------------------------------------------------------
  XmlSerializer
  ----------------------------------------------------------------------------------------------------------------------*/

  XmlNodeB b, b2;

  b.c = 33;
  b.a.f = 11;
  b.a.i = 22;
  b.a.s = "A static string";
  b.s = "A dynamic marvelous string";
  b.b = true;

  b2.c = 255;
  b2.a.f = 1102.343f;
  b2.a.i = 2202;
  b2.a.s = "A static string";
  b2.s = "A dynamic marvelous string";
  b2.b = false;

  std::cout << b.c << " " << b.a.f << " " << b.a.i << " " << b.a.s.GetPtr() << std::endl;
  std::cout << b2.c << " " << b2.a.f << " " << b2.a.i << " " << b2.a.s.GetPtr() << std::endl;

  XmlSerializer sz;

  E_SERIALIZE(sz, b, b);
  E_SERIALIZE(sz, b2, b2);

  XmlNodeB bb, bb2;

  E_DESERIALIZE(sz, bb, b);
  E_DESERIALIZE(sz, bb2, b2);

  std::cout << bb.c << " " << bb.a.f << " " << bb.a.i << " " << bb.a.s.GetPtr() << std::endl;
  std::cout << bb2.c << " " << bb2.a.f << " " << bb2.a.i << " " << bb2.a.s.GetPtr() << std::endl;

  E::StringBuffer sss1, sss2;
  sss1 = "A";
  sss2 = "A";

  sz.Save("../../../Bin/xmlFile.xml");
  std::cout << std::endl;

  return true;
}

bool Test::Serialization::RunPerformanceTest()
{
  std::cout << "[Test::Serialization::RunPerformanceTest]" << std::endl;

  return true;
}
