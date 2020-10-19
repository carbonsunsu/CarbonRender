#include "..\Inc\CRLight.h"

Light::Light(LightType iType, float iIntensity)
{
	objType = ObjectType::eLight;
	lightType = iType;
	SetIntensity(iIntensity);
	lightSize = 1.0f;
	nearClip = 1.0f;
	farClip = 100.0f;
	shadowMapSize = 2048;
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

void Light::SetNearClip(float nearDis)
{
	nearClip = nearDis;
}

float Light::GetNearClip()
{
	return nearClip;
}

void Light::SetFarClip(float farDis)
{
	farClip = farDis;
}

float Light::GetFarClip()
{
	return farClip;
}

void Light::SetLightSize(float size)
{
	lightSize = size;
}

float Light::GetLightSize()
{
	return lightSize;
}

void Light::SetShadowMapSize(int size)
{
	shadowMapSize = size;
}

int Light::GetShadowMapSize()
{
	return shadowMapSize;
}

float3 Light::GetDepthClampPara()
{
	return float3(nearClip, farClip, 1.0f / farClip);
}
