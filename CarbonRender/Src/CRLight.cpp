#include "..\Inc\CRLight.h"

Light::Light(LightType iType, float iIntensity)
{
	objType = ObjectType::eLight;
	lightType = iType;
	SetIntensity(iIntensity);
}

Light::~Light()
{
}

LightType Light::GetType()
{
	return lightType;
}

float Light::GetIntensity()
{
	return intensity;
}

void Light::SetIntensity(float iIntensity)
{
	intensity = Math::Max(0.0f, iIntensity);
}

float3 Light::GetColor()
{
	return color * intensity;
}

void Light::SetColor(float3 iColor)
{
	color = iColor;
}
