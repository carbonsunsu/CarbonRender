#include "..\Inc\CRWeatherSystem.h"

WeatherSystem* WeatherSystem::ins = nullptr;

WeatherSystem::WeatherSystem()
{
	Init();
}

WeatherSystem * WeatherSystem::Instance()
{
	if (ins == nullptr)
		ins = new WeatherSystem();

	return ins;
}

WeatherSystem::~WeatherSystem()
{
	ins = nullptr;
}

void WeatherSystem::Init()
{
	latitude = 0.0f;
	day = 180;
	hour = 12.0f;
	turbidity = 2.0f;
	exposure = 30.0f;
	timeSpeed = 10.0f;
	lightR = 1000.0f;
	windDir = float4(0.5f, 0.0f, 1.0f, 10.0f * timeSpeed);
	cloudBias = float3(0.0f);

	Update();
}

void WeatherSystem::UpdateAtmosphere()
{
	//X->E	-X->W
	//Z->S	-Z->N
	float l = latitude * A2R;
	float paraA = 0.17f * sin(4.0f * PI * (day - 80.0f) / 373.0f);
	float paraB = 0.129f * sin(2.0f * PI * (day - 8.0f) / 355.0f);
	float t = hour + paraA - paraB;
	float delta = 0.4093f * sin(2.0f * PI * (day - 81.0f) / 368.0f);
	thetaS = PI * 0.5f - asin(sin(l) * sin(delta) - cos(l) * cos(delta) * cos(PI * t / 12.0f));
	float Phi = atan((-cos(delta) * sin(PI * t / 12.0f)) / (cos(l) * sin(delta) - sin(l) * cos(delta) * cos(PI * t / 12.0f)));
	wsSunPos.y = lightR * cos(thetaS);
	wsSunPos.x = -lightR * sin(thetaS) * sin(Phi);
	wsSunPos.z = lightR * sin(thetaS) * cos(Phi);
	wsSunPos.w = 1.0f;

	float X = ((4.0f / 9.0f) - (turbidity / 120.0f)) * (PI - 2.0f * thetaS);
	zenith.z = (4.0453f * turbidity - 4.9710f) * tan(X) - 0.2155f * turbidity + 2.4192f;

	float ThetaS_3 = pow(thetaS, 3.0f);
	float ThetaS_2 = pow(thetaS, 2.0f);
	float T_2 = turbidity * turbidity;
	float4 temp(T_2 * 0.00166f - turbidity * 0.02903f + 0.11693f,
		-T_2 * 0.00375f + turbidity * 0.06377f - 0.21196f,
		T_2 * 0.00209f - turbidity * 0.03202f + 0.06052f,
		turbidity * 0.00394f + 0.25886f);
	zenith.x = ThetaS_3 * temp.x + ThetaS_2 * temp.y + thetaS * temp.z + temp.w;

	temp = float4(T_2 * 0.00275f - turbidity * 0.04214f + 0.15346f,
		-T_2 * 0.00610f + turbidity * 0.08970f - 0.26756f,
		T_2 * 0.00317f - turbidity * 0.04156f + 0.06670f,
		turbidity * 0.00516f + 0.26688f);
	zenith.y = ThetaS_3 * temp.x + ThetaS_2 * temp.y + thetaS * temp.z + temp.w;
}

void WeatherSystem::UpdateCloud()
{
	cloudBias = float3(windDir.x, windDir.y, windDir.z).normalize() * windDir.w * FIXEDUPDATE_TIME + cloudBias;
}

void WeatherSystem::Update()
{
	hour += FIXEDUPDATE_TIME * 0.0002778f * timeSpeed;
	if (hour >= 24.0f)
		hour = 0.0f;

	UpdateAtmosphere();
	UpdateCloud();
}

void WeatherSystem::SetHour(float h)
{
	hour = h;
}

float WeatherSystem::GetHour()
{
	return hour;
}

float4 WeatherSystem::GetWsSunPos()
{
	return wsSunPos;
}

float4 WeatherSystem::GetSunColor()
{
	if (wsSunPos.y <= 0.0f)
		return float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 up(0.0f, 1.0f, 0.0f);

	float3 vP = wsSunPos.normalize();
	float cosTheta = Dot(vP, up);

	float4 skyColor(0.0f, 0.0f, 0.0f, 1.0f);
	float3 A(-0.0193f*turbidity - 0.2592f, -0.0167f*turbidity - 0.2608f, 0.1787f*turbidity - 1.4630f);
	float3 B(-0.0665f*turbidity + 0.0008f, -0.0950f*turbidity + 0.0092f, -0.3554f*turbidity + 0.4275f);
	float3 C(-0.0004f*turbidity + 0.2125f, -0.0079f*turbidity + 0.2102f, -0.0227f*turbidity + 5.3251f);
	float3 D(-0.0641f*turbidity - 0.8989f, -0.0441f*turbidity - 1.6537f, 0.1206f*turbidity - 2.5771f);
	float3 E(-0.0033f*turbidity + 0.0452f, -0.0109f*turbidity + 0.0529f, -0.0670f*turbidity + 0.3703f);

	float3 F1 = (1.0f + A*exp(B / cosTheta))*(1.0f + C + E);
	float3 F2 = (1.0f + A*exp(B))*(1.0f + C*exp(D*thetaS) + E*pow(cos(thetaS), 2.0f));
	float3 xyY = zenith*(F1 / F2);
	xyY.z = 1.0f - exp((-1.0f/exposure)*xyY.z);

	skyColor = xyY2RGB(xyY);

	return Min(skyColor * 5.0f, 1.0f);
}

float4 WeatherSystem::GetSkyUpColor()
{
	float3 vP(0.0f, 1.0f, 0.0f);
	float3 vS = wsSunPos.normalize();
	float vSovP = Dot(vS, vP);
	float gamma = acos(vSovP);

	float4 zenithColor(0.0f, 0.0f, 0.0f, 1.0f);
	float3 A(-0.0193f*turbidity - 0.2592f, -0.0167f*turbidity - 0.2608f, 0.1787f*turbidity - 1.4630f);
	float3 B(-0.0665f*turbidity + 0.0008f, -0.0950f*turbidity + 0.0092f, -0.3554f*turbidity + 0.4275f);
	float3 C(-0.0004f*turbidity + 0.2125f, -0.0079f*turbidity + 0.2102f, -0.0227f*turbidity + 5.3251f);
	float3 D(-0.0641f*turbidity - 0.8989f, -0.0441f*turbidity - 1.6537f, 0.1206f*turbidity - 2.5771f);
	float3 E(-0.0033f*turbidity + 0.0452f, -0.0109f*turbidity + 0.0529f, -0.0670f*turbidity + 0.3703f);

	float3 F1 = (1.0f + A*exp(B))*(1.0f + C*exp(D*gamma) + E*pow(vSovP, 2.0f));
	float3 F2 = (1.0f + A*exp(B))*(1.0f + C*exp(D*thetaS) + E*pow(cos(thetaS), 2.0f));
	float3 xyY = zenith*(F1 / F2);
	xyY.z = 1.0f - exp((-1.0f / exposure)*xyY.z);

	zenithColor = xyY2RGB(xyY);

	return zenithColor;
}

float * WeatherSystem::GetShaderParas()
{
	shaderParas[0] = turbidity;
	shaderParas[1] = exposure;
	shaderParas[2] = thetaS;
	shaderParas[3] = wsSunPos.x;
	shaderParas[4] = wsSunPos.y;
	shaderParas[5] = wsSunPos.z;
	shaderParas[6] = wsSunPos.w;
	shaderParas[7] = zenith.x;
	shaderParas[8] = zenith.y;
	shaderParas[9] = zenith.z;

	return shaderParas;
}

float3 WeatherSystem::GetCloudBias()
{
	return cloudBias;
}
