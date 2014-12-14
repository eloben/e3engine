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

// Created 15-Nov-2011 by Elías Lozada-Benavente (updated 30-Dec-2013)
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Transform.h
This file defines a 4x4 matrix class.
*/

#ifndef E_TRANSFORM_H
#define E_TRANSFORM_H

#include "Vector3.h"

namespace E
{
  namespace Math
  {
    /*------------------------------------------------------------------------------------------------------------------
    Transform

    Please note that this class has the following usage contract: 

    1. Floating point types are expected to be used with this class: F32, D64.
    2. Transform uses a row-major data ordering and a left-handed coordinate system for rotations. Remember then than 
    vectors are represented as rows and composition matrices are pre-multiplied.
    3. The chosen rotation order is Pitch Yaw Roll (X,Y,Z) in opposition of DirectX SDK Yaw Pitch Roll (Y, X, Z),
    however you may find the DirectX SDK rotation order version commented.
    ------------------------------------------------------------------------------------------------------------------*/
	  template <typename T>
	  class Transform
	  {
	  public:
		  Transform();
		  Transform(T a11,T a12,T a13,T a14, T a21,T a22,T a23,T a24, T a31,T a32,T a33,T a34, T a41,T a42,T a43,T a44);
		  Transform(const T* p);

		  // Operators
      const T&          operator[](U32 i) const;
      T&                operator[](U32 i);
      const T&          operator()(U32 i, U32 j) const;
      T&                operator()(U32 i, U32 j);      
      bool              operator==(const Transform& other) const;
      bool              operator!=(const Transform& other) const;
      Transform           operator+(const Transform& other) const;
      Transform&          operator+=(const Transform& other);
      Transform           operator-() const;
      Transform           operator-(const Transform& other) const;
      Transform&          operator-=(const Transform& other);
      Transform           operator*(const Transform& other) const;
      Transform           operator*(T v) const;
      Transform&          operator*=(const Transform& other);
      Transform&          operator*=(T v);

		  // Accessors     
      Vector3<T>        GetTranslation() const;
      Vector3<T>        GetVectorForward() const;
      Vector3<T>        GetVectorRight() const;
      Vector3<T>        GetVectorUp() const;
      bool              IsAffine() const;
		  bool              IsIdentity() const;
	    void              SetIdentity();
	    void              SetRotation(T pitch, T yaw, T roll);
      void              SetRotation(const Vector3<T>& eulerAngles);
      void              SetRotation(T radians, const Vector3<T>& axis);
      void              SetRotation(const Vector3<T>& right, const Vector3<T>& up, const Vector3<T>& forward);
      void              SetRotationX(T pitch);
		  void              SetRotationY(T yaw);
		  void              SetRotationZ(T roll);
		  void              SetScale(T v);
		  void              SetScale(T x, T y, T z);
      void              SetScale(const Vector3<T>& v);
		  void              SetTranslation(T x, T y, T z);
		  void              SetTranslation(const Vector3<T>& v);

		  // Methods
		  void              AffineInvert();
		  void              AffineInvertTranspose();
      bool              Invert();
      void              Regenerate();
		  void              RotateX(T radians);
		  void              RotateY(T radians);
		  void              RotateZ(T radians);
      void              Rotate(T radians, const Vector3<T>& axis);
		  void              Scale(T x, T y, T z);
      void              Scale(const Vector3<T>& v);
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

      // Static methods
      static Transform    Identity();
      static Vector3<T> Rotate(const Transform& m, const Vector3<T>& v);
      static Vector3<T> Transform(const Transform& m, const Vector3<T>& p);

	  private:
		  T m[16];

		  // Relying on default copy constructor and assignment operator
	  };

    /*------------------------------------------------------------------------------------------------------------------
    Transform initialization & finalization
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
    inline Transform<T>::Transform()
	  {
		  SetIdentity();
	  }

    template <typename T>
	
    inline Transform<T>::Transform(T a11,T a12,T a13,T a14, T a21,T a22,T a23,T a24, T a31,T a32,T a33,T a34, T a41,T a42,T a43,T a44)
	  {
		  m[0]  = a11; 
		  m[1]  = a12; 
		  m[2]  = a13; 
		  m[3]  = a14; 
		  m[4]  = a21; 
		  m[5]  = a22; 
		  m[6]  = a23; 
		  m[7]  = a24; 
		  m[8]  = a31; 
		  m[9]  = a32; 
		  m[10] = a33; 
		  m[11] = a34; 
		  m[12] = a41; 
		  m[13] = a42; 
		  m[14] = a43; 
		  m[15] = a44;
	  }

    template <typename T>
	
	  inline Transform<T>::Transform(const T* p)
	  {
      m[0]  = p[0]; 
      m[1]  = p[1]; 
      m[2]  = p[2]; 
      m[3]  = p[3]; 
      m[4]  = p[4]; 
      m[5]  = p[5]; 
      m[6]  = p[6]; 
      m[7]  = p[7]; 
      m[8]  = p[8]; 
      m[9]  = p[9]; 
      m[10] = p[10]; 
      m[11] = p[11]; 
      m[12] = p[12]; 
      m[13] = p[13]; 
      m[14] = p[14]; 
      m[15] = p[15];
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Transform operators
    ------------------------------------------------------------------------------------------------------------------*/

    template <typename T>
    inline T& Transform<T>::operator[](U32 i)     
    { 
      return m[i]; 
    }

    template <typename T>
    inline const T& Transform<T>::operator[](U32 i) const    
    { 
      return m[i]; 
    }

    template <typename T>
	  inline T& Transform<T>::operator()(U32 i, U32 j)       
	  { 
      return m[i * 4 + j]; 
    }
	
    template <typename T>
	  inline const T& Transform<T>::operator()(U32 i, U32 j) const 
	  { 
      return m[i * 4 + j]; 
    }

    template <typename T>
    inline bool Transform<T>::operator==(const Transform& other) const
    {
      for(int i=0; i<16; i++)
      {
        if (!Math::IsEqual(m[i], other.m[i]))
          return false;
      }
      return true;
    }

    template <typename T>
    inline bool Transform<T>::operator!=(const Transform& other) const
    {
      return !((*this) == other);
    }

    template <typename T>
	  inline Transform<T> Transform<T>::operator+(const Transform& other) const
	  {
		  return Transform<T>(
          m[ 0] + other.m[ 0], m[ 1] + other.m[ 1], m[ 2] + other.m[ 2], m[ 3] + other.m[ 3],
          m[ 4] + other.m[ 4], m[ 5] + other.m[ 5], m[ 6] + other.m[ 6], m[ 7] + other.m[ 7],
          m[ 8] + other.m[ 8], m[ 9] + other.m[ 9], m[10] + other.m[10], m[11] + other.m[11],
          m[12] + other.m[12], m[13] + other.m[13], m[14] + other.m[14], m[15] + other.m[15]);
	  }

    template <typename T>
    inline Transform<T>& Transform<T>::operator+=(const Transform& other)
    {
      (*this) = (*this) + other;
      return (*this);
    }

    template <typename T>
    inline Transform<T> Transform<T>::operator-() const
    {
      Transform<T> result(*this);
      for(int i=0; i<16; i++)
        result.m[i] = -result.m[i];
      return result;
    }

    template <typename T>
	  inline Transform<T> Transform<T>::operator-(const Transform& other) const
	  {
		  return Transform<T>(	
        m[ 0] - other.m[ 0], m[ 1] - other.m[ 1], m[ 2] - other.m[ 2], m[ 3] - other.m[ 3],
			  m[ 4] - other.m[ 4], m[ 5] - other.m[ 5], m[ 6] - other.m[ 6], m[ 7] - other.m[ 7],
			  m[ 8] - other.m[ 8], m[ 9] - other.m[ 9], m[10] - other.m[10], m[11] - other.m[11],
			  m[12] - other.m[12], m[13] - other.m[13], m[14] - other.m[14], m[15] - other.m[15]);
	  }

    template <typename T>
    inline Transform<T>& Transform<T>::operator-=(const Transform& other)
    {
      (*this) = (*this) - other;
      return (*this);
    }

    template <typename T>
	  inline Transform<T> Transform<T>::operator*(const Transform& other) const
	  {
		  return Transform<T>(	
        m[0] * other.m[ 0] + m[4] * other.m[ 1] + m[ 8] * other.m[ 2] + m[12] * other.m[ 3],
        m[1] * other.m[ 0] + m[5] * other.m[ 1] + m[ 9] * other.m[ 2] + m[13] * other.m[ 3],
        m[2] * other.m[ 0] + m[6] * other.m[ 1] + m[10] * other.m[ 2] + m[14] * other.m[ 3],
        m[3] * other.m[ 0] + m[7] * other.m[ 1] + m[11] * other.m[ 2] + m[15] * other.m[ 3],
        m[0] * other.m[ 4] + m[4] * other.m[ 5] + m[ 8] * other.m[ 6] + m[12] * other.m[ 7],
        m[1] * other.m[ 4] + m[5] * other.m[ 5] + m[ 9] * other.m[ 6] + m[13] * other.m[ 7],
        m[2] * other.m[ 4] + m[6] * other.m[ 5] + m[10] * other.m[ 6] + m[14] * other.m[ 7],
        m[3] * other.m[ 4] + m[7] * other.m[ 5] + m[11] * other.m[ 6] + m[15] * other.m[ 7],
        m[0] * other.m[ 8] + m[4] * other.m[ 9] + m[ 8] * other.m[10] + m[12] * other.m[11],
        m[1] * other.m[ 8] + m[5] * other.m[ 9] + m[ 9] * other.m[10] + m[13] * other.m[11],
        m[2] * other.m[ 8] + m[6] * other.m[ 9] + m[10] * other.m[10] + m[14] * other.m[11],
        m[3] * other.m[ 8] + m[7] * other.m[ 9] + m[11] * other.m[10] + m[15] * other.m[11],
        m[0] * other.m[12] + m[4] * other.m[13] + m[ 8] * other.m[14] + m[12] * other.m[15],
        m[1] * other.m[12] + m[5] * other.m[13] + m[ 9] * other.m[14] + m[13] * other.m[15],
        m[2] * other.m[12] + m[6] * other.m[13] + m[10] * other.m[14] + m[14] * other.m[15],
        m[3] * other.m[12] + m[7] * other.m[13] + m[11] * other.m[14] + m[15] * other.m[15]);
	  }

    template <typename T>
    inline Transform<T> Transform<T>::operator*(T v) const
    {
      return Transform<T>(
        m[0]  * v, m[1]  * v, m[2]  * v, m[3]  * v,
        m[4]  * v, m[5]  * v, m[6]  * v, m[7]  * v,
        m[8]  * v, m[9]  * v, m[10] * v, m[11] * v,
        m[12] * v, m[13] * v, m[14] * v, m[15] * v);
    }

    template <typename T>
    inline Transform<T>& Transform<T>::operator*=(const Transform& other)
    {
      for (U32 i = 0; i < 16; ++i)
      {
        m[i] *= other[i];
      }

      return (*this);
    }

    template <typename T>
    inline Transform<T>& Transform<T>::operator*=(T v)
    {
      for (U32 i = 0; i < 16; ++i)
      {
        m[i] *= v;
      }

      return (*this);
    }

    /*------------------------------------------------------------------------------------------------------------------
    Transform accessors
    ------------------------------------------------------------------------------------------------------------------*/

    template <typename T>
    inline Vector3<T> Transform<T>::GetTranslation() const 
    { 
      return Vector3<T>(m[12], m[13], m[14]);
    }

    template <typename T>
    inline Vector3<T> Transform<T>::GetVectorForward() const 
    { 
      return Vector3<T>(m[2], m[6], m[10]);
    }

    template <typename T>
    inline Vector3<T> Transform<T>::GetVectorRight() const 
    {
      return Vector3<T>(m[0], m[4], m[8]);
    }

    template <typename T>
    inline Vector3<T> Transform<T>::GetVectorUp() const 
    {
      return Vector3<T>(m[1], m[5], m[9]);
    }

    template <typename T>
    inline bool Transform<T>::IsAffine() const
    {
      return(
        Math::IsEqual(m[ 3], static_cast<T>(0)) &&
        Math::IsEqual(m[ 7], static_cast<T>(0)) &&
        Math::IsEqual(m[11], static_cast<T>(0)) &&
        Math::IsEqual(m[15], static_cast<T>(1)));
    }

    template <typename T>
	  inline bool Transform<T>::IsIdentity() const
	  {
		  return(
        Math::IsEqual(m[ 0], static_cast<T>(1)) &&
        Math::IsEqual(m[ 1], static_cast<T>(0)) &&
        Math::IsEqual(m[ 2], static_cast<T>(0)) &&
        Math::IsEqual(m[ 3], static_cast<T>(0)) &&
        Math::IsEqual(m[ 4], static_cast<T>(0)) &&
        Math::IsEqual(m[ 5], static_cast<T>(1)) &&
        Math::IsEqual(m[ 6], static_cast<T>(0)) &&
        Math::IsEqual(m[ 7], static_cast<T>(0)) &&
        Math::IsEqual(m[ 8], static_cast<T>(0)) &&
        Math::IsEqual(m[ 9], static_cast<T>(0)) &&
        Math::IsEqual(m[10], static_cast<T>(1)) &&
        Math::IsEqual(m[11], static_cast<T>(0)) &&
        Math::IsEqual(m[12], static_cast<T>(0)) &&
        Math::IsEqual(m[13], static_cast<T>(0)) &&
        Math::IsEqual(m[14], static_cast<T>(0)) &&
        Math::IsEqual(m[15], static_cast<T>(1)));
	  }

    template <typename T>
	  inline void Transform<T>::SetIdentity()
	  {
		  m[0]  = static_cast<T>(1); m[1]  = static_cast<T>(0); m[2]  = static_cast<T>(0); m[3]  = static_cast<T>(0);
		  m[4]  = static_cast<T>(0); m[5]  = static_cast<T>(1); m[6]  = static_cast<T>(0); m[7]  = static_cast<T>(0);
		  m[8]  = static_cast<T>(0); m[9]  = static_cast<T>(0); m[10] = static_cast<T>(1); m[11] = static_cast<T>(0);
		  m[12] = static_cast<T>(0); m[13] = static_cast<T>(0); m[14] = static_cast<T>(0); m[15] = static_cast<T>(1);
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Sets a rotation using Euler angles X,Y,Z or pitch, yaw, roll
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
    inline void Transform<T>::SetRotation(T pitch, T yaw, T roll)
    {
      // Rotation is performed in a Pitch, Yaw, Roll order: 
      /*
      Transform<T> r, rX, rY, rZ;
      rX.SetRotationX(pitch);
      rY.SetRotationY(yaw);
      rZ.SetRotationZ(roll);
      r = rX * rY * rZ;
      */

      T cx = Math::Cos(pitch);
      T sx = Math::Sin(pitch);
      T cy = Math::Cos(yaw);
      T sy = Math::Sin(yaw);
      T cz = Math::Cos(roll);
      T sz = Math::Sin(roll);

      // Pitch Yaw Roll
      m[ 0] = cz * cy;
      m[ 1] = sz * cx + cz * sy * sx;
      m[ 2] = sz * sx - cz * sy * cx;

      m[ 4] = -sz * cy;
      m[ 5] = cz * cx - sz * sy * sx;
      m[ 6] = cz * sx + sz * sy * cx;

      m[ 8] = sy;
      m[ 9] = -cy * sx;
      m[10] = cy * cx;
    
      // Yaw Pitch Roll (DirectX SDK)
      // Note: to switch to this rotation order you also must change Quaternion<T>::SetRotation(T pitch, T yaw, T roll)
      /*
      m[0] = cy * cz + sx * sy * sz;
      m[1] = cx * sz;
      m[2] = sx * sz * cy - cz * sy;

      m[4] = cz * sx * sy - cy * sz;
      m[5] = cx * cz;
      m[6] = sy * sz + cy * cz * sx;

      m[ 8] = cx * sy;
      m[ 9] = -sx;
      m[10] = cx * cy;
      */
    }

    /*------------------------------------------------------------------------------------------------------------------
    Sets a rotation using Euler angles X,Y,Z or pitch, yaw, roll
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
    inline void Transform<T>::SetRotation(const Vector3<T>& eulerAngles)
    {
      SetRotation(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    /*------------------------------------------------------------------------------------------------------------------
    Sets a rotation around an arbitrary axis (note axis vector must be normalized)
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
    inline void Transform<T>::SetRotation(T radians, const Vector3<T>& axis)
    {
      T c = Math::Cos(radians);
      T s = Math::Sin(radians);
      T t = 1 - c;     

      m[0] = t * axis.x * axis.x + c;
      m[1] = t * axis.x * axis.y + s * axis.z;
      m[2] = t * axis.x * axis.z - s * axis.y;

      m[4] = t * axis.x * axis.y - s * axis.z;
      m[5] = t * axis.y * axis.y + c;
      m[6] = t * axis.y * axis.z + s * axis.x;

      m[ 8] = t * axis.x * axis.z + s * axis.y;
      m[ 9] = t * axis.y * axis.z - s * axis.x;
      m[10] = t * axis.z * axis.z + c;
    }

    /*------------------------------------------------------------------------------------------------------------------
    Sets the rotation through rotation vectors right, up and forward corresponding to col0, col1, col2

    Remember that axis (mutually orthogonal) form a basis (like the standard basis X(1,0,0), Y(0,1,0), Z(0,0,1). To 
    express a rotation matrix from a basis you use the transposed axis vectors. As we are using a left handed system
    vectors are expressed as rows, therefore their transposed equivalents must be expressed as columns,
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
	  inline void Transform<T>::SetRotation(const Vector3<T>& right, const Vector3<T>& up, const Vector3<T>& forward)
	  {
		  m[0] = right.x;
		  m[4] = right.y;
		  m[8] = right.z;

		  m[1] = up.x;
		  m[5] = up.y;
		  m[9] = up.z;

		  m[2] = forward.x;
		  m[6] = forward.y;
		  m[10] = forward.z;
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Sets a rotation around X axis
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
	  inline void Transform<T>::SetRotationX(T radians)
	  {
		  m[0] = 1;
		  m[1] = 0;
		  m[2] = 0;

		  m[4] = 0;
		  m[5] = Math::Cos(radians);
		  m[6] = Math::Sin(radians);

		  m[8] = 0;
		  m[9] = -m[6];
		  m[10] = m[5];
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Sets a rotation around Y axis
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
	  inline void Transform<T>::SetRotationY(T radians)
	  {
		  m[0] = Math::Cos(radians);
		  m[1] = 0;
		  m[2] = -Math::Sin(radians);

		  m[4] = 0;
		  m[5] = 1;
		  m[6] = 0;

		  m[8] = -m[2];
		  m[9] = 0;
		  m[10] = m[0];
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Sets a rotation around Z axis
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
	  inline void Transform<T>::SetRotationZ(T radians)
	  {
		  m[0] = Math::Cos(radians);
		  m[1] = Math::Sin(radians);
		  m[2] = 0;

      m[4] = -m[1];
		  m[5] = m[0];
		  m[6] = 0;

      m[8] = 0;
		  m[9] = 0;
		  m[10] = 1;
	  }

    template <typename T>
	  inline void Transform<T>::SetScale(T v)
	  {
		  m[0] = v;
		  m[5] = v;
		  m[10] = v;
	  }

    template <typename T>
	  void Transform<T>::SetScale(T x, T y, T z)
	  {
		  m[0] = x;
		  m[5] = y;
		  m[10] = z;
	  }

    template <typename T>
    inline void Transform<T>::SetScale(const Vector3<T>& v)
    { 
      SetScale(v.x, v.y, v.z); 
    }

    template <typename T>
	  inline void Transform<T>::SetTranslation(T x, T y, T z)
	  {
		  m[12] = x;
		  m[13] = y;
		  m[14] = z;
	  }
  
    template <typename T>
	  inline void Transform<T>::SetTranslation(const Vector3<T>& v)
	  {
      SetTranslation(v.x, v.y, v.z);
    }

    /*------------------------------------------------------------------------------------------------------------------
    Transform methods
    ------------------------------------------------------------------------------------------------------------------*/

	  // Methods
	
	  /* Note about affine transformations:
	  Affine methods must only be used if the matrix is composed of rotations and/or translations.
	  Do not use it if there is any scaling transformation involved.
	  */
    template <typename T>
	  inline void Transform<T>::AffineInvert()
	  {
		  // Compute the transpose of the rotation part and the negative of the inverse rotated translation part
		  Transform<T> result(	
        m[0], m[4], m[ 8], static_cast<T>(0),
		    m[1], m[5], m[ 9], static_cast<T>(0),
		    m[2], m[6], m[10], static_cast<T>(0),
		    -(m[0] * m[12] + m[1] * m[13] + m[2] * m[14]), -(m[4] * m[12] + m[5] * m[13] + m[6] * m[14]), -(m[8] * m[12] + m[9] * m[13] + m[10] * m[14]), static_cast<T>(1));

		  *this = result;
	  }

    template <typename T>
	  inline void Transform<T>::AffineInvertTranspose()
	  {
		  Transform<T> result(
        m[0],m[1],m[ 2], -(m[0] * m[12] + m[1] * m[13] + m[ 2] * m[14]),
		    m[4],m[5],m[ 6], -(m[4] * m[12] + m[5] * m[13] + m[ 6] * m[14]),
		    m[8],m[9],m[10], -(m[8] * m[12] + m[9] * m[13] + m[10] * m[14]),
		    static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
		  *this = result;
	  }

    template <typename T>
	  inline bool Transform<T>::Invert()
	  {
		  // Based on code from www.geometrictools.com
		  T a0 = m[ 0] * m[ 5] - m[ 1] * m[ 4];
		  T a1 = m[ 0] * m[ 6] - m[ 2] * m[ 4];
		  T a2 = m[ 0] * m[ 7] - m[ 3] * m[ 4];
		  T a3 = m[ 1] * m[ 6] - m[ 2] * m[ 5];
		  T a4 = m[ 1] * m[ 7] - m[ 3] * m[ 5];
		  T a5 = m[ 2] * m[ 7] - m[ 3] * m[ 6];
		  T b0 = m[ 8] * m[13] - m[ 9] * m[12];
		  T b1 = m[ 8] * m[14] - m[10] * m[12];
		  T b2 = m[ 8] * m[15] - m[11] * m[12];
		  T b3 = m[ 9] * m[14] - m[10] * m[13];
		  T b4 = m[ 9] * m[15] - m[11] * m[13];
		  T b5 = m[10] * m[15] - m[11] * m[14];
		  T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

		  if (det == static_cast<T>(0))
			  return false;

		  Transform<T> inverse;
		  inverse.m[ 0] = + m[ 5] * b5 - m[ 6] * b4 + m[ 7] * b3;
		  inverse.m[ 4] = - m[ 4] * b5 + m[ 6] * b2 - m[ 7] * b1;
		  inverse.m[ 8] = + m[ 4] * b4 - m[ 5] * b2 + m[ 7] * b0;
		  inverse.m[12] = - m[ 4] * b3 + m[ 5] * b1 - m[ 6] * b0;
		  inverse.m[ 1] = - m[ 1] * b5 + m[ 2] * b4 - m[ 3] * b3;
		  inverse.m[ 5] = + m[ 0] * b5 - m[ 2] * b2 + m[ 3] * b1;
		  inverse.m[ 9] = - m[ 0] * b4 + m[ 1] * b2 - m[ 3] * b0;
		  inverse.m[13] = + m[ 0] * b3 - m[ 1] * b1 + m[ 2] * b0;
		  inverse.m[ 2] = + m[13] * a5 - m[14] * a4 + m[15] * a3;
		  inverse.m[ 6] = - m[12] * a5 + m[14] * a2 - m[15] * a1;
		  inverse.m[10] = + m[12] * a4 - m[13] * a2 + m[15] * a0;
		  inverse.m[14] = - m[12] * a3 + m[13] * a1 - m[14] * a0;
		  inverse.m[ 3] = - m[ 9] * a5 + m[10] * a4 - m[11] * a3;
		  inverse.m[ 7] = + m[ 8] * a5 - m[10] * a2 + m[11] * a1;
		  inverse.m[11] = - m[ 8] * a4 + m[ 9] * a2 - m[11] * a0;
		  inverse.m[15] = + m[ 8] * a3 - m[ 9] * a1 + m[10] * a0;

		  T invDet = 1 / det;
		  inverse *= invDet;

		  *this = inverse;

		  return true;
	  }

    template <typename T>
    inline void Transform<T>::Regenerate()
    {
      for (U32 i = 0; i < 16; ++i)
      {
        m[i] = Math::IsEqual(m[i], static_cast<T>(0)) ? 0 : (Math::IsEqual(m[i], static_cast<T>(1)) ? 1 : m[i]);
      }
    }

    /*------------------------------------------------------------------------------------------------------------------
    Rotates around X axis
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
	  inline void Transform<T>::RotateX(T radians)
	  {
		  T c = Math::Cos(radians);
		  T s = Math::Sin(radians);
      T result4   = m[ 4] * c + m[ 8] * s;
      T result5   = m[ 5] * c + m[ 9] * s;
      T result6   = m[ 6] * c + m[10] * s;
      T result8   = m[ 8] * c - m[ 4] * s;
      T result9   = m[ 9] * c - m[ 5] * s;
		  T result10  = m[10] * c - m[ 6] * s;

		  m[4] = result4;
      m[5] = result5;
      m[6] = result6;
      m[8] = result8; 
      m[9] = result9;
      m[10] = result10;
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Rotates around Y axis
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
	  inline void Transform<T>::RotateY(T radians)
	  {
		  T c = Math::Cos(radians);
		  T s = Math::Sin(radians);
		  T result0   = m[ 0] * c - m[8] * s;
		  T result1   = m[ 1] * c - m[9] * s;
		  T result2   = m[ 2] * c - m[10] * s;
		  T result8   = m[ 8] * c + m[0] * s;
		  T result9   = m[ 9] * c + m[1] * s;
		  T result10  = m[10] * c + m[2] * s;

		  m[0] = result0; m[1] = result1; m[2] = result2;
		  m[8] = result8; m[9] = result9; m[10] = result10;
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Rotates around Z axis
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
	  inline void Transform<T>::RotateZ(T radians)
	  {
		  T c = Math::Cos(radians);
		  T s = Math::Sin(radians);
      T result0 = m[0] * c + m[4] * s;
      T result1 = m[1] * c + m[5] * s;
      T result2 = m[2] * c + m[6] * s;
		  T result4 = m[4] * c - m[0] * s;
		  T result5 = m[5] * c - m[1] * s;
		  T result6 = m[6] * c - m[2] * s;

		  m[0] = result0; m[1] = result1; m[2] = result2;
		  m[4] = result4; m[5] = result5; m[6] = result6;
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Rotates around an arbitrary axis (note that axis vector must be normalized)
    ------------------------------------------------------------------------------------------------------------------*/
    template <typename T>
    inline void Transform<T>::Rotate(T radians, const Vector3<T>& axis) 
    { 
      T h = static_cast<T>(1) / axis.GetLength();

      T hx = axis.x * h;
      T hy = axis.y * h;
      T hz = axis.z * h;

      T c = Math::Cos(radians);
      T s = Math::Sin(radians);
      T t = static_cast<T>(1) - c;

      T r0 = t * hx * hx + c;
      T r1 = t * hx * hy + s * hz;
      T r2 = t * hx * hz - s * hy;

      T r4 = t * hx * hy - s * hz;
      T r5 = t * hy * hy + c;
      T r6 = t * hy * hz + s * hx;

      T r8 = t * hx * hz + s * hy;
      T r9 = t * hy * hz - s * hx;
      T r10 = t * hz * hz + c;

      T result0  = m[0] * r0 + m[4] * r1 + m[ 8] * r2;
      T result1  = m[1] * r0 + m[5] * r1 + m[ 9] * r2;
      T result2  = m[2] * r0 + m[6] * r1 + m[10] * r2;
      T result4  = m[0] * r4 + m[4] * r5 + m[ 8] * r6;
      T result5  = m[1] * r4 + m[5] * r5 + m[ 9] * r6;
      T result6  = m[2] * r4 + m[6] * r5 + m[10] * r6;
      T result8  = m[0] * r8 + m[4] * r9 + m[ 8] * r10;
      T result9  = m[1] * r8 + m[5] * r9 + m[ 9] * r10;
      T result10 = m[2] * r8 + m[6] * r9 + m[10] * r10;

      m[0] = result0; m[1] = result1; m[2] = result2;
      m[4] = result4; m[5] = result5; m[6] = result6;
      m[8] = result8; m[9] = result9; m[10] = result10;
    }

    template <typename T>
    inline void Transform<T>::Scale(T v)
    { 
      Scale(v, v, v);
    }

    template <typename T>
	  inline void Transform<T>::Scale(T x, T y, T z)
	  {
		  m[0] *= x;
		  m[1] *= x;
		  m[2] *= x;

		  m[4] *= y;
		  m[5] *= y;
		  m[6] *= y;

		  m[ 8] *= z;
		  m[ 9] *= z;
		  m[10] *= z;
	  }

    template <typename T>
	  inline void Transform<T>::Scale(const Vector3<T>& v)
	  { 
      Scale(v.x, v.y, v.z);
    }

    template <typename T>
	  inline void Transform<T>::ScaleX(T v)
	  {
      m[0] *= v;
      m[1] *= v;
      m[2] *= v;
    }

    template <typename T>
	  inline void Transform<T>::ScaleY(T v)
	  {
      m[4] *= v;
      m[5] *= v;
      m[6] *= v;
	  }

    template <typename T>
	  inline void Transform<T>::ScaleZ(T v)
	  {
      m[ 8] *= v;
      m[ 9] *= v;
      m[10] *= v;
	  }

    template <typename T>
	  inline void Transform<T>::Translate(T x, T y, T z)
	  {
      m[12] += m[0] * x + m[4] * y + m[ 8] * z;
      m[13] += m[1] * x + m[5] * y + m[ 9] * z;
      m[14] += m[2] * x + m[6] * y + m[10] * z;
	  }

    template <typename T>
	  inline void Transform<T>::Translate(const Vector3<T>& v) 
	  { 
      Translate(v.x, v.y, v.z);
    }

    template <typename T>
	  inline void Transform<T>::TranslateX(T v)
	  {
		  m[12] += m[0] * v;
		  m[13] += m[1] * v;
		  m[14] += m[2] * v;
	  }

    template <typename T>
	  inline void Transform<T>::TranslateY(T v)
	  {
		  m[12] += m[4] * v;
		  m[13] += m[5] * v;
		  m[14] += m[6] * v;
	  }

    template <typename T>
	  inline void Transform<T>::TranslateZ(T v)
	  {
		  m[12] += m[ 8] * v;
		  m[13] += m[ 9] * v;
		  m[14] += m[10] * v;
	  }
	
    template <typename T>
	  inline void Transform<T>::Transpose()
	  {
		  std::swap(m[1],m[4]); std::swap(m[2],m[ 8]); std::swap(m[ 3],m[12]);
		  std::swap(m[6],m[9]); std::swap(m[7],m[13]); std::swap(m[11],m[14]);
	  }

    /*------------------------------------------------------------------------------------------------------------------
    Transform static methods
    ------------------------------------------------------------------------------------------------------------------*/

    template <typename T>
	  inline Transform<T> Transform<T>::Identity()
	  { 
      static Transform<T> sIdentity; 
      return sIdentity;
    }

    template <typename T>
    inline Vector3<T> Transform<T>::Rotate(const Transform& m, const Vector3<T>& v)
    {
      return Vector3<T>(	
        m[0] * v.x + m[4] * v.y +  m[8] * v.z,
        m[1] * v.x + m[5] * v.y +  m[9] * v.z,
        m[2] * v.x + m[6] * v.y + m[10] * v.z);
    }

    template <typename T>
	  inline Vector3<T> Transform<T>::Transform(const Transform& m, const Vector3<T>& p)
	  {
		  return Vector3<T>(
        m[0] * p.x + m[4] * p.y +  m[8] * p.z + m[12],
			  m[1] * p.x + m[5] * p.y +  m[9] * p.z + m[13],
			  m[2] * p.x + m[6] * p.y + m[10] * p.z + m[14]);
    
	  }
  }
  
  /*--------------------------------------------------------------------------------------------------------------------
  Transform types
  --------------------------------------------------------------------------------------------------------------------*/
  typedef Math::Transform<F32> Transformf;
	typedef Math::Transform<D64> Transformd;
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Transformf)
E_DECLARE_POD(E::Transformd)

#endif
