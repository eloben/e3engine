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

/** @file Quaternion.cpp
This file defines Quaternion test functions.
*/

#include <CoreTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Quaternion::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Quaternion::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Quaternion::RunPerformanceTest");

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

bool Test::Quaternion::RunFunctionalityTest()
{
  std::cout << "[Test::Quaternion::RunFunctionalityTest]" << std::endl;

  /*-------------------------------------------------------------------------------
  Quaternion
  -------------------------------------------------------------------------------*/

  /*
  Quaternion();
  
  Quaternion(U x, U y, U z, U w);
  */

  Quatf q;
  Quatf preSetQ(2,0,0,1);
  Vector3f preSetP(2.0f, 7.0f, 13.0f);
  /*
  // Operators
  bool              operator==(const Quaternion<T>& other) const;
  bool              operator!=(const Quaternion<T>& other) const;
  Quaternion<T>     operator+(const Quaternion<T>& other);
  Quaternion<T>&    operator+=(const Quaternion<T>& other);
  Quaternion<T>     operator-() const;
  Quaternion<T>     operator-(const Quaternion<T>& other);
  Quaternion<T>&    operator-=(const Quaternion<T>& other);
  Quaternion<T>     operator*(const Quaternion<T>& other);
  Quaternion<T>     operator*(T v) const;
  Quaternion<T>&    operator*=(const Quaternion<T>& other);
  Quaternion<T>&    operator*=(const Vector3<T>& v);
  Quaternion<T>&    operator*=(T v);
  Quaternion<T>     operator~() const;
  */
  q = -q;
  q = ~q;
  q = -q;
  q = ~q;
  E_ASSERT(q.IsIdentity());

  /*
  // Accessors
  T                 GetAxisAndAngle(Vector3<T>& v) const;
  T                 GetLength() const;
  T                 GetLengthSquared() const;
  void              GetRotation(Matrix4<T>& m) const;
  bool              IsIdentity() const; (already tested);
  void              SetIdentity();
  void              SetRotation(const Matrix4<T>& m);
  void              SetRotation(T pitch, T yaw, T roll);
  void              SetRotation(const Vector3<T>& eulerAngles);
  void              SetRotation(T radians, const Vector3<T>& axis);
  void              SetRotationBetweenVectors(const Vector3<T>& a, const Vector3<T>& b);
  */
  
  Vector3f v(20, 45, 60);
  v.x = Math::Rad(v.x);
  v.y = Math::Rad(v.y);
  v.z = Math::Rad(v.z);
  q.SetRotation(v.x, v.y, v.z);
  Quatf q2;
  q2.SetRotation(v);
  E_ASSERT(q == q2);

  Matrix4f m;
  m.SetRotation(v);
  q.SetRotation(v);
  q2.SetRotation(m);
  E_ASSERT(q == q2);

  // Random orientation test
  for (U32 i = 0; i < 100; ++i)
  {
    // Yaw Pitch Roll rotation generation
    v = Vector3f(
      Math::Global::GetRandom().GetF32(0, 180),
      Math::Global::GetRandom().GetF32(0, 180),
      Math::Global::GetRandom().GetF32(0, 180));

    Vector3f radV(Math::Rad(v.x), Math::Rad(v.y), Math::Rad(v.z));
    q.SetRotation(radV);
    m.SetRotation(radV);
    q2.SetRotation(m);

   /* E_ASSERT(
      Math::IsEqual(dxQ.x, dxQ2.x) &&
      Math::IsEqual(dxQ.y, dxQ2.y) &&
      Math::IsEqual(dxQ.z, dxQ2.z) &&
      Math::IsEqual(dxQ.w, dxQ2.w));*/
    // The quaternion has to be the same or its negated counter part
    E_ASSERT(q == q2 || q == -q2);
 
    // Point transformation
    Vector3f p(20, 45, 60);
    Vector3f transP = Matrix4f::TransformPoint(m, p);
    Vector3f transP2 = Quatf::Rotate(q, p);
    E_ASSERT(
      Math::IsEqual(transP.x, transP2.x, 1e-3f) && 
      Math::IsEqual(transP.y, transP2.y, 1e-3f) && 
      Math::IsEqual(transP.z, transP2.z, 1e-3f));

    /*
    E_ASSERT(
      !Math::IsEqual(transP.x, transP2.x, 1e-6f) ||  // It may fail sometimes
      !Math::IsEqual(transP.y, transP2.y, 1e-6f) ||  // It may fail sometimes
      !Math::IsEqual(transP.z, transP2.z, 1e-6f));   // It may fail sometimes
    */

    // Rotation matrix reconstruction
    Matrix4f m2;
    q.GetRotation(m2);
    E_ASSERT(m == m2);
  }

  q.SetRotation(Vector3f(1, 0, 0), Vector3f(0, 1, 0));
  q2.SetRotation(Math::Rad(90.f), Vector3f(0, 0, 1));
  E_ASSERT(q == q2);
  
  Vector3f v2;
  E_ASSERT(Math::Deg(q2.GetAxisAndAngle(v2)) == 90.0f);
  E_ASSERT(v2 == Vector3f(0, 0, 1));

  /*
  // Methods
  void              Normalize();
  void              Regenerate();
  */

  q2.x = 0.000002f;
  q2.y = -0.000005f;
  q2.z = 0.000003f;
  q2.w = 1.000002f;
  q2.Regenerate();
  E_ASSERT(q2.IsIdentity());

  /*
  static T              Dot(const Quaternion<T>& q1, const Quaternion<T>& q2);
  static Quaternion<T>  Lerp(const Quaternion<T>& qFrom, const Quaternion<T>& qTo, T t);
  static Vector3<T>     Rotate(const Quaternion<T>& q, const Vector3<T>& v);      (already tested)
  static Quaternion<T>  Slerp(const Quaternion<T>& qFrom, const Quaternion<T>& qTo, T t);
  */

  v = Vector3f(20, 45, 60);
  v.x = Math::Rad(v.x);
  v.y = Math::Rad(v.y);
  v.z = Math::Rad(v.z);
  q.SetRotation(v);
  q2.SetRotation(v);
  Quatf q3 = Quatf::Lerp(q, q2, .5f);
  E_ASSERT(q3 == q2);
  q3 = Quatf::Slerp(q, q2, .5f);
  E_ASSERT(q3 == q2);

  q.SetRotation(Vector3f(Math::Rad(90.f), 0.f, 0.f));
  q2.SetRotation(Vector3f(Math::Rad(60.f), 0.f, 0.f));
  Quatf q4;
  q4.SetRotation(Vector3f(Math::Rad(75.f), 0.f, 0.f));
  q3 = Quatf::Lerp(q, q2, .5f);
  E_ASSERT(q3 == q4);
  q3 = Quatf::Slerp(q, q2, .05f);
  E_ASSERT(q3 != q4);
  q3 = Quatf::Slerp(q, q2, .25f);
  E_ASSERT(q3 != q4);
  q3 = Quatf::Slerp(q, q2, .5f);
  E_ASSERT(q3 != q4);
  q3 = Quatf::Slerp(q, q2, .75f);
  E_ASSERT(q3 != q4);
  q3 = Quatf::Slerp(q, q2, .95f);
  E_ASSERT(q3 != q4);

  return true;
}

bool Test::Quaternion::RunPerformanceTest()
{
  std::cout << "[Test::Quaternion::RunPerformanceTest]" << std::endl;

  F32 pitch, yaw, roll; // X, Y, Z
  U32 iterationCount = 1000;
  Quatf rotation;
  Matrix4f rotationMatrix, rotationMatrix2;
  D3DXQUATERNION dxRotation;

  for (U32 i = 0; i < iterationCount; ++i)
  {
    pitch = Math::Global::GetRandom().GetF32(-180.0f, 180.0f);
    yaw = Math::Global::GetRandom().GetF32(-180.0f, 180.0f);
    roll = Math::Global::GetRandom().GetF32(-180.0f, 180.0f);

    rotation.SetRotation(pitch, yaw, roll);
    D3DXQuaternionRotationYawPitchRoll(&dxRotation, yaw, pitch, roll);
    E_DEBUG_VECTOR4(rotation);
    E_DEBUG_VECTOR4(dxRotation);
    E_ASSERT(
      Math::IsEqual(rotation.x, dxRotation.x) &&
      Math::IsEqual(rotation.y, dxRotation.y) &&
      Math::IsEqual(rotation.z, dxRotation.z) &&
      Math::IsEqual(rotation.w, dxRotation.w));

    // Matrix comparison
    rotationMatrix.SetRotation(pitch, yaw, roll);
    rotation.GetRotation(rotationMatrix2);
    E_DEBUG_MATRIX4(rotationMatrix);
    E_DEBUG_MATRIX4(rotationMatrix2);
    
    E_ASSERT(
      Math::IsEqual(rotationMatrix[0],  rotationMatrix2[0]) &&
      Math::IsEqual(rotationMatrix[1],  rotationMatrix2[1]) &&
      Math::IsEqual(rotationMatrix[2],  rotationMatrix2[2]) &&
      Math::IsEqual(rotationMatrix[3],  rotationMatrix2[3]) &&

      Math::IsEqual(rotationMatrix[4],  rotationMatrix2[4]) &&
      Math::IsEqual(rotationMatrix[5],  rotationMatrix2[5]) &&
      Math::IsEqual(rotationMatrix[6],  rotationMatrix2[6]) &&
      Math::IsEqual(rotationMatrix[7],  rotationMatrix2[7]) &&

      Math::IsEqual(rotationMatrix[8],  rotationMatrix2[8]) &&
      Math::IsEqual(rotationMatrix[9],  rotationMatrix2[9]) &&
      Math::IsEqual(rotationMatrix[10], rotationMatrix2[10]) &&
      Math::IsEqual(rotationMatrix[11], rotationMatrix2[11]) &&

      Math::IsEqual(rotationMatrix[12], rotationMatrix2[12]) &&
      Math::IsEqual(rotationMatrix[13], rotationMatrix2[13]) &&
      Math::IsEqual(rotationMatrix[14], rotationMatrix2[14]) &&
      Math::IsEqual(rotationMatrix[15], rotationMatrix2[15]));
  }

  return true;
}
