#ifndef CR_LIGHT_MANAGER
#define CR_LIGHT_MANAGER

#include "..\Inc\CRLight.h"
/**/
class LightManager
{
private:
	LightManager();
	static LightManager* ins;
	unordered_map<unsigned int, Light*> lights;
	float4 zenithColor;
	unsigned int lightID;

public:
	~LightManager();
	static LightManager* Instance();

	unsigned int CreateNewLight(LightType type, float i);
	Light* GetLight(int index);
	void DeleteLight(int index);
	float4 GetZenithColor();
	void SetZenithColor(float4 c);
};
/**/
#endif