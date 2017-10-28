#include "..\Inc\CRLight.h"

Light::Light(LightType iType, float iIntensity)
{
	type = iType;
	intensity = iIntensity;
}

Light::~Light()
{
}

LightType Light::GetType()
{
	return type;
}

float Light::GetIntensity()
{
	return intensity;
}

void Light::SetIntensity(float iIntensity)
{
	intensity = iIntensity;
}

float3 Light::GetColor()
{
	return color * intensity;
}

void Light::SetColor(float3 iColor)
{
	color = iColor;
}
