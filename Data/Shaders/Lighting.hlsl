/*----------------------------------------------------------------------------------------------------------------------
Light
----------------------------------------------------------------------------------------------------------------------*/
struct Light
{
  float3 direction;
  float3 color;
};

struct LightPoint
{
  float3 color;
  float3 position;
  float4 attenuation;
};

struct LightSpot
{
  float3 direction;
  float3 color;
  float3 position;
  float4 attenuation;
  float  cosCutOffAngle;
  float4x4 viewProjectionMatrix;
};

/*----------------------------------------------------------------------------------------------------------------------
Material
----------------------------------------------------------------------------------------------------------------------*/
struct Material
{
  float4 diffuseColor;
  float4 specularColor;
};

/*----------------------------------------------------------------------------------------------------------------------
Lighting methods
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
SpecularPower

Implementation based on:
http://seblagarde.wordpress.com/2011/08/17/hello-world/
----------------------------------------------------------------------------------------------------------------------*/
float SpecularPower(in float gloss)
{ 
  return exp2(10 * gloss + 1);
}

/*----------------------------------------------------------------------------------------------------------------------
ComputeAttenuation

Implementation based on:
http://content.gpwiki.org/D3DBook:%28Lighting%29_Direct_Light_Sources

Please note that this method has the following usage contract: 

  1. The attenuationParams interpretation is:
    x: quadratic attenuation
    y: linear attenuation
    z: constant attenuation
    w: range
----------------------------------------------------------------------------------------------------------------------*/
float ComputeAttenuation(in float4 attenuationParams, float lightDistance)
{ 
  float attenuation = 1.0f / (attenuationParams.x * lightDistance * lightDistance + attenuationParams.y * lightDistance + attenuationParams.z); 
  // Use the step() intrinsic to clamp light to  zero out of its defined range
  return step(lightDistance, attenuationParams.w) * saturate(attenuation);
}

/*----------------------------------------------------------------------------------------------------------------------
ComputeBlinn (25 instruction slots)

Implementation based on:
- Crafting Physically Motivated Shading Models for Game Development by Naty Hoffman. Activision. SIGGRAPH 2010 Course

  Lo(v) = (Cdiff * saturate(N•L) + [Cspec * pow(saturate(H•N), specularPower) if (N•L) > 0]) x Clight
  
H is the half vector between the viewer and the light direction.
----------------------------------------------------------------------------------------------------------------------*/
float4 ComputeBlinn(in Light light, in Material material, in float3 normal, in float3 viewer)
{ 
  float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
  float3 specularColor = float3(0.0f, 0.0f, 0.0f);
  
  float NdotL = saturate(dot(normal, -light.direction)); // L = -light.direction
  
  [flatten]
  if(NdotL > 0.0f)
	{
    diffuseColor = material.diffuseColor.xyz * NdotL; // (no need to saturate diffuseColor)
    // Calculate the half vector based on the light and viewer direction.
		float3 halfVector = normalize(viewer -light.direction);
		// Determine the amount of the specularColor component.
		specularColor = saturate(material.specularColor.xyz * pow(saturate(dot(halfVector, normal)), SpecularPower(material.specularColor.w)));
  }

  return float4(saturate((diffuseColor + specularColor) * light.color), material.diffuseColor.a);
}

/*----------------------------------------------------------------------------------------------------------------------
ComputeBlinn2 (22 instruction slots)

Implementation based on:
- Crafting Physically Motivated Shading Models for Game Development by Naty Hoffman. Activision. SIGGRAPH 2010 Course

  Lo(v) = (Cdiff + Cspec * pow(saturate(H•N), specularPower) x Clight * saturate(N•L)

H is the half vector between the viewer and the light direction.
----------------------------------------------------------------------------------------------------------------------*/
float4 ComputeBlinn2(in Light light, in Material material, in float3 normal, in float3 viewer)
{ 
  // Calculate the light color
  float3 color = light.color * saturate(dot(normal, -light.direction)); // L = -light.direction (no need to saturate diffuseColor)
  // Calculate the half vector based on the light and viewer direction.
	float3 halfVector = normalize(viewer - light.direction); // L = -light.direction
	// Determine the amount of the specularColor component.
	float3 specularColor = saturate(material.specularColor.xyz * pow(saturate(dot(halfVector, normal)), SpecularPower(material.specularColor.w)));

  return float4(saturate((material.diffuseColor.xyz + specularColor) * color), material.diffuseColor.a);
}

/*----------------------------------------------------------------------------------------------------------------------
ComputeFresnelSchlickTerm

Implementation based on:
http://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations/
http://en.wikipedia.org/wiki/Schlick%27s_approximation
----------------------------------------------------------------------------------------------------------------------*/
float3 ComputeFresnelSchlickTerm(in float3 materialSpecular, in float3 viewer, in float3 halfVector)
{
  return materialSpecular + (1.0f - materialSpecular) * pow(1.0f - saturate(dot(viewer, halfVector)), 5);
}

/*----------------------------------------------------------------------------------------------------------------------
ComputeNormalizedBlinn (32 instruction slots)

Implementation based on:
- Crafting Physically Motivated Shading Models for Game Development by Naty Hoffman. Activision. SIGGRAPH 2010 Course

  Lo(v) = Cdiff + ((specularPower + 2) / 8) * pow(saturate(H•N), specularPower) * Fschlick(Cspec, V, H)) * Clight * saturate(N•L)
----------------------------------------------------------------------------------------------------------------------*/
float4 ComputeNormalizedBlinn(in Light light, in Material material, in float3 normal, in float3 viewer)
{ 
  // Calculate the light color
  float3 color = light.color * saturate(dot(normal, -light.direction)); // L = -light.direction (no need to saturate diffuseColor)
  // Calculate the half vector based on the light and viewer direction.
	float3 halfVector = normalize(viewer - light.direction); // L = -light.direction
  // Determine the amount of the specularColor component.
  float3 specularColor = (SpecularPower(material.specularColor.w) + 2) * 0.125f * pow(saturate(dot(halfVector, normal)), SpecularPower(material.specularColor.w));
  // Add Schlick approximation of Fresnel term.
  specularColor = saturate(specularColor * ComputeFresnelSchlickTerm(material.specularColor.xyz, viewer, halfVector));

  return float4(saturate((material.diffuseColor.xyz + specularColor) * color), material.diffuseColor.a);
}

/*----------------------------------------------------------------------------------------------------------------------
ComputePhong (24 instruction slots)

Implementation based on:
- Crafting Physically Motivated Shading Models for Game Development by Naty Hoffman. Activision. SIGGRAPH 2010 Course

  Lo(v) = (Cdiff * saturate(N•L) + [Cspec * pow(saturate(Rv•L), specularPower) if (N•L) > 0]) x Clight
  
Rv is the reflected viewer incident vector respect the normal. A more common representation is:

  Lo(v) = (Cdiff * saturate(N•L) + [Cspec * pow(saturate(Rl•V), specularPower) if (N•L) > 0]) x Clight

Rl is the reflected incident light vector respect the normal.
----------------------------------------------------------------------------------------------------------------------*/
float4 ComputePhong(in Light light, in Material material, in float3 normal, in float3 viewer)
{ 
  float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
  float3 specularColor = float3(0.0f, 0.0f, 0.0f);
  
  float NdotL = saturate(dot(normal, -light.direction)); // L = -light.direction
  
  [flatten]
  if(NdotL > 0.0f)
	{
    diffuseColor = material.diffuseColor.xyz * NdotL; // (no need to saturate diffuseColor)
    // Calculate the reflection vector based on the normal and light direction.
		float3 reflection = reflect(light.direction, normal);
		// Determine the amount of the specularColor component.
		specularColor = saturate(material.specularColor.xyz * pow(saturate(dot(reflection, viewer)), SpecularPower(material.specularColor.w)));
  }

  return float4(saturate((diffuseColor + specularColor) * light.color), material.diffuseColor.a);
}

/*----------------------------------------------------------------------------------------------------------------------
ComputePhong2 (23 instruction slots)

Implementation based on:
- Crafting Physically Motivated Shading Models for Game Development by Naty Hoffman. Activision. SIGGRAPH 2010 Course

  Lo(v) = (Cdiff + Cspec * pow(saturate(Rv•L), specularPower) x Clight * saturate(N•L)

Rv is the reflected viewer incident vector respect the normal. A more common representation is:

  Lo(v) = (Cdiff + Cspec * pow(saturate(Rl•V), specularPower) x Clight * saturate(N•L)

Rl is the reflected incident light vector respect the normal.

Please note that this method has the following usage contract: 

  1. Specular component is defaulted to zero when specularColor power is zero to avoid color aberrations
----------------------------------------------------------------------------------------------------------------------*/
float4 ComputePhong2(in Light light, in Material material, in float3 normal, in float3 viewer)
{ 
  // Calculate the light color
  float3 color = light.color * saturate(dot(normal, -light.direction)); // L = -light.direction (no need to saturate diffuseColor)
  // Calculate the reflection vector based on the normal and light direction.
	float3 reflection = reflect(light.direction, normal);
	// Determine the amount of the specularColor component.
	float3 specularColor = (SpecularPower(material.specularColor.w) > 0.0f) ? saturate(material.specularColor.xyz * pow(saturate(dot(reflection, viewer)), SpecularPower(material.specularColor.w))) : float3(0.0f, 0.0f, 0.0f);

  return float4(saturate((material.diffuseColor.xyz + specularColor) * color), material.diffuseColor.a);
}
