#ifndef CR_WEATHERSYS
#define CR_WEATHERSYS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRLightManager.h"

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
	bool isTimeStop;
	float lightR;

	float thetaS;
	float4 wsSunPos;
	float3 zenith; // xyY

	float4 windDir;
	float3 cloudBias;

	float shaderParas[10];

	int sunLight;

	float cloudCoverage;

	void Init();
	void UpdateAtmosphere();
	void UpdateCloud();

	float4 GetSunColor();
	float4 GetZenithColor();
public:
	static WeatherSystem* Instance();
	~WeatherSystem();
	
	void Update();

	void SetLatitude(float l);
	float GetLatitude();
	void SetDay(int d);
	int GetDay();
	void SetHour(float h);
	float GetHour();
	void SetTurbidity(float t);
	float GetTurbidity();
	void SetExposure(float exp);
	float GetExposure();
	void SetTimeSpeed(float t);
	float GetTimeSpeed();
	void SetWindDirection(float3 d);
	float3 GetWindDirection();
	void SetWindStrength(float s);
	float GetWindStrength();
	void SetCloudCoverage(float c);
	void AddCloudCoverage(float c);
	float GetCloudCoverage();
	void ToggleTimeLapse();
	void SetTimeStop(bool bStop);
	bool IsTimeStop();

	float* GetShaderParas();
	float3 GetCloudBias();
};

#endif
