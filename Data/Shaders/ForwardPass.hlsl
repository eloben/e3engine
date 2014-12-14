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

// Created 24-Oct-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ForwardPass.hlsl
This file defines the forward renderer default shader
*/

#pragma pack_matrix (row_major)

#include "Lighting.hlsl"

cbuffer interFrameBuffer : register(b0)
{
  struct
  {
	float4x4 viewProjectionMatrix;
  }camera;
};

#ifndef E_HAS_DEPTH_TARGET_ONLY
#ifdef E_HAS_DIFFUSE_MAP
Texture2D		                DiffuseTexture  : register(t0);
SamplerState	              DiffuseSampler  : register(s0);
#endif
StructuredBuffer<Material>  MaterialBuffer  : register(t8);
#endif
StructuredBuffer<float4x4>  TransformBuffer : register(t9);

struct VSInput
{
  float3 position	: POSITION;
  uint	 meshID		: ID0;
  #ifdef E_HAS_DIFFUSE_MAP
    float2 tex0		: TEXCOORD0;
  #endif
};

struct PSInput
{
  float4 position	: SV_POSITION;
  #ifndef E_HAS_DEPTH_TARGET_ONLY
    uint	 meshID		: ID0;
    #ifdef E_HAS_DIFFUSE_MAP
      float2 tex0		: TEXCOORD0;
    #endif
  #endif
};

PSInput VS(VSInput input)
{
  PSInput output;
  output.position = mul(float4(input.position.xyz, 1.0f), TransformBuffer[input.meshID]);
  output.position = mul(output.position, camera.viewProjectionMatrix);
  #ifndef E_HAS_DEPTH_TARGET_ONLY
  output.meshID = input.meshID;
  #ifdef E_HAS_DIFFUSE_MAP
    output.tex0 = input.tex0;
  #endif
  #endif
  return output;
}

#ifndef E_HAS_DEPTH_TARGET_ONLY
float4 PS(PSInput input): SV_TARGET
{
  #ifdef E_HAS_DIFFUSE_MAP
    float4 color = DiffuseTexture.Sample(DiffuseSampler, input.tex0) * MaterialBuffer[input.meshID].diffuseColor;
  #else
    float4 color = MaterialBuffer[input.meshID].diffuseColor;
  #endif

  #ifdef E_HAS_LIGHT_AMBIENT
    color.rgb *= 0.01f;
  #endif

  return color;
}
#else
//void PS(PSInput input) {}

#endif