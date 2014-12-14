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

// Created 18-Jun-2014 by Elías Lozada-Benavente
// Original 10-Apr-2012 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Color.hlsl
This file defines the default color shader
*/

#include "../Conversion.hlsl"

#pragma pack_matrix (row_major)

cbuffer cameraBuffer : register(b0)
{
  struct
  {
	  float4x4 viewProjectionMatrix;
  }camera;
};

StructuredBuffer<float4x4> transformBuffer : register(t9);

struct VSInput
{
  float3  position  : POSITION;
  uint	  meshID    : ID0;
  uint    color	    : COLOR0;
};

struct PSInput
{
  float4 position : SV_POSITION;
  float4 color	  : COLOR0;
};

PSInput VS(VSInput input)
{
  PSInput output;
  output.position = mul(float4(input.position.xyz, 1.0f), transformBuffer[input.meshID]);
  output.position = mul(output.position, camera.viewProjectionMatrix);
  output.color.xyz = DecodeColor(input.color);
  output.color.w = 1.0f;
  
  return output;
}

float4 PS(PSInput input): SV_TARGET
{
  return input.color;
}
