#ifndef CR_WEATHERSYS
#define CR_WEATHERSYS

#include "..\Inc\CRLightManager.h"
#include "..\Inc\CRTextureManager.h"

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

	char* weatherMapPath;
	GLuint weatherMapId;
	float cloudCoverage;
	float cloudPrecipitation;
	GLuint cloudNoises[3];//PerlinWorley, Worley, Curl

	float fogDensity;

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
	void SetCloudPrecipitation(float p);
	void AddCloudPrecipitation(float p);
	float GetCloudPrecipitation();
	void SetWeatherMap(char* path);
	char* GetWeatherMapPath();
	GLuint GetWeatherMapId();
	void GenerateCloudNoise();
	GLuint GetBaseNoise();
	GLuint GetDetailNoise();
	GLuint GetCurlNoise();
	void SetFogDensity(float d);
	void AddFogDensity(float d);
	float GetFogDensity();
	void ToggleTimeLapse();
	void SetTimeStop(bool bStop);
	bool IsTimeStop();

	float* GetShaderParas();
	float3 GetCloudBias();
};

#endif
