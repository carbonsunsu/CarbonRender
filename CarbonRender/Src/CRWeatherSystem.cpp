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
	latitude = 45.0f;
	day = 200;
	hour = 5.0f;
	turbidity = 2.0f;
	exposure = 50.0f;
	timeSpeed = 4000.0f;
	lightR = 1000.0f;

	Update();
}

void WeatherSystem::Update()
{
	hour += FIXEDUPDATE_TIME * 0.0002778f * timeSpeed;
	if (hour >= 24.0f)
		hour = 0.0f;

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
	Yz = (4.0453f * turbidity - 4.9710f) * tan(X) - 0.2155f * turbidity + 2.4192f;
	Yz = abs(Yz);

	float ThetaS_3 = pow(thetaS, 3.0f);
	float ThetaS_2 = pow(thetaS, 2.0f);
	float T_2 = turbidity * turbidity;
	float4 temp(T_2 * 0.00166f - turbidity * 0.02903f + 0.11693f,
				-T_2 * 0.00375f + turbidity * 0.06377f - 0.21196f,
				T_2 * 0.00209f - turbidity * 0.03202f + 0.06052f,
				turbidity * 0.00394f + 0.25886f);
	xz = ThetaS_3 * temp.x + ThetaS_2 * temp.y + thetaS * temp.z + temp.w;

	temp = float4(T_2 * 0.00275f - turbidity * 0.04214f + 0.15346f,
				-T_2 * 0.00610f + turbidity * 0.08970f - 0.26756f,
				T_2 * 0.00317f - turbidity * 0.04156f + 0.06670f,
				turbidity * 0.00516f + 0.26688f);
	yz = ThetaS_3 * temp.x + ThetaS_2 * temp.y + thetaS * temp.z + temp.w;
}

float4 WeatherSystem::GetWsSunPos()
{
	return wsSunPos;
}

float4 WeatherSystem::GetSunColor()
{
	float3 up(0.0f, 1.0f, 0.0f);

	float3 vP = wsSunPos.normalize();
	vP.y = abs(vP.y);
	float cosTheta = Dot(vP, up);
	float3 vS = wsSunPos.normalize();
	float vSovP = Dot(vS, vP);
	float gamma = acos(vSovP);

	float4 skyColor(0.0f, 0.0f, 0.0f, 1.0f);
	//Y
	float A = 0.1787f*turbidity - 1.4630f;
	float B = -0.3554f*turbidity + 0.4275f;
	float C = -0.0227f*turbidity + 5.3251f;
	float D = 0.1206f*turbidity - 2.5771f;
	float E = -0.0670f*turbidity + 0.3703f;

	float F1 = (1.0f + A*exp(B / cosTheta))*(1.0f + C*exp(D*gamma) + E*pow(vSovP, 2.0f));
	float F2 = (1.0f + A*exp(B))*(1.0f + C*exp(D*thetaS) + E*pow(cos(thetaS), 2.0f));
	float Y = Yz*(F1 / F2);
	Y = 1.0f - exp((-1.0f/exposure)*Y);

	//x
	A = -0.0193f*turbidity - 0.2592f;
	B = -0.0665f*turbidity + 0.0008f;
	C = -0.0004f*turbidity + 0.2125f;
	D = -0.0641f*turbidity - 0.8989f;
	E = -0.0033f*turbidity + 0.0452f;

	F1 = (1.0f + A*exp(B / cosTheta))*(1.0f + C*exp(D*gamma) + E*pow(vSovP, 2.0f));
	F2 = (1.0f + A*exp(B))*(1.0f + C*exp(D*thetaS) + E*pow(cos(thetaS), 2.0f));
	float x = xz*(F1 / F2);

	//y
	A = -0.0167f*turbidity - 0.2608f;
	B = -0.0950f*turbidity + 0.0092f;
	C = -0.0079f*turbidity + 0.2102f;
	D = -0.0441f*turbidity - 1.6537f;
	E = -0.0109f*turbidity + 0.0529f;

	F1 = (1.0f + A*exp(B / cosTheta))*(1.0f + C*exp(D*gamma) + E*pow(vSovP, 2.0f));
	F2 = (1.0f + A*exp(B))*(1.0f + C*exp(D*thetaS) + E*pow(cos(thetaS), 2.0f));
	float y = yz*(F1 / F2);

	skyColor = Yxy2RGB(Y, x, y);

	return float4();
}

float4 WeatherSystem::GetSkyUpColor()
{
	return Yxy2RGB(Yz, xz, yz);
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
	shaderParas[7] = Yz;
	shaderParas[8] = yz;
	shaderParas[9] = xz;

	return shaderParas;
}
