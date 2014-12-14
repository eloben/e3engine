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

// Created 28-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Projection.h
This file defines utility functions for computing orthographic and perspective projection matrices.
*/
#ifndef E3_PROJECTION_H
#define E3_PROJECTION_H

#include "Matrix4.h"

namespace E 
{
namespace Math
{
/*----------------------------------------------------------------------------------------------------------------------
Math (projection)

Please note that Math conversion functions have the following usage contract: 

1. BuildPerspectiveLH and BuildPerspectiveRH fov argument value must be expressed in degrees.
----------------------------------------------------------------------------------------------------------------------*/
static const U32 kMaxFieldOfView = 120;
static const U32 kMinFieldOfView = 10;

Matrix4f	BuildOrthographicLH(U32 width, U32 height, F32 nearZ, F32 farZ);
Matrix4f	BuildOrthographicRH(U32 width, U32 height, F32 nearZ, F32 farZ);
Matrix4f	BuildPerspectiveLH(U32 fov, F32 aspectRatio, F32 nearZ, F32 farZ);
Matrix4f	BuildPerspectiveRH(U32 fov, F32 aspectRatio, F32 nearZ, F32 farZ);

/*----------------------------------------------------------------------------------------------------------------------
Math methods (projection)
----------------------------------------------------------------------------------------------------------------------*/

inline Matrix4f BuildOrthographicLH(U32 width, U32 height, F32 nearZ, F32 farZ)
{
  /* Orthographic  (Left Handed)

  2 / width	0			      0						            0
  0			    2 / height	0						            0
  0			    0			      1 / (farZ - nearZ)		  0
  0			    0			      nearZ / (nearZ - farZ)	1
  */

  // By default matrix is identity
  Matrix4f projectionMatrix;

  projectionMatrix[0]  = 2.0f / width;
  projectionMatrix[5]  = 2.0f / height;
  projectionMatrix[10] = 1.0f / (farZ - nearZ);
  projectionMatrix[14] = nearZ / (nearZ - farZ);

  return projectionMatrix;
}

inline Matrix4f BuildOrthographicRH(U32 width, U32 height, F32 nearZ, F32 farZ)
{
  /* Orthographic (Right Handed)

  2 / width	0			      0						            0
  0			    2 / height	0						            0
  0			    0			      1 / (nearZ - farZ)		  0
  0			    0			      nearZ / (nearZ - farZ)	1
  */

  // By default matrix is identity
  Matrix4f projectionMatrix;

  projectionMatrix[0]  = 2.0f / width;
  projectionMatrix[5]  = 2.0f / height;

  F32 nearSubFarInv = 1.0f / (nearZ - farZ);
  projectionMatrix[10] = 1.0f * nearSubFarInv;
  projectionMatrix[14] = nearZ * nearSubFarInv;

  return projectionMatrix;
}

inline Matrix4f BuildPerspectiveLH(U32 fov, F32 aspectRatio, F32 nearZ, F32 farZ)
{
  /* Perspective (Left Handed)

  a / aspectRatio	0			0									              0
  0					      a			0									              0
  0					      0			farZ / (farZ - nearZ)				    1
  0					      0			-nearZ * farZ / (farZ - nearZ)	0

  where a = cot(fov / 2)
  */

  // By default matrix is identity
  Matrix4f projectionMatrix;

  // Clamp fov value between minimum and maximum
  fov = Math::Clamp(fov, kMinFieldOfView, kMaxFieldOfView);

  // Compute "a"
  F32 a = 1.0f / tan(fov * Math::kPiDiv180f * 0.5f);

  // Compute aspect ratio related values
  if (aspectRatio >= 1.0f)
  {
    projectionMatrix[0]  = a / aspectRatio;
    projectionMatrix[5]  = a;
  }
  else
  {
    projectionMatrix[0]  = a;
    projectionMatrix[5]  = a * aspectRatio;
  }

  // Compute nearZ farZ related values
  F32 farSubNear = farZ - nearZ;
  F32 farSubNearInv = 1.0f / farSubNear;

  projectionMatrix[10] = farZ * farSubNearInv;
  projectionMatrix[11] = 1.0f;
  projectionMatrix[14] = - nearZ * farZ * farSubNearInv;
  projectionMatrix[15] = 0.0f;

  return projectionMatrix;
}

inline Matrix4f BuildPerspectiveRH(U32 fov, F32 aspectRatio, F32 nearZ, F32 farZ)
{
  // D3DXMatrixPerspectiveFovLH (Right Handed)
  // a = atan(fov /2 )
  // 
  // 	a / aspectRatio	0			0							          0
  //  0					      a			0							          0
  //  0					      0			-farZ / (farZ - nearZ)	-nearZ * farZ / (farZ - nearZ)
  //  0					      0			-1							        0

  // By default matrix is identity
  Matrix4f projectionMatrix;

  // Clamp fov value between minimum and maximum
  fov = Math::Clamp(fov, kMinFieldOfView, kMaxFieldOfView);

  // Compute "a"
  F32 a = 1.0f / tan(fov * Math::kPiDiv180f * 0.5f);

  // Compute aspect ratio related values
  if (aspectRatio < 1.0f)
  {
    projectionMatrix[0]  = a / aspectRatio;
    projectionMatrix[5]  = a;
  }
  else
  {
    projectionMatrix[0]  = a;
    projectionMatrix[5]  = a * aspectRatio;
  }

  // Compute nearZ farZ related values
  F32 farSubNear = farZ - nearZ;
  F32 farSubNearInv = 1.0f / farSubNear;

  projectionMatrix[10] = -farZ * farSubNearInv;
  projectionMatrix[11] = - nearZ * farZ * farSubNearInv;
  projectionMatrix[14] = - 1.0f;
  projectionMatrix[15] = 0.0f;

  return projectionMatrix;
}
}
}

#endif
