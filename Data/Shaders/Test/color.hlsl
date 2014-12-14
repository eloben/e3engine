cbuffer cameraBuffer : register(b0)
{
  struct
  {
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
  }camera;
};

struct VSInput
{
  float3 position : POSITION;
};

struct PSInput
{
  float4 position : SV_POSITION;
};

PSInput VS(VSInput input)
{
  PSInput output;
  output.position = mul(float4(input.position.xyz, 1.0f), camera.worldMatrix);
  output.position = mul(output.position, camera.viewMatrix);
  output.position = mul(output.position, camera.projMatrix);
  
  return output;
}

float4 PS(PSInput input): SV_TARGET
{
  return float4(0.0f, 0.2f, 0.0f, 1.0f);
}
