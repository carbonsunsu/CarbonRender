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

	float4 windDir;
	float3 cloudBias;

	float shaderParas[10];

	void Init();
	void UpdateAtmosphere();
	void UpdateCloud();
public:
	static WeatherSystem* Instance();
	~WeatherSystem();
	
	void Update();

	void SetHour(float h);
	float GetHour();
	float4 GetWsSunPos();
	float4 GetSunColor();
	float4 GetSkyUpColor();
	float* GetShaderParas();

	float3 GetCloudBias();
};

#endif
