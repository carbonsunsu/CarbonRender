#ifndef CR_LIGHT
#define CR_LIGHT

#include "..\Inc\CRObject.h"

enum LightType
{
	eDirection = 0,
	ePoint = 1,
	eSpot = 2,
};

class Light : public Object
{
private:
	LightType lightType;
	float intensity;
	float3 color;

public:
	Light(LightType iType, float iIntensity);
	~Light();

	LightType GetType();
	float GetIntensity();
	void SetIntensity(float iIntensity);
	float3 GetColor();
	void SetColor(float3 iColor);
};

#endif