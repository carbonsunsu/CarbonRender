#ifndef CR_WEATHERSYS
#define CR_WEATHERSYS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMath.h"

class WeatherSystem
{
private:
	float latitude = 45.0f;
	int day = 200;
	float hour = 8.0f;
	float turbidity = 2.0f;
	float exposure = 0.04f;
	float timeSpeed = 1.0f;

	static float4 Yxy2RGB(float Y, float x, float y);
};

#endif
