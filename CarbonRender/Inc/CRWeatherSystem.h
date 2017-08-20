#ifndef CR_WEATHERSYS
#define CR_WEATHERSYS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMath.h"

class WeatherSystem
{
private:
	static WeatherSystem* ins;
	WeatherSystem();

	float latitude;
	int day;
	float hour;
	float turbidity;
	float exposure;
	float timeSpeed;
	float lightR;

	float thetaS;
	float4 wsSunPos;
	float3 zenith; // xyY

	float shaderParas[10];

	void Init();
public:
	static WeatherSystem* Instance();
	~WeatherSystem();
	
	void Update();

	float4 GetWsSunPos();
	float4 GetSunColor();
	float4 GetSkyUpColor();
	float* GetShaderParas();
};

#endif
