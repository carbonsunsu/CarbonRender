#include "..\Inc\CRLightManager.h"
/**/
LightManager* LightManager::ins = nullptr;

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
	lights.shrink_to_fit();
}

LightManager * LightManager::Instance()
{
	if (ins == nullptr)
		ins = new LightManager();

	return ins;
}

int LightManager::AddLight(Light light)
{
	lights.emplace_back(light);
	return lights.size() - 1;
}

Light* LightManager::GetLight(int index)
{
	return &lights[index];
}

void LightManager::ClearLight()
{
	lights.shrink_to_fit();
}

float4 LightManager::GetZenithColor()
{
	return zenithColor;
}

void LightManager::SetZenithColor(float4 c)
{
	zenithColor = c;
}
/**/