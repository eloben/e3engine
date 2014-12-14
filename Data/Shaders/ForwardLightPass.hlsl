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

/** @file ForwardLightPass.hlsl
This file defines the forward renderer default shader
*/

#pragma pack_matrix (row_major)

#include "Lighting.hlsl"

#include "Conversion.hlsl"

cbuffer interFrameBufferLit : register(b0)
{
  struct
  {
	  float4x4  viewProjectionMatrix;
    float4x4  projectionMatrix;
    float4x4  viewMatrix;
    float3    position;
  }camera;
};

cbuffer intraFrameBufferLit : register(b1)
{
  struct
  {
	  float4x4  viewProjectionMatrix;
    float4    direction;
    float4    color;
    float4    attenuation;
    float4    position;
  }light;
};

#ifdef E_HAS_DIFFUSE_MAP
Texture2D		                DiffuseTexture  : register(t0);
SamplerState	              DiffuseSampler  : register(s0);
#endif

#ifdef E_HAS_SHADOW_MAP
Texture2D		                ShadowTexture  : register(t4);
SamplerComparisonState      ShadowSampler  : register(s4);
#endif

StructuredBuffer<Material>  MaterialBuffer  : register(t8);
StructuredBuffer<float4x4>  TransformBuffer : register(t9);

struct VSInput
{
  float3 position	  : POSITION;
  uint	 meshID		  : ID0;
  #ifdef E_HAS_DIFFUSE_MAP
    float2 tex0		  : TEXCOORD0;
  #endif
  float3 normal		  : NORMAL0;
};

struct PSInput
{
  float4 positionH  : SV_POSITION;
  uint	 meshID		  : ID0;
  #ifdef E_HAS_DIFFUSE_MAP
    float2 tex0		  : TEXCOORD0;
  #endif
  float3 position   : POSITION0;
  float3 normal	    : NORMAL0;
  float4 lightViewPositionH : POSITION1;
};

PSInput VS(VSInput input)
{
  PSInput output;

  // Calculate first world space position
  output.positionH = mul(float4(input.position.xyz, 1.0f), TransformBuffer[input.meshID]);
  output.position = output.positionH.xyz;

  output.lightViewPositionH = mul(output.positionH, light.viewProjectionMatrix);

   // Calculate homegeneous space position
  output.positionH = mul(output.positionH, camera.viewProjectionMatrix);

  // Calculate world space normal
  output.normal = mul(input.normal.xyz, (float3x3)TransformBuffer[input.meshID]);

  // Forward texture coordinates and mesh ID
  #ifdef E_HAS_DIFFUSE_MAP
    output.tex0 = input.tex0;
  #endif
  output.meshID = input.meshID;

  return output;
}

float4 PS(PSInput input): SV_TARGET
{
  // Load material
  Material materialInfo;
  #ifdef E_HAS_DIFFUSE_MAP
    materialInfo.diffuseColor = DiffuseTexture.Sample(DiffuseSampler, input.tex0) * MaterialBuffer[input.meshID].diffuseColor;
  #else
    materialInfo.diffuseColor = MaterialBuffer[input.meshID].diffuseColor;
  #endif
  materialInfo.specularColor = MaterialBuffer[input.meshID].specularColor;

  // Normalize input.normal to avoid per-vertex artifacts
  input.normal = normalize(input.normal);
  
  // Calculate viewer direction from world position
  float3 viewer = normalize(camera.position - input.position);

  // Load light
  Light lightInfo;
  //lightInfo.color = DecodeColor(asuint(light.direction.w));
  lightInfo.color = light.color.rgb;

  #ifdef E_HAS_LIGHT_DIRECTIONAL
    lightInfo.direction = light.direction.xyz;

      #ifdef E_HAS_SHADOW_MAP
      float2 projectTexCoord;
      projectTexCoord.x =  input.lightViewPositionH.x / input.lightViewPositionH.w / 2.0f + 0.5f;
      projectTexCoord.y = -input.lightViewPositionH.y / input.lightViewPositionH.w / 2.0f + 0.5f;
      
      // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
      if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
      {
        // Calculate the depth of the light.
        float lightDepthValue = input.lightViewPositionH.z / input.lightViewPositionH.w;

        //float depthValue = ShadowTexture.Sample(ShadowSampler, projectTexCoord).r;
        float depthValue = ShadowTexture.SampleCmp(ShadowSampler, projectTexCoord, lightDepthValue - 0.025f);

        // Subtract the bias from the lightDepthValue.
        return ComputeNormalizedBlinn(lightInfo, materialInfo, input.normal, viewer) * depthValue;
      }
      return float4(0,0,0,0);
      #else
      return ComputeNormalizedBlinn(lightInfo, materialInfo, input.normal, viewer);
      #endif

  #else
    // Compute light vector
    float3 lightVector = input.position - light.position;
    float lightDistance = length(lightVector);
    lightVector /= lightDistance;

    #ifdef E_HAS_LIGHT_POINT
      lightInfo.direction = lightVector;
      return ComputeNormalizedBlinn(lightInfo, materialInfo, input.normal, viewer) * ComputeAttenuation(light.attenuation, lightDistance);
    #endif
    #ifdef E_HAS_LIGHT_SPOT
      
      #ifdef E_HAS_SHADOW_MAP
      
      float2 projectTexCoord;
      projectTexCoord.x =  input.lightViewPositionH.x / input.lightViewPositionH.w / 2.0f + 0.5f;
      projectTexCoord.y = -input.lightViewPositionH.y / input.lightViewPositionH.w / 2.0f + 0.5f;

      // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
      if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
      {
        // Calculate the depth of the light.
        float lightDepthValue = input.lightViewPositionH.z / input.lightViewPositionH.w;

        //float depthValue = ShadowTexture.Sample(ShadowSampler, projectTexCoord).r;
        float depthValue = ShadowTexture.SampleCmp(ShadowSampler, projectTexCoord, lightDepthValue - 0.0005f);

        // Subtract the bias from the lightDepthValue.
       // lightDepthValue = lightDepthValue - 0.001f;

        lightInfo.direction = light.direction;
	      float spot = max(dot(lightVector, light.direction), 0.0f);

        // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
        if(spot > light.position.w)
        {
          return ComputeNormalizedBlinn(lightInfo, materialInfo, input.normal, viewer) * spot * ComputeAttenuation(light.attenuation, lightDistance) * depthValue;
        }
      }
      return float4(0,0,0,0);
      #else
      lightInfo.direction = light.direction;
	    float spot = max(dot(lightVector, light.direction), 0.0f);
      return (spot > light.position.w) ? ComputeNormalizedBlinn(lightInfo, materialInfo, input.normal, viewer) * spot * ComputeAttenuation(light.attenuation, lightDistance) : float4(0,0,0,0);
      #endif
    #endif
    
    // Error
    return float4(1,0,1,1);
  #endif
}
