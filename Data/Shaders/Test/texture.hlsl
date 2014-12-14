cbuffer cameraBuffer : register(b0)
{
  struct
  {
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
  }camera;
};

Texture2D		ColorTexture : register(t0);
SamplerState	ColorSampler : register(s0);

struct VSInput
{
  float3 position	: POSITION;
  float2 tex0		: TEXCOORD0;
};

struct PSInput
{
  float4 position	: SV_POSITION;
  float2 tex0		: TEXCOORD0;
};

PSInput VS(VSInput input)
{
  PSInput output;
  output.position = mul(float4(input.position.xyz, 1.0f), camera.worldMatrix);
  output.position = mul(output.position, camera.viewMatrix);
  output.position = mul(output.position, camera.projMatrix);
  output.tex0 = input.tex0;
  return output;
}

float4 PS(PSInput input): SV_TARGET
{
  return ColorTexture.Sample(ColorSampler, input.tex0);
}
