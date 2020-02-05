#version 430
#define LIGHT_SAMPLE_COUNT 6
#define CLOUD_SAMPLE_COUNT 128
#define Pi 3.1415926

layout(location = 0) out vec4 cColor;

in vec4 wsP;

uniform sampler3D perlinWorleyMap;
uniform sampler3D worleyMap;
uniform sampler2D curlMap;
uniform sampler2D depthMap;
uniform sampler2D weatherMap;

uniform vec3 wsCamPos;
uniform vec3 wsSunPos;

uniform vec3 sunColor;
uniform vec3 zenithColor;

uniform mat4 viewMat;
uniform mat4 proMat;

uniform vec3 cloudBias;
uniform float CoverageFactor;
uniform float PrecipitationFactor = 1.0f;

const float PLANET_RADIUS = 6500000.0f;
const vec3 PLANET_CENTER = vec3(0.0f, -PLANET_RADIUS, 0.0f);
const float MIN_CLOUD_ALTITUDE = 1500.0f;
const float MAX_CLOUD_ALTITUDE = 6000.0f;
const float CLOUD_HEIGHT = MAX_CLOUD_ALTITUDE - MIN_CLOUD_ALTITUDE;
const float CLOUD_HEIGHT_INV = 1.0f / CLOUD_HEIGHT;

const vec3 randomV[6] = vec3[6](vec3(0.38051305f,  0.92453449f, -0.02111345f),
			                     vec3(-0.50625799f, -0.03590792f, -0.86163418f),
								 vec3(-0.32509218f, -0.94557439f,  0.01428793f),
								 vec3(0.09026238f, -0.27376545f,  0.95755165f),
								 vec3(0.28128598f,  0.42443639f, -0.86065785f),
								 vec3(-0.16852403f,  0.14748697f,  0.97460106f));

const vec4 StratusFactor = vec4(0.0f, 0.1f, 0.2f, 0.3f);
const vec4 CumulusFactor = vec4(0.05f, 0.25f, 0.45f, 0.65f);
const vec4 CumulonumbisFactor = vec4(0.0f, 0.05f, 0.7f, 0.9f);

const float MAX_FOG_ALTITUDE = 10.0f;

float Remap(float value, float oldMin, float oldMax, float newMin, float newMax)
{
	return newMin + (value - oldMin) * (newMax - newMin) / (oldMax - oldMin);
}

float GetHeightGradient(vec3 pos)
{
	float d = distance(pos, PLANET_CENTER);
	d = (d - MIN_CLOUD_ALTITUDE - PLANET_RADIUS) * CLOUD_HEIGHT_INV;
	d = clamp(d, 0.0f, 1.0f);

	return d;
}

float GetCloudTypeGradient(float heightGrad, vec4 typePara)
{
	float a = Remap(heightGrad, typePara.x, typePara.y, 0.0f, 1.0f) * Remap(heightGrad, typePara.z, typePara.w, 1.0f, 0.0f);
	a = clamp(a, 0.0f, 1.0f);

	return a;	
}

bool IsInCloud(vec3 pos)
{
	float a = distance(pos, PLANET_CENTER) - PLANET_RADIUS;
	return a >= MIN_CLOUD_ALTITUDE && a <= MAX_CLOUD_ALTITUDE;
}

vec3 UVW(vec3 p, float scale)
{
	vec3 uvw = p * 0.00002f * 0.5f + 0.5f;
	uvw *= scale;

	return uvw;
}

float Beers(float d, float p)
{
	return exp(-d * p);
}

float Powder(float d)
{
	return 1.0f - exp(-d * 2.0f);
}

float HG(float vol, float g)
{
	return (1.0f - g*g) / (4.0f * Pi * pow(1.0f + g*g - 2.0f * g * vol, 1.5f));
}

float GetDepth(vec3 pos)
{
	vec4 csP = viewMat * vec4(pos, 1.0f);
	return -csP.z;
}

vec3 GetScreenPosAndDepth(vec3 pos)
{
	vec4 ssP = viewMat * vec4(pos, 1.0f);
	float d = -ssP.z;
	ssP = proMat * ssP;
	ssP.xy = ssP.xy / ssP.w;
	ssP.xy = ssP.xy * 0.5f + 0.5f;

	return vec3(ssP.xy, d);
}

vec3 SampleWeather(vec3 pos)
{
	vec3 weatherData = texture2D(weatherMap, UVW(pos + cloudBias * 2.0f, 1.0f).xz).rgb;//R - coverage; G - precipitation; B - cloud type
	weatherData.r = weatherData.r * CoverageFactor;
	weatherData.g = mix(1.0f, 2.0f, weatherData.g); 
	weatherData.g *= PrecipitationFactor;
	weatherData.b = weatherData.b;

	return weatherData;
}

float SampleCloudDensity(vec3 pos, float mipmap, bool useCheapWay)
{
	vec3 samplePos = pos;
	vec3 weather = SampleWeather(samplePos);
	float heightF = GetHeightGradient(samplePos);
	vec3 uvw = UVW(samplePos + cloudBias, 8.0f);
	vec4 noise = textureLod(perlinWorleyMap, uvw, mipmap);
	float lfFbm = noise.g * 0.625f + noise.b * 0.25f + noise.a * 0.125f;
	lfFbm = Remap(noise.r, lfFbm - 1.0f, 1.0f, 0.0f, 1.0f);
	lfFbm = clamp(lfFbm, 0.0f, 1.0f);

	vec4 typeFactor = StratusFactor * (1.0f - clamp(weather.b * 2.0f, 0.0f, 1.0f));
	typeFactor += CumulusFactor * (1.0f - abs(weather.b - 0.5f) * 2.0f);
	typeFactor += CumulonumbisFactor * clamp(weather.b - 0.5f, 0.0f, 1.0f) * 2.0f;
	float typeGrad = GetCloudTypeGradient(heightF, typeFactor);
	lfFbm *= typeGrad;

	float coverage = weather.r;
	lfFbm = Remap(lfFbm, 1.0f - coverage, 1.0f, 0.0f, 1.0f);
	lfFbm *= coverage;

	if (!useCheapWay)
	{
		//do curl
		vec4 curlNoise = texture2D(curlMap, uvw.xz);
		uvw.xz += curlNoise.xz * clamp(heightF * 5.0f, 0.0f, 1.0f);

		noise = textureLod(worleyMap, uvw * 0.6f, mipmap);
		float hfFbm = noise.r * 0.625f + noise.g * 0.25f + noise.b * 0.125f;
		hfFbm = 1.0f - hfFbm;
		hfFbm *= clamp((heightF - typeFactor.x) / (typeFactor.w - typeFactor.x) * 10.0f * 0.12f, 0.0f, 1.0f);
		lfFbm = Remap(lfFbm, hfFbm * 0.5f, 1.0f, 0.0f, 1.0f);
	}

	return clamp(lfFbm, 0.0f, 1.0f);
}

vec3 ConeTracingLight(vec3 pos, float density, vec3 lightRay, float stepSize, vec3 viewRay, float mipmapLev)
{
	vec3 centerPos = pos;
	float precipitation = SampleWeather(centerPos).g;
	vec3 lightSamplePos;
	vec3 sampleStep = lightRay * stepSize;
	float h = GetHeightGradient(pos);
	vec3 sColor = mix(zenithColor, sunColor, h);
	h = mix(1.0f, 10.0f, h);
	float accumDensity = 0.0f;
	float lightRayDensity = 0.0f;
	float invSumLength = 1.0f / (stepSize * LIGHT_SAMPLE_COUNT);
	float b = 1.0f;
	
	for (int j = 0; j < LIGHT_SAMPLE_COUNT; j++)
	{
		vec3 bias = randomV[j];
		bias = normalize(bias);
		lightSamplePos = centerPos + bias * (j + 1);
		
		float sampleDensity = SampleCloudDensity(lightSamplePos, mipmapLev, false);

		if (sampleDensity > 0.0f)
			lightRayDensity += sampleDensity;

		centerPos += sampleStep;
	}

	lightSamplePos = pos + sampleStep * 8.0f;
	float sampleDensity = SampleCloudDensity(lightSamplePos, mipmapLev, false);
	if (sampleDensity > 0.0f)
		lightRayDensity += sampleDensity;

	float vol = clamp(dot(viewRay, lightRay), 0.0f, 1.0f);
	sColor *= 2.0f * Beers(lightRayDensity, precipitation);
	sColor *= Powder(density);
	sColor *= mix(HG(vol, 0.7f), HG(vol, -0.1f), 0.5f);
	return sColor * h;
}

void main ()
{
	vec3 viewRay = normalize(wsP.xyz - wsCamPos);
	vec3 lightRay = normalize(wsSunPos);

	vec3 ssP = GetScreenPosAndDepth(wsP.xyz);
	vec4 normalAndDepth = texture2D(depthMap, ssP.xy);
	bool isObj = distance(normalAndDepth.xyz, vec3(0.0f)) > 0.0f;

	//Volumetric cloud
	vec3 vL = normalize(PLANET_CENTER - wsCamPos);
	float L = distance(PLANET_CENTER, wsCamPos);
	float alpha = dot(vL, viewRay);
	float l1 = L * abs(alpha);
	float d = sqrt(L * L - l1 * l1);
	float r = PLANET_RADIUS + MIN_CLOUD_ALTITUDE;
	float l2 = sqrt(r * r - d * d);
	vec3 nearPos = alpha >= 0.0f ? wsCamPos + viewRay * (l1 + l2) : wsCamPos + viewRay * (l2 - l1);

	r = PLANET_RADIUS + MAX_CLOUD_ALTITUDE;
	l2 = sqrt(r * r - d * d);
	vec3 farPos = alpha >= 0.0f ? wsCamPos + viewRay * (l1 + l2) : wsCamPos + viewRay * (l2 - l1);

	vec3 samplePos = nearPos;
	float sampleDis = distance(samplePos, farPos);
	float invSampleDis = 1.0f / sampleDis;
	float stepSize = sampleDis / CLOUD_SAMPLE_COUNT;

	if (IsInCloud(wsCamPos))
	{
		samplePos = wsCamPos.xyz;
		stepSize = min(128.0f, stepSize);
	}
	
	vec3 sampleStep = viewRay * stepSize;
	vec4 cloudColor = vec4(0.0f);
	float cloudDensity = 0.0f;
	float mipmapLev = 0.0f;
	
	for(int i = 0; i < CLOUD_SAMPLE_COUNT; i++)
	{	
		if ((GetDepth(samplePos) >= normalAndDepth.w && isObj)
			|| samplePos.y < -100.0f
			|| (distance(samplePos, PLANET_CENTER) - PLANET_RADIUS) > MAX_CLOUD_ALTITUDE
			) break;
			
		float sampleDensity = SampleCloudDensity(samplePos, mipmapLev, false);

		if (sampleDensity > 0.0f)
		{
			vec3 sampleColor = ConeTracingLight(samplePos, sampleDensity, lightRay, stepSize, viewRay, mipmapLev);
			cloudColor.rgb += sampleColor;
			cloudColor.a += sampleDensity;

			if (cloudColor.a >= 1.0f) break;
		}

		samplePos += sampleStep;	
	}

	cColor = cloudColor;
	cColor = clamp(cColor, 0.0f, 1.0f);
}