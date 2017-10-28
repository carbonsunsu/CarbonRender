#ifndef CR_LIGHT
#define CR_LIGHT

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRObject.h"
#include "..\Inc\CRMath.h"

enum LightType
{
	Direction = 0,
	Point = 1,
	Spot = 2,
};

class Light : public Object
{
private:
	LightType type;
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