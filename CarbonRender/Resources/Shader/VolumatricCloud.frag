#version 430
#define LIGHT_SAMPLE_COUNT 6
#define CLOUD_SAMPLE_COUNT 128
#define Pi 3.1415926

layout(location = 0) out vec4 cColor;

in vec4 wsP;

uniform sampler3D perlinWorleyMap;
uniform sampler3D worleyMap;
uniform sampler2D curlMap;
uniform sampler2D posMap;
uniform sampler2D weatherMap;
uniform sampler2D stencilMap;
uniform sampler2D shadowMap;

uniform vec3 wsCamPos;
uniform vec3 wsSunPos;

uniform vec3 sunColor;
uniform vec3 zenithColor;

uniform mat4 viewMat;
uniform mat4 proMat;
uniform mat4 smMat;

uniform vec3 cloudBias;
uniform float CoverageFactor;
uniform float PrecipitationFactor = 1.0f;

const float PLANET_RADIUS = 6356755.0f;
const vec3 PLANET_CENTER = vec3(0.0f, -PLANET_RADIUS, 0.0f);
const float MIN_CLOUD_ALTITUDE = 150.0f;
const float MAX_CLOUD_ALTITUDE = 600.0f;
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

const float MAX_FOG_ALTITUDE = 20.0f;

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

vec3 UVW(vec3 p, float scale)
{
	vec3 uvw;

	float a = 0.1f / (MAX_CLOUD_ALTITUDE - MIN_CLOUD_ALTITUDE);
	uvw.xz = p.xz * a * 0.5f + 0.5f;
	uvw.y = p.y * a * 0.5f + 0.5f;

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

vec3 CloudLighting(vec3 pos, float density, vec3 lightRay, float stepSize, vec3 viewRay, float mipmapLev)
{
	vec3 centerPos = pos;
	float precipitation = SampleWeather(centerPos).g;
	vec3 lightSamplePos;
	vec3 sampleStep = lightRay * stepSize;
	float h = GetHeightGradient(pos);
	vec3 sColor = mix(zenithColor, sunColor, h);
	h = mix(2.0f, 10.0f, h);
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
/*
vec4 GetFog (vec3 viewRay, vec3 lightRay, vec3 intersectPos, bool stensilMark)
{
	//Volumetric fog
	vec3 samplePos = wsCamPos;
	vec3 startPos = wsCamPos;
	vec3 endPos = intersectPos;

	float fogStepSize = 1.0f;
	float fogDensity = 0.1f;

	vec3 sampleStep = viewRay * fogStepSize;
	float fogMaxSampleDis;

	float sampleDisSum = 0.0f;
	float fogSampleDensity = 0.0f;
	vec3 fogLightColor = vec3(0.0f);

	float fogDensityStep = fogDensity * fogStepSize;
	int fogSampleCount = int(1.0f / fogDensity);
	fogSampleCount = min(128, fogSampleCount);	

	if (wsCamPos.y > MAX_FOG_ALTITUDE)
	{
		float d = (wsCamPos.y - MAX_FOG_ALTITUDE) / dot(viewRay, vec3(0.0f, -1.0f, 0.0f));
		startPos = wsCamPos + viewRay * d;
		sampleDisSum += d;
	}
	else
	{
		if (!stensilMark)
		{
			endPos = wsCamPos + viewRay * (MAX_FOG_ALTITUDE - wsCamPos.y) / dot(viewRay, vec3(0.0f, 1.0f, 0.0f));
		}
	}
	

	for (int i = 0; i < fogSampleCount; i++)
	{	
		if (samplePos.y > MAX_FOG_ALTITUDE || 
			fogSampleDensity > 1.0f)
			break;

		if (sampleDisSum >= fogMaxSampleDis)
			samplePos = objPos;

		float lightRayDensity = 0.0f;
		vec3 lightSamplePos = samplePos;
		vec3 lightSampleStep = lightRay * fogStepSize;
		for (int j = 0; j < 6; j++)
		{
			if (lightSamplePos.y > MAX_FOG_ALTITUDE)
				break;

			lightRayDensity += fogDensityStep;
			lightSamplePos += lightSampleStep;
		}

		vec3 fogSampleColor = sunColor;
		float vol = clamp(dot(viewRay, lightRay), 0.0f, 1.0f);
		fogSampleColor *= 2.0f * Beers(lightRayDensity, 5.0f);
		fogSampleColor *= Powder(fogDensityStep);
		fogLightColor += fogSampleColor;		

		if (sampleDisSum >= fogMaxSampleDis)
		{	
			fogSampleDensity += fogDensityStep * (fogStepSize - sampleDisSum + fogMaxSampleDis) / fogStepSize;
			break;
		}
		else
			fogSampleDensity += fogDensityStep;

		samplePos += sampleStep;
		sampleDisSum += fogStepSize;
	}
	
	cColor.a = min(fogSampleDensity, 1.0f);
	cColor.rgb = cColor.aaa;//fogLightColor;
	return vec4(0.0f);
}
*/
vec4 GetCloud(vec3 viewRay, vec3 lightRay, vec3 intersectPos, bool stensilMark)
{
	//Volumetric cloud
	float intersectDis = distance(wsCamPos, intersectPos);

	vec3 startPos;
	vec3 endPos;
	if (wsCamPos.y < MIN_CLOUD_ALTITUDE)
	{
		float L = distance(PLANET_CENTER, wsCamPos);
		float alpha = dot( normalize(PLANET_CENTER - wsCamPos), viewRay);
		float l1 = L * abs(alpha);
		float d = sqrt(L * L - l1 * l1);
		float r = PLANET_RADIUS + MIN_CLOUD_ALTITUDE;
		float l2 = sqrt(r * r - d * d);
		startPos = alpha >= 0.0f ? wsCamPos + viewRay * (l1 + l2) : wsCamPos + viewRay * (l2 - l1);

		r = PLANET_RADIUS + MAX_CLOUD_ALTITUDE;
		l2 = sqrt(r * r - d * d);
		endPos = alpha >= 0.0f ? wsCamPos + viewRay * (l1 + l2) : wsCamPos + viewRay * (l2 - l1);
	}
	else if (wsCamPos.y > MAX_CLOUD_ALTITUDE)
	{
		float L = wsCamPos.y + PLANET_RADIUS;
		float alpha = dot(vec3(0.0f, -1.0f, 0.0f), viewRay);
		if (alpha < 0.0) return vec4(0.0);
		float l1 = L * alpha;
		float d = sqrt(L * L - l1 * l1);
		float r = PLANET_RADIUS + MAX_CLOUD_ALTITUDE;
		float l2 = sqrt(r * r - d * d);
		startPos = wsCamPos + viewRay * (l1 - l2 + 1.0f);

		r = PLANET_RADIUS + MIN_CLOUD_ALTITUDE;
		l2 = sqrt(r * r - d * d);
		endPos = wsCamPos + viewRay * (l1 - l2);
	}
	else
	{
		startPos = wsCamPos;
		endPos = wsCamPos + viewRay * 1000.0f;
	}

	vec3 samplePos = startPos;
	float sampleDisSum = distance(startPos, wsCamPos);
	float sampleDis = distance(wsCamPos, endPos);
	float invSampleDis = 1.0f / sampleDis;
	float cloudStepSize = sampleDis / CLOUD_SAMPLE_COUNT;
	vec3 sampleStep = viewRay * cloudStepSize;
	vec4 cloudColor = vec4(0.0f);
	float mipmapLev = 0.0f;
	
	for(int i = 0; i < CLOUD_SAMPLE_COUNT; i++)
	{
		if (sampleDisSum >= intersectDis || 
			samplePos.y < -100.0f ||
			sampleDisSum > sampleDis
			) break;
			
		float sampleDensity = SampleCloudDensity(samplePos, mipmapLev, false);

		if (sampleDensity > 0.0f)
		{
			vec3 sampleColor = CloudLighting(samplePos, sampleDensity, lightRay, cloudStepSize, viewRay, mipmapLev);
			cloudColor.rgb += sampleColor;
			cloudColor.a += sampleDensity;

			if (cloudColor.a >= 1.0f) break;
		}

		samplePos += sampleStep;
		sampleDisSum += cloudStepSize;
	}

	return cloudColor;
}

void main ()
{
	cColor = vec4(0.0f);
	vec3 viewRay = normalize(wsP.xyz - wsCamPos);
	vec3 lightRay = normalize(wsSunPos);

	vec3 ssP = GetScreenPosAndDepth(wsP.xyz);
	vec3 intersectPos = texture2D(posMap, ssP.xy).xyz;
	bool stensilMark = texture2D(stencilMap, ssP.xy).r >= 1.0f;
	if (!stensilMark) intersectPos = wsCamPos + viewRay * 100000000.0f;
	
	//cColor = GetFog(viewRay, lightRay, intersectPos, stensilMark);
	if (cColor.a >= 1.0f)
		return;

	vec4 cloudColor = GetCloud(viewRay, lightRay, intersectPos, stensilMark);

	cColor.rgb += cloudColor.rgb * (1.0f - cColor.a);
	cColor.a += cloudColor.a;
	cColor = clamp(cColor, 0.0f, 1.0f);
}