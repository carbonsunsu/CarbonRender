#include "..\Inc\CRLightManager.h"
/**/
LightManager* LightManager::ins = nullptr;

LightManager::LightManager()
{
	lightID = 0;
}

LightManager::~LightManager()
{
	for (unordered_map<unsigned int, Light*>::iterator i = lights.begin(); i != lights.end(); i++)
		delete i->second;

	lights.clear();
	ins = nullptr;
}

LightManager * LightManager::Instance()
{
	if (ins == nullptr)
		ins = new LightManager();

	return ins;
}

unsigned int LightManager::CreateNewLight(LightType type, float i)
{
	unsigned int newLightID = lightID;
	lightID++;

	Light* newLight = new Light(type, i);
	lights[newLightID] = newLight;

	return newLightID;
}

Light* LightManager::GetLight(int index)
{
	return lights[index];
}

void LightManager::DeleteLight(int index)
{
	delete lights[index];
	lights.erase(index);
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