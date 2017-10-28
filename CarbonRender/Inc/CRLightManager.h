#ifndef CR_LIGHT_MANAGER
#define CR_LIGHT_MANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRLight.h"
/**/
class LightManager
{
private:
	LightManager();
	static LightManager* ins;
	vector<Light> lights;
	float4 zenithColor;

public:
	~LightManager();
	static LightManager* Instance();

	int AddLight(Light light);
	Light* GetLight(int index);
	void DeleteLight();
	void ClearLight();
	float4 GetZenithColor();
	void SetZenithColor(float4 c);
};
/**/
#endif