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

/** @file Vector.cpp
This file defines Vector test functions.
*/

#include <CoreTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Vector::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Vector::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Vector::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

//#include "Base.h"

enum Bubba
{
  eMyEnumValue,
};

bool Test::Vector::RunFunctionalityTest()
{
  std::cout << "[Test::Vector::RunFunctionalityTest]" << std::endl;

  /*-------------------------------------------------------------------------------
  Vector2
  -------------------------------------------------------------------------------*/

  /*
  Vector2();
  explicit Vector2(T v);
  Vector2(T x, T y);
  */
  Vector2f v2f;
  Vector2i preSetV2i(1, 2);
  Vector2f preSetV2f(static_cast<F32>(preSetV2i.x), static_cast<F32>(preSetV2i.y));
  E_ASSERT(preSetV2i.x == preSetV2f.x && preSetV2i.y == preSetV2f.y);

  /*
  bool                  operator==(const Vector2<T>& other);
  bool                  operator!=(const Vector2<T>& other);
  Vector2<T>            operator+(const Vector2<T>& other) const;
  Vector2<T>            operator+(T v) const;
  Vector2<T>&           operator+=(const Vector2<T>& other);
  Vector2<T>&           operator+=(T v);
  Vector2<T>            operator-() const;
  Vector2<T>            operator-(const Vector2<T>& other) const;
  Vector2<T>            operator-(T v) const;
  Vector2<T>&	          operator-=(const Vector2<T>& other);
  Vector2<T>&           operator-=(T v);
  Vector2<T>            operator*(const Vector2<T>& other) const;
  Vector2<T>            operator*(T v) const;
  Vector2<T>&           operator*=(const Vector2<T>& other);
  Vector2<T>&           operator*=(T v);
  Vector2<T>            operator/(const Vector2<T>& other) const;
  Vector2<T>            operator/(T v) const;
  Vector2<T>&           operator/=(const Vector2<T>& other);
  Vector2<T>&           operator/=(T v);
  T                     operator^(const Vector2<T>& other) const;
  */
  E_ASSERT(v2f != preSetV2f);
  v2f = preSetV2f;
  E_ASSERT(v2f == preSetV2f);
  Vector2f v2f2 = v2f + v2f;
  v2f2 += v2f;
  v2f2 += 0.0f;

  Vector2f v2f3 = v2f2 - v2f;
  v2f3 -= v2f;
  v2f3 -= 0.0f;
  Vector2f negV2f = -v2f;
  E_ASSERT(-negV2f == v2f);

  E_ASSERT(v2f3 == v2f);
  E_ASSERT(v2f2 == v2f * 3);
  
  Vector2f v2f4 = v2f * v2f;
  v2f4 *= v2f;
  v2f4 *= 1.0f;

  Vector2f v2f5 = v2f4 / v2f;
  v2f5 /= v2f;
  v2f5 /= 1.0f;

  E_ASSERT(v2f5 == v2f);
  E_ASSERT((v2f4 ^ v2f5) < 0.0f);

  /*
  T                     GetLength() const;
  T                     GetLengthSquared() const;
  bool                  IsZero() const;
  void                  SetZero();
  */
  
  //preSetV2i.GetLength() // Not possible!
  Vector2d v2d;
  v2d = Vector2d(v2f.x, v2f.y);
  D64 length2 = v2d. GetLength();
  E_ASSERT(length2 * length2 != v2d.GetLengthSquared());
  E_ASSERT(Math::IsEqual(length2 * length2, v2d.GetLengthSquared()));
  E_ASSERT(!v2f.IsZero());
  v2f.SetZero();
  E_ASSERT(v2f.IsZero());
  v2f = Vector2f(v2f5.x, v2f5.y);
  
  /*
  void                  Normalize();
  void                  Sign();
  static Vector2<T>     AxisX();
  static Vector2<T>     AxisY();
  static T              Cross(const Vector2<T>& v1, const Vector2<T>& v2);
  static T              Dot(const Vector2<T>& v1, const Vector2<T>& v2);
  static void           MinMax(Vector2<T>& min, Vector2<T>& max, const Vector2<T>* pSource, const U32 count);
  */
  E_ASSERT(Vector2f::AxisX().x == 1.0f && Vector2f::AxisX().y == 0.0f);
  E_ASSERT(Vector2f::AxisY().x == 0.0f && Vector2f::AxisY().y == 1.0f);
  E_ASSERT(Vector2f::Cross(v2f, v2f5) == -Vector2f::Cross(v2f5, v2f));
  E_ASSERT(Vector2f::Dot(v2f, v2f) == v2f.GetLengthSquared());
  Vector2d unitV2d = v2d;
  unitV2d /= length2;
  v2d.Normalize();
  E_ASSERT(v2d == unitV2d);
  Vector2d negUnitV2d = unitV2d;
  negUnitV2d.Sign();
  E_ASSERT(negUnitV2d == -unitV2d);

  E::Containers::List<Vector2f> v2fList;
  for (U32 i = 0; i < 100; ++i) 
  {
    v2fList.PushBack(Vector2f(Math::Global::GetRandom().GetF32(), Math::Global::GetRandom().GetF32()));
  }

  Vector2f v2fMin, v2fMax;
  Vector2f::MinMax(v2fMin, v2fMax, v2fList.GetPtr(), static_cast<U32>(v2fList.GetCount()));
  E::Serialization::StringSerializer ss;
  ss << "Min: " << v2fMin.x << "," << v2fMin.y << " Max: " << v2fMax.x << "," << v2fMax.y;
  std::cout << ss.GetPtr() << std::endl;
     
  /*-------------------------------------------------------------------------------
  Vector3
  -------------------------------------------------------------------------------*/

  /*
  Vector3();
  explicit Vector3(T v);
  Vector3(T x, T y, T z);
  */
  Vector3f v3f;
  Vector3f preSetV3f(1.0, 3.0, 7.0);

  /*
  bool                  operator==(const Vector3<T>& other);
  bool                  operator!=(const Vector3<T>& other);
  Vector3<T>            operator+(const Vector3<T>& other) const;
  Vector3<T>            operator+(T v) const;
  Vector3<T>&           operator+=(const Vector3<T>& other);
  Vector3<T>&           operator+=(T v);
  Vector3<T>            operator-() const;
  Vector3<T>            operator-(const Vector3<T>& other) const;
  Vector3<T>            operator-(T v) const;
  Vector3<T>&	          operator-=(const Vector3<T>& other);
  Vector3<T>&           operator-=(T v);
  Vector3<T>            operator*(const Vector3<T>& other) const;
  Vector3<T>            operator*(T v) const;
  Vector3<T>&           operator*=(const Vector3<T>& other);
  Vector3<T>&           operator*=(T v);
  Vector3<T>            operator/(const Vector3<T>& other) const;
  Vector3<T>            operator/(T v) const;
  Vector3<T>&           operator/=(const Vector3<T>& other);
  Vector3<T>&           operator/=(T v);
  T                     operator^(const Vector3<T>& other) const;
  */
  E_ASSERT(v3f != preSetV3f);
  v3f = preSetV3f;
  E_ASSERT(v3f == preSetV3f);
  Vector3f v3f2 = v3f + v3f;
  v3f2 += v3f;
  v3f2 += 0.0f;

  Vector3f v3f3 = v3f2 - v3f;
  v3f3 -= v3f;
  v3f3 -= 0.0f;
  Vector3f negV3f = -v3f;
  E_ASSERT(-negV3f == v3f);

  E_ASSERT(v3f3 == v3f);
  E_ASSERT(v3f2 == v3f * 3);
  
  Vector3f v3f4 = v3f * v3f;
  v3f4 *= v3f;
  v3f4 *= 1.0f;

  Vector3f v3f5 = v3f4 / v3f;
  v3f5 /= v3f;
  v3f5 /= 1.0f;

  E_ASSERT(v3f5 == v3f);
  Vector3f cross = v3f4 ^ v3f5;
  Vector3f cross2 = v3f5 ^ v3f4;
  E_ASSERT(cross == -cross2);

  /*
  T                     GetLength() const;
  T                     GetLengthSquared() const;
  bool                  IsZero() const;
  void                  SetZero();
  */
  
  //preSetV2i.GetLength() // Not possible!
  Vector3d v3d;
  v3d = Vector3d(v3f.x, v3f.y, v3f.z);
  D64 length3 = v3d. GetLength();
  E_ASSERT(length3 * length3 != v3d.GetLengthSquared());
  E_ASSERT(Math::IsEqual(length3 * length3, v3d.GetLengthSquared()));
  E_ASSERT(!v3f.IsZero());
  v3f.SetZero();
  E_ASSERT(v3f.IsZero());
  v3f = Vector3f(v3f5.x, v3f5.y, v3f5.z);
  
  /*
  void                  Normalize();
  void                  Sign();
  static Vector3<T>     AxisX();
  static Vector3<T>     AxisY();
  static Vector3<T>     AxisZ();
  static Vector3<T>     Cross(const Vector3<T>& v1, const Vector3<T>& v2);
  static T              Dot(const Vector3<T>& v1, const Vector3<T>& v2);
  static void           MinMax(Vector3<T>& min, Vector3<T>& max, const Vector3<T>* pSource, const U32 count);
  */

  E_ASSERT(Vector3f::AxisX().x == 1.0f && Vector3f::AxisX().y == 0.0f && Vector3f::AxisX().z == 0.0f);
  E_ASSERT(Vector3f::AxisY().x == 0.0f && Vector3f::AxisY().y == 1.0f && Vector3f::AxisY().z == 0.0f);
  E_ASSERT(Vector3f::AxisZ().x == 0.0f && Vector3f::AxisZ().y == 0.0f && Vector3f::AxisZ().z == 1.0f);
  E_ASSERT(Vector3f::Cross(v3f, v3f5) == -Vector3f::Cross(v3f5, v3f));
  E_ASSERT(Vector3f::Dot(v3f, v3f) == v3f.GetLengthSquared());
  Vector3d unitV3d = v3d;
  unitV3d /= length3;
  v3d.Normalize();
  E_ASSERT(v3d == unitV3d);
  Vector3d negUnitV3d = unitV3d;
  negUnitV3d.Sign();
  E_ASSERT(negUnitV3d == -unitV3d);

  ss.Clear();
  E::Containers::List<Vector3f> v3fList;
  for (U32 i = 0; i < 100; ++i) 
  {
    v3fList.PushBack(Vector3f(Math::Global::GetRandom().GetF32(), Math::Global::GetRandom().GetF32(), Math::Global::GetRandom().GetF32()));
  }
  Vector3f v3fMin, v3fMax;
  Vector3f::MinMax(v3fMin, v3fMax, v3fList.GetPtr(), static_cast<U32>(v3fList.GetCount()));
  ss << "Min: " << v3fMin.x << "," << v3fMin.y << "," << v3fMin.z << " Max: " << v3fMax.x << "," << v3fMax.y << "," << v3fMax.z;
  std::cout << ss.GetPtr() << std::endl;

  /*-------------------------------------------------------------------------------
  Vector4
  -------------------------------------------------------------------------------*/

  /*
  Vector4();
  explicit Vector4(T v);
  Vector4(T x, T y, T z, T w);
  */
  Vector4f v4f;
  Vector4f preSetV4f(1.0f, 4.0f, 7.0f, 67867.0f);
  /*
  bool                  operator==(const Vector4<T>& other);
  bool                  operator!=(const Vector4<T>& other);
  Vector4<T>            operator+(const Vector4<T>& other) const;
  Vector4<T>            operator+(T v) const;
  Vector4<T>&           operator+=(const Vector4<T>& other);
  Vector4<T>&           operator+=(T v);
  Vector4<T>            operator-() const;
  Vector4<T>            operator-(const Vector4<T>& other) const;
  Vector4<T>            operator-(T v) const;
  Vector4<T>&	          operator-=(const Vector4<T>& other);
  Vector4<T>&           operator-=(T v);
  Vector4<T>            operator*(const Vector4<T>& other) const;
  Vector4<T>            operator*(T v) const;
  Vector4<T>&           operator*=(const Vector4<T>& other);
  Vector4<T>&           operator*=(T v);
  Vector4<T>            operator/(const Vector4<T>& other) const;
  Vector4<T>            operator/(T v) const;
  Vector4<T>&           operator/=(const Vector4<T>& other);
  Vector4<T>&           operator/=(T v);
  */
  E_ASSERT(v4f != preSetV4f);
  v4f = preSetV4f;
  E_ASSERT(v4f == preSetV4f);
  Vector4f v4f2 = v4f + v4f;
  v4f2 += v4f;
  v4f2 += 0.0f;

  Vector4f v4f3 = v4f2 - v4f;
  v4f3 -= v4f;
  v4f3 -= 0.0f;
  Vector4f negV4f = -v4f3;
  E_ASSERT(-negV4f == v4f3);

  E_ASSERT(v4f3 == v4f);
  E_ASSERT(v4f2 == v4f * 3);
  
  Vector4f v4f4 = v4f * v4f;
  v4f4 *= v4f;
  v4f4 *= 1.0f;

  Vector4f v4f5 = v4f4 / v4f;
  v4f5 /= v4f;
  v4f5 /= 1.0f;

  /*
  T                     GetLength() const;
  T                     GetLengthSquared() const;
  bool                  IsZero() const;
  void                  SetZero();
  */
  
  //preSetV2i.GetLength() // Not possible!
  Vector4d v4d;
  v4d = Vector4d(v4f.x, v4f.y, v4f.z, v4f.w);
  D64 length4 = v4d. GetLength();
  E_ASSERT(length4 * length4 != v4d.GetLengthSquared());
  E_ASSERT(Math::IsEqual(length4 * length4, v4d.GetLengthSquared(), 1e-5));
  E_ASSERT(!v4f.IsZero());
  v4f.SetZero();
  E_ASSERT(v4f.IsZero());
  v4f = Vector4f(v4f5.x, v4f5.y, v4f5.z, v4f5.w);
  
  /*
  void                  Normalize();
  void                  Sign();
  static Vector4<T>     AxisX();
  static Vector4<T>     AxisY();
  static Vector4<T>     AxisZ();
  static Vector4<T>     AxisW();
  static T              Dot(const Vector4<T>& v1, const Vector4<T>& v2);
  static void           MinMax(Vector4<T>& min, Vector4<T>& max, const Vector4<T>* pSource, const U32 count);
  */
  E_ASSERT(Vector4f::AxisX().x == 1.0f && Vector4f::AxisX().y == 0.0f && Vector4f::AxisX().z == 0.0f && Vector4f::AxisX().w == 0.0f);
  E_ASSERT(Vector4f::AxisY().x == 0.0f && Vector4f::AxisY().y == 1.0f && Vector4f::AxisY().z == 0.0f && Vector4f::AxisY().w == 0.0f);
  E_ASSERT(Vector4f::AxisZ().x == 0.0f && Vector4f::AxisZ().y == 0.0f && Vector4f::AxisZ().z == 1.0f && Vector4f::AxisZ().w == 0.0f);
  E_ASSERT(Vector4f::AxisW().x == 0.0f && Vector4f::AxisW().y == 0.0f && Vector4f::AxisW().z == 0.0f && Vector4f::AxisW().w == 1.0f);
  E_ASSERT(Vector4f::Dot(v4f, v4f) == v4f.GetLengthSquared());
  Vector4d unitV4d = v4d;
  unitV4d /= length4;
  v4d.Normalize();
  E_ASSERT(v4d == unitV4d);
  Vector4d negUnitV4d = unitV4d;
  negUnitV4d.Sign();
  E_ASSERT(negUnitV4d == -unitV4d);

  ss.Clear();
  E::Containers::List<Vector4f> v4fList;
  for (U32 i = 0; i < 100; ++i) 
  {
    v4fList.PushBack(Vector4f(Math::Global::GetRandom().GetF32(), Math::Global::GetRandom().GetF32(), Math::Global::GetRandom().GetF32(), Math::Global::GetRandom().GetF32()));
  }
  Vector4f v4fMin, v4fMax;
  Vector4f::MinMax(v4fMin, v4fMax, v4fList.GetPtr(), static_cast<U32>(v4fList.GetCount()));
  ss << "Min: " << v4fMin.x << "," << v4fMin.y << "," << v4fMin.z << "," << v4fMin.w << " Max: " << v4fMax.x << "," << v4fMax.y << "," << v4fMax.z << "," << v4fMax.w;
  std::cout << ss.GetPtr() << std::endl;

  return true;
}

bool Test::Vector::RunPerformanceTest()
{
  std::cout << "[Test::Vector::RunPerformanceTest]" << std::endl;

  return true;
}
