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
	turbidity = 4.0f;
	exposure = 30.0f;
	timeSpeed = 150.0f;
	isTimeStop = true;
	lightR = 3000.0f;
	windDir = float4(0.5f, 0.0f, 1.0f, 10.0f);
	cloudBias = float3(0.0f);
	Light sun(LightType::eDirection, 1.0f);
	sun.SetNearClip(1.0f);
	sun.SetFarClip(lightR * 1.5f);
	sun.SetLightSize(0.009342f * lightR);
	sun.SetShadowMapSize(2048);
	sunLight = LightManager::Instance()->AddLight(sun);
	cloudCoverage = 0.55f;
	cloudPrecipitation = 1.0f;
	GenerateCloudNoise();
	fogDensity = 0.02f;

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

	Light* sun = LightManager::Instance()->GetLight(sunLight);
	sun->SetPosition(wsSunPos);
	sun->SetColor(GetSunColor());
	sun->LookAt(float3(0.0f));
	LightManager::Instance()->SetZenithColor(GetZenithColor());
}

void WeatherSystem::UpdateCloud()
{
	cloudBias = float3(windDir.x, windDir.y, windDir.z).normalize() * windDir.w * FIXEDUPDATE_TIME * timeSpeed * (isTimeStop ? 0.0f : 1.0f) + cloudBias;
}

void WeatherSystem::Update()
{
	hour += FIXEDUPDATE_TIME * 0.0002778f * timeSpeed * (isTimeStop ? 0.0f : 1.0f);
	if (hour >= 24.0f)
		hour = 0.0f;

	UpdateAtmosphere();
	UpdateCloud();
}

void WeatherSystem::SetLatitude(float l)
{
	latitude = l;
}

float WeatherSystem::GetLatitude()
{
	return latitude;
}

void WeatherSystem::SetDay(int d)
{
	day = d;
}

int WeatherSystem::GetDay()
{
	return day;
}

void WeatherSystem::SetHour(float h)
{
	hour = h;
}

float WeatherSystem::GetHour()
{
	return hour;
}

void WeatherSystem::SetTurbidity(float t)
{
	turbidity = t;
}

float WeatherSystem::GetTurbidity()
{
	return turbidity;
}

void WeatherSystem::SetExposure(float exp)
{
	exposure = exp;
}

float WeatherSystem::GetExposure()
{
	return exposure;
}

void WeatherSystem::SetTimeSpeed(float t)
{
	timeSpeed = t;
}

float WeatherSystem::GetTimeSpeed()
{
	return timeSpeed;
}

void WeatherSystem::SetWindDirection(float3 d)
{
	windDir.x = d.x;
	windDir.y = d.y;
	windDir.z = d.z;
}

float3 WeatherSystem::GetWindDirection()
{
	return windDir;
}

void WeatherSystem::SetWindStrength(float s)
{
	windDir.w = s;
}

float WeatherSystem::GetWindStrength()
{
	return windDir.w;
}

void WeatherSystem::SetCloudMaxAltitude(float a)
{
	cloudMaxAltitude = a;
}

float WeatherSystem::GetCloudMaxAltitude()
{
	return cloudMaxAltitude;
}

void WeatherSystem::SetCloudMinAltitude(float a)
{
	cloudMinAltitude = a;
}

float WeatherSystem::GetCloudMinAltitude()
{
	return cloudMinAltitude;
}

void WeatherSystem::SetCloudCoverage(float c)
{
	cloudCoverage = c;
	cloudCoverage = Math::Max(cloudCoverage, 0.0f);
	cloudCoverage = Math::Min(cloudCoverage, 1.0f);
}

void WeatherSystem::AddCloudCoverage(float c)
{
	cloudCoverage += c;
	cloudCoverage = Math::Max(cloudCoverage, 0.0f);
	cloudCoverage = Math::Min(cloudCoverage, 1.0f);
}

float WeatherSystem::GetCloudCoverage()
{
	return cloudCoverage;
}

void WeatherSystem::SetCloudPrecipitation(float p)
{
	cloudPrecipitation = p;
}

void WeatherSystem::AddCloudPrecipitation(float p)
{
	cloudPrecipitation += p;
}

float WeatherSystem::GetCloudPrecipitation()
{
	return cloudPrecipitation;
}

void WeatherSystem::SetWeatherMap(char* path)
{
	weatherMapPath = path;
	weatherMapId = TextureManager::Instance()->LoadTexture(path);
}

char * WeatherSystem::GetWeatherMapPath()
{
	return weatherMapPath;
}

GLuint WeatherSystem::GetWeatherMapId()
{
	return weatherMapId;
}

void WeatherSystem::GenerateCloudNoise()
{
	GLubyte* perlinWorley = new GLubyte[128 * 128 * 128 * 4];
	GLubyte* worley = new GLubyte[32 * 32 * 32 * 3];
	GLubyte* curl = new GLubyte[128 * 128 * 3];

	bool fileExist = true;
	fstream pwNoiseFile;
	pwNoiseFile.open("Resources\\Textures\\PerlinWorley.noise", ios::in | ios::binary);
	if (!pwNoiseFile)
	{
		fileExist = false;
		pwNoiseFile.open("Resources\\Textures\\PerlinWorley.noise", ios::out | ios::binary);
	}
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 128; j++)
			for (int k = 0; k < 128; k++)
			{
				float3 uv = float3(i * 0.0078125f, j * 0.0078125f, k * 0.0078125f);
				if (fileExist)
				{
					float noise;
					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 0] = (GLubyte)noise;

					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 1] = (GLubyte)noise;

					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 2] = (GLubyte)noise;

					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 3] = (GLubyte)noise;
				}
				else
				{
					float cellCount = 8.0f;
					float worleyNoise0 = Noise::WorleyNoise(uv, cellCount * 2.0f);
					float worleyNoise1 = Noise::WorleyNoise(uv, cellCount * 4.0f);
					float worleyNoise2 = Noise::WorleyNoise(uv, cellCount * 8.0f);
					float worleyNoise3 = Noise::WorleyNoise(uv, cellCount * 14.0f);
					float worleyNoise4 = Noise::WorleyNoise(uv, cellCount * 16.0f);
					float worleyNoise5 = Noise::WorleyNoise(uv, cellCount * 32.0f);

					float worleyFbm = worleyNoise0 * 0.625f + worleyNoise2 * 0.25f + worleyNoise3 * 0.125f;
					float noise = Math::Remap(Noise::PerlinFbm(uv, 8.0f, 3), 0.0f, 1.0f, worleyFbm, 1.0f) * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 0] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise0 * 0.625f + worleyNoise1 * 0.25f + worleyNoise2 * 0.125f;
					noise = worleyFbm * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 1] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise1 * 0.625f + worleyNoise2 * 0.25f + worleyNoise3 * 0.125f;
					noise = worleyFbm * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 2] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise2 * 0.625f + worleyNoise4 * 0.25f + worleyNoise5 * 0.125f;
					noise = worleyFbm * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 3] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));
				}
			}
	pwNoiseFile.close();

	fileExist = true;
	fstream wNoiseFile;
	wNoiseFile.open("Resources\\Textures\\Worley.noise", ios::in | ios::binary);
	if (!wNoiseFile)
	{
		fileExist = false;
		wNoiseFile.open("Resources\\Textures\\Worley.noise", ios::out | ios::binary);
	}
	for (int i = 0; i < 32; i++)
		for (int j = 0; j < 32; j++)
			for (int k = 0; k < 32; k++)
			{
				float3 uv = float3(i * 0.03125f, j * 0.03125f, k * 0.03125f);
				if (fileExist)
				{
					float noise;
					wNoiseFile.read((char*)&noise, sizeof(float));
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 0] = (GLubyte)noise;

					wNoiseFile.read((char*)&noise, sizeof(float));
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 1] = (GLubyte)noise;

					wNoiseFile.read((char*)&noise, sizeof(float));
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 2] = (GLubyte)noise;
				}
				else
				{
					float cellCount = 2.0f;
					float worleyNoise0 = Noise::WorleyNoise(uv, cellCount * 1.0f);
					float worleyNoise1 = Noise::WorleyNoise(uv, cellCount * 2.0f);
					float worleyNoise2 = Noise::WorleyNoise(uv, cellCount * 4.0f);
					float worleyNoise3 = Noise::WorleyNoise(uv, cellCount * 8.0f);
					float worleyNoise4 = Noise::WorleyNoise(uv, cellCount * 16.0f);

					float worleyFbm = worleyNoise0 * 0.625f + worleyNoise1 * 0.25f + worleyNoise2 * 0.125f;
					float noise = worleyFbm * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 0] = (GLubyte)noise;
					wNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise1 * 0.625f + worleyNoise2 * 0.25f + worleyNoise3 * 0.125f;
					noise = worleyFbm * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 1] = (GLubyte)noise;
					wNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise2 * 0.625f + worleyNoise3 * 0.25f + worleyNoise4 * 0.125f;
					noise = worleyFbm * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 2] = (GLubyte)noise;
					wNoiseFile.write((char*)&noise, sizeof(float));
				}
			}
	wNoiseFile.close();

	glGenTextures(3, cloudNoises);
	glBindTexture(GL_TEXTURE_3D, cloudNoises[0]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, 128, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, perlinWorley);
	glGenerateMipmap(GL_TEXTURE_3D);

	glBindTexture(GL_TEXTURE_3D, cloudNoises[1]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, 32, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, worley);
	glGenerateMipmap(GL_TEXTURE_3D);

	cloudNoises[2] = TextureManager::Instance()->LoadTexture("CurlNoise");
}

GLuint WeatherSystem::GetBaseNoise()
{
	return cloudNoises[0];
}

GLuint WeatherSystem::GetDetailNoise()
{
	return cloudNoises[1];
}

GLuint WeatherSystem::GetCurlNoise()
{
	return cloudNoises[2];
}

void WeatherSystem::SetFogDensity(float d)
{
	fogDensity = d;
}

void WeatherSystem::AddFogDensity(float d)
{
	fogDensity += d;
}

float WeatherSystem::GetFogDensity()
{
	return fogDensity;
}

void WeatherSystem::SetFogColor(float3 c)
{
	fogColor = c;
}

float3 WeatherSystem::GetFogColor()
{
	return fogColor;
}

void WeatherSystem::SetFogMaxAltitude(float a)
{
	fogMaxAltitude = a;
}

void WeatherSystem::AddFogMaxAltitude(float a)
{
	fogMaxAltitude += a;
}

float WeatherSystem::GetFogMaxAltitude()
{
	return fogMaxAltitude;
}

void WeatherSystem::SetFogPrecipitation(float p)
{
	fogPrecipitation = p;
}

void WeatherSystem::AddFogPrecipitation(float p)
{
	fogPrecipitation += p;
}

float WeatherSystem::GetFogPrecipitation()
{
	return fogPrecipitation;
}

void WeatherSystem::ToggleTimeLapse()
{
	isTimeStop = !isTimeStop;
}

void WeatherSystem::SetTimeStop(bool bStop)
{
	isTimeStop = bStop;
}

bool WeatherSystem::IsTimeStop()
{
	return isTimeStop;
}

float4 WeatherSystem::GetSunColor()
{
	if (wsSunPos.y <= 0.0f)
		return float4(0.002f, 0.002f, 0.002f, 0.002f);

	float3 up(0.0f, 1.0f, 0.0f);

	float3 vP = wsSunPos.normalize();
	float cosTheta = Math::Dot(vP, up);

	float4 skyColor(0.0f, 0.0f, 0.0f, 1.0f);
	float3 A(-0.0193f*turbidity - 0.2592f, -0.0167f*turbidity - 0.2608f, 0.1787f*turbidity - 1.4630f);
	float3 B(-0.0665f*turbidity + 0.0008f, -0.0950f*turbidity + 0.0092f, -0.3554f*turbidity + 0.4275f);
	float3 C(-0.0004f*turbidity + 0.2125f, -0.0079f*turbidity + 0.2102f, -0.0227f*turbidity + 5.3251f);
	float3 D(-0.0641f*turbidity - 0.8989f, -0.0441f*turbidity - 1.6537f, 0.1206f*turbidity - 2.5771f);
	float3 E(-0.0033f*turbidity + 0.0452f, -0.0109f*turbidity + 0.0529f, -0.0670f*turbidity + 0.3703f);

	float3 F1 = (1.0f + A*Math::Exp(B / cosTheta))*(1.0f + C + E);
	float3 F2 = (1.0f + A*Math::Exp(B))*(1.0f + C*Math::Exp(D*thetaS) + E*pow(cos(thetaS), 2.0f));
	float3 xyY = zenith*(F1 / F2);
	xyY.z = 1.0f - exp((-1.0f/exposure)*xyY.z);

	skyColor = Math::xyY2RGB(xyY);

	return skyColor;
}

float4 WeatherSystem::GetZenithColor()
{
	if (wsSunPos.y <= 0.0f)
		return float4(0.002f, 0.002f, 0.002f, 0.002f);

	float3 vP(0.0f, 1.0f, 0.0f);
	float3 vS = wsSunPos.normalize();
	float vSovP = Math::Dot(vS, vP);
	float gamma = acos(vSovP);

	float4 zenithColor(0.0f, 0.0f, 0.0f, 1.0f);
	float3 A(-0.0193f*turbidity - 0.2592f, -0.0167f*turbidity - 0.2608f, 0.1787f*turbidity - 1.4630f);
	float3 B(-0.0665f*turbidity + 0.0008f, -0.0950f*turbidity + 0.0092f, -0.3554f*turbidity + 0.4275f);
	float3 C(-0.0004f*turbidity + 0.2125f, -0.0079f*turbidity + 0.2102f, -0.0227f*turbidity + 5.3251f);
	float3 D(-0.0641f*turbidity - 0.8989f, -0.0441f*turbidity - 1.6537f, 0.1206f*turbidity - 2.5771f);
	float3 E(-0.0033f*turbidity + 0.0452f, -0.0109f*turbidity + 0.0529f, -0.0670f*turbidity + 0.3703f);

	float3 F1 = (1.0f + A*Math::Exp(B))*(1.0f + C*Math::Exp(D*gamma) + E*pow(vSovP, 2.0f));
	float3 F2 = (1.0f + A*Math::Exp(B))*(1.0f + C*Math::Exp(D*thetaS) + E*pow(cos(thetaS), 2.0f));
	float3 xyY = zenith*(F1 / F2);
	xyY.z = 1.0f - exp((-1.0f / exposure)*xyY.z);

	zenithColor = Math::xyY2RGB(xyY);

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
