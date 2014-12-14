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

/** @file Matrix.cpp
This file defines Matrix test functions.
*/

#include <CoreTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Matrix::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Matrix::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Matrix::RunPerformanceTest");

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

bool Test::Matrix::RunFunctionalityTest()
{
  std::cout << "[Test::Matrix::RunFunctionalityTest]" << std::endl;

  /*-------------------------------------------------------------------------------
  Matrix4
  -------------------------------------------------------------------------------*/
  /*
  Matrix4();
  
  Matrix4(U a11,U a12,U a13,U a14, U a21,U a22,U a23,U a24, U a31,U a32,U a33,U a34, U a41,U a42,U a43,U a44);
  
  Matrix4(const U* p);
  */

  Matrix4f m;
  Matrix4f preSetM(
    2,0,0,0,
    0,2,0,0,
    0,0,2,0,
    0,0,0,1);
  Vector3f p;
  Vector3f preSetP(2.0f, 7.0f, 13.0f);

  /*
  // Operators
  T&                operator()(U32 i, U32 j);
  T                 operator()(U32 i, U32 j) const;
  bool              operator==(const Matrix4<T>& other) const;
  bool              operator!=(const Matrix4<T>& other) const;
  Matrix4<T>        operator+(const Matrix4<T>& other) const;
  Matrix4<T>&       operator+=(const Matrix4<T>& other);
  Matrix4<T>        operator-() const;
  Matrix4<T>        operator-(const Matrix4<T>& other) const;
  Matrix4<T>&       operator-=(const Matrix4<T>& other);
  Matrix4<T>        operator*(const Matrix4<T>& other) const;
  Matrix4<T>        operator*(T v) const;
  Matrix4<T>&       operator*=(const Matrix4<T>& other);
  Matrix4<T>&       operator*=(T v);
  */

  m *= 2;
  E_ASSERT(m != preSetM);
  m.SetIdentity();
  E_ASSERT(m.IsIdentity());
  m.Scale(2);
  E_ASSERT(m == preSetM);

  /*
  // Accessors      
  Vector3<T>        GetRotationVectorX() const;
  Vector3<T>        GetRotationVectorY() const;
  Vector3<T>        GetRotationVectorZ() const;
  Vector3<T>        GetTranslation() const;
  bool              IsIdentity() const;
  bool              IsOrthogonal() const;
  void              SetIdentity()
  void              SetRotation(T pitch, T yaw, T roll);
  void              SetRotation(const Vector3<T>& eulerAngles);
  void              SetRotation(T radians, const Vector3& axis);
  void              SetRotationVectors(const Vector3<T>& vx, const Vector3<T>& vy, const Vector3<T>& vz);
  void              SetRotationX(T pitch);
  void              SetRotationY(T yaw);
  void              SetRotationZ(T roll);
  void              SetScale(T v);
  void              SetScale(T x, T y, T z);
  void              SetScale(const Vector3<T>& v);
  void              SetTranslation(T x, T y, T z);
  void              SetTranslation(const Vector3<T>& v);
  */
 
  p = preSetP;
  m.SetRotation(Math::Rad(90.f), Math::Rad(45.f), Math::Rad(60.f));
  m.SetRotation(Vector3f(Math::Rad(90.f), Math::Rad(45.f), Math::Rad(60.f)));
  m.SetRotation(Math::Rad(30.f), Vector3f(0.5f, 0.5f, 0.0f));
  m.SetRotation(Math::Rad(30.f), Vector3f(0.0f, 0.0f, 1.0f));
  m.SetRotation(Vector3f(1, 0, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 1));
  E_ASSERT(m.GetVectorRight() == Vector3f(1, 0, 0));
  E_ASSERT(m.GetVectorUp() == Vector3f(0, 1, 0));
  E_ASSERT(m.GetVectorForward() == Vector3f(0, 0, 1));
  p = Matrix4f::RotateVector(m, p);
  E_ASSERT(p == preSetP);
  m.SetRotationX(Math::Rad(90.f));
  m.SetRotationY(Math::Rad(90.f));
  m.SetRotationZ(Math::Rad(90.f));
  m.SetIdentity();
  m.SetScale(2.f, 2.f, 2.f);
  m.SetScale(0.25f);
  m.SetScale(Vector3f(2.f, 2.f, 2.f));
  m.SetTranslation(-10.0f, -10.f, -10.f);
  E_ASSERT(m.GetTranslation() == Vector3f(-10.0f, -10.f, -10.f));
  m.SetTranslation(Vector3f(10.0f, 10.f, 10.f));
  m.SetIdentity();
  E_ASSERT(m == Matrix4f::Identity());
 
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
  m.SetRotation(Math::Rad(90.f), .0f, .0f);
  p = Matrix4f::RotateVector(m, p);
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
  m.SetRotation(.0f, Math::Rad(45.f), .0f);
  p = Matrix4f::RotateVector(m, p);
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
  m.SetRotation(.0f, .0f, Math::Rad(60.f));
  p = Matrix4f::RotateVector(m, p);
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
  m.SetRotation(.0f, .0f, Math::Rad(-60.f));
  p = Matrix4f::RotateVector(m, p);
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
  m.SetRotation(.0f, Math::Rad(-45.f), .0f);
  p = Matrix4f::RotateVector(m, p);
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
  m.SetRotation(Math::Rad(-90.f), .0f, .0f);
  p = Matrix4f::RotateVector(m, p);
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
 
  E_ASSERT(Math::IsEqual(p.x, preSetP.x, 1e-5f) && Math::IsEqual(p.y, preSetP.y, 1e-5f) && Math::IsEqual(p.z, preSetP.z, 1e-5f) );

  /*
  // Methods
  void              AffineInvert();
  void              AffineInvertTranspose();
  bool              Invert();
  void              RotateX(T radians);
  void              RotateY(T radians);
  void              RotateZ(T radians);
  void              Rotate(T radians, const Vector3<T>& axis);
  void              Scale(T x, T y, T z);
  void              Scale(T v);
  void              ScaleX(T v);
  void              ScaleY(T v);
  void              ScaleZ(T v);
  void              Translate(T x, T y, T z);
  void              Translate(const Vector3<T>& v);
  void              TranslateX(T v);
  void              TranslateY(T v);
  void              TranslateZ(T v);
  void              Transpose();
  */

  m = Matrix4f::Identity();
  m.AffineInvert();
  m.AffineInvertTranspose();
  m.Transpose();
  m.Invert();
  m.RotateX(Math::Rad(90.f));
  m.RotateY(Math::Rad(45.f));
  m.RotateZ(Math::Rad(60.f));
  m.RotateZ(Math::Rad(-60.f));
  m.RotateY(Math::Rad(-45.f));
  m.RotateX(Math::Rad(-90.f));
  m.Scale(2.f, 2.f, 2.f);
  m.Scale(0.5f);
  m.ScaleX(2.f);
  m.ScaleY(2.f);
  m.ScaleZ(2.f);
  Matrix4f mi = Matrix4f::Invert(m);
  Matrix4f mii = Matrix4f::Invert(mi);
  E_ASSERT(m == mii);
  m.Regenerate();
  mii.Regenerate();
  E_ASSERT(m == mii);
  m.Scale(Vector3f(0.5f, 0.5f, 0.5f));
  m.Translate(5.f, 5.f, 5.f);
  m.Translate(Vector3f(5.f, 5.f, 5.f));
  m.TranslateX(-10.f);
  m.TranslateY(-10.f);
  m.TranslateZ(-10.f);
  mi = Matrix4f::Invert(m);
  mii = Matrix4f::Invert(mi);
  E_ASSERT(m == mii);
  m.Regenerate();
  mii.Regenerate();
  E_ASSERT(m == mii);

  /*
  static Matrix4<T> Identity(); (already tested)
  static Vector3<T> Rotate(const Matrix4<T>& m, const Vector3<T>& v); (already tested)
  static Vector3<T> TransformPoint(const Matrix4<T>& m, const Vector3<T>& p);
  */

  p = Matrix4f::TransformPoint(m, p);
  std::cout << std::setprecision(9) << "Point: "  << p.x << ", " << p.y << ", "  << p.z  << std::endl;
  E_ASSERT(Math::IsEqual(p.x, preSetP.x, 1e-5f) && Math::IsEqual(p.y, preSetP.y, 1e-5f) && Math::IsEqual(p.z, preSetP.z, 1e-5f) );

  Matrix4f m2;
  m2.RotateX(Math::Rad(45.f));
  m.SetIdentity();
  m.SetRotationX(Math::Rad(45.f));
  E_ASSERT(m == m2);

  m2.SetIdentity();
  m2.RotateY(Math::Rad(60.f));
  m.SetIdentity();
  m.SetRotationY(Math::Rad(60.f));
  E_ASSERT(m == m2);

  m2.SetIdentity();
  m2.RotateZ(Math::Rad(30.f));
  m.SetIdentity();
  m.SetRotationZ(Math::Rad(30.f));
  E_ASSERT(m == m2);

  return true;
}

bool Test::Matrix::RunPerformanceTest()
{
  std::cout << "[Test::Matrix::RunPerformanceTest]" << std::endl;

  F32 pitch, yaw, roll; // X, Y, Z
  U32 iterationCount = 1000;
  Matrix4f rotation, rotationXYZ, rotationX, rotationY, rotationZ;
  D3DXMATRIX dxRotation, dxRotationXYZ, dxRotationX, dxRotationY, dxRotationZ;;

  for (U32 i = 0; i < iterationCount; ++i)
  {
    pitch = Math::Global::GetRandom().GetF32(-180.0f, 180.0f);
    yaw = Math::Global::GetRandom().GetF32(-180.0f, 180.0f);
    roll = Math::Global::GetRandom().GetF32(-180.0f, 180.0f);

    rotation.SetRotation(pitch, yaw, roll);
    D3DXMatrixRotationYawPitchRoll(&dxRotation, yaw, pitch, roll);
    E_DEBUG_MATRIX4(rotation);
    E_DEBUG_MATRIX4(dxRotation);
    E_ASSERT(
      Math::IsEqual(rotation[0],  dxRotation[0]) &&
      Math::IsEqual(rotation[1],  dxRotation[1]) &&
      Math::IsEqual(rotation[2],  dxRotation[2]) &&
      Math::IsEqual(rotation[3],  dxRotation[3]) &&

      Math::IsEqual(rotation[4],  dxRotation[4]) &&
      Math::IsEqual(rotation[5],  dxRotation[5]) &&
      Math::IsEqual(rotation[6],  dxRotation[6]) &&
      Math::IsEqual(rotation[7],  dxRotation[7]) &&

      Math::IsEqual(rotation[8],  dxRotation[8]) &&
      Math::IsEqual(rotation[9],  dxRotation[9]) &&
      Math::IsEqual(rotation[10], dxRotation[10]) &&
      Math::IsEqual(rotation[11], dxRotation[11]) &&

      Math::IsEqual(rotation[12], dxRotation[12]) &&
      Math::IsEqual(rotation[13], dxRotation[13]) &&
      Math::IsEqual(rotation[14], dxRotation[14]) &&
      Math::IsEqual(rotation[15], dxRotation[15]));

    // Composite rotation
    rotationX.SetRotationX(pitch);
    rotationY.SetRotationY(yaw);
    rotationZ.SetRotationZ(roll);
    rotationXYZ = rotationZ * rotationX * rotationY;
    D3DXMatrixRotationX(&dxRotationX, pitch);
    D3DXMatrixRotationY(&dxRotationY, yaw);
    D3DXMatrixRotationZ(&dxRotationZ, roll);
    dxRotationXYZ = dxRotationZ * dxRotationX * dxRotationY;
    E_DEBUG_MATRIX4(rotationXYZ);
    E_DEBUG_MATRIX4(dxRotationXYZ);
    E_ASSERT(
      Math::IsEqual(rotationXYZ[0],  dxRotationXYZ[0]) &&
      Math::IsEqual(rotationXYZ[1],  dxRotationXYZ[1]) &&
      Math::IsEqual(rotationXYZ[2],  dxRotationXYZ[2]) &&
      Math::IsEqual(rotationXYZ[3],  dxRotationXYZ[3]) &&

      Math::IsEqual(rotationXYZ[4],  dxRotationXYZ[4]) &&
      Math::IsEqual(rotationXYZ[5],  dxRotationXYZ[5]) &&
      Math::IsEqual(rotationXYZ[6],  dxRotationXYZ[6]) &&
      Math::IsEqual(rotationXYZ[7],  dxRotationXYZ[7]) &&

      Math::IsEqual(rotationXYZ[8],  dxRotationXYZ[8]) &&
      Math::IsEqual(rotationXYZ[9],  dxRotationXYZ[9]) &&
      Math::IsEqual(rotationXYZ[10], dxRotationXYZ[10]) &&
      Math::IsEqual(rotationXYZ[11], dxRotationXYZ[11]) &&

      Math::IsEqual(rotationXYZ[12], dxRotationXYZ[12]) &&
      Math::IsEqual(rotationXYZ[13], dxRotationXYZ[13]) &&
      Math::IsEqual(rotationXYZ[14], dxRotationXYZ[14]) &&
      Math::IsEqual(rotationXYZ[15], dxRotationXYZ[15]));

    E_ASSERT(
      Math::IsEqual(rotationXYZ[0],  rotation[0]) &&
      Math::IsEqual(rotationXYZ[1],  rotation[1]) &&
      Math::IsEqual(rotationXYZ[2],  rotation[2]) &&
      Math::IsEqual(rotationXYZ[3],  rotation[3]) &&

      Math::IsEqual(rotationXYZ[4],  rotation[4]) &&
      Math::IsEqual(rotationXYZ[5],  rotation[5]) &&
      Math::IsEqual(rotationXYZ[6],  rotation[6]) &&
      Math::IsEqual(rotationXYZ[7],  rotation[7]) &&

      Math::IsEqual(rotationXYZ[8],  rotation[8]) &&
      Math::IsEqual(rotationXYZ[9],  rotation[9]) &&
      Math::IsEqual(rotationXYZ[10], rotation[10]) &&
      Math::IsEqual(rotationXYZ[11], rotation[11]) &&

      Math::IsEqual(rotationXYZ[12], rotation[12]) &&
      Math::IsEqual(rotationXYZ[13], rotation[13]) &&
      Math::IsEqual(rotationXYZ[14], rotation[14]) &&
      Math::IsEqual(rotationXYZ[15], rotation[15]));
  }
  
  return true;
}
