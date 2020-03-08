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
uniform sampler2D smMapLv0;
uniform sampler2D smMapLv1;

uniform vec3 wsCamPos;
uniform vec3 wsSunPos;

uniform vec3 sunColor;
uniform vec3 zenithColor;

uniform mat4 viewMat;
uniform mat4 proMat;
uniform mat4 smMat;
uniform mat4 smProMatLv0;
uniform mat4 smProMatLv1;

uniform float cloudMaxAltitude;
uniform float cloudMinAltitude;
uniform float cloudHeightInv;
uniform vec3 cloudBias;
uniform float cloudCoverageScale;
uniform float cloudPrecipitationScale;

uniform float fogDensity;
uniform vec3 fogColorScale;
uniform float fogPrecipitaion;
uniform float fogMaxAltitude;

const float PLANET_RADIUS = 6356755.0f;
const vec3 PLANET_CENTER = vec3(0.0f, -PLANET_RADIUS, 0.0f);

const vec3 randomV[6] = vec3[6](vec3(0.38051305f,  0.92453449f, -0.02111345f),
			                     vec3(-0.50625799f, -0.03590792f, -0.86163418f),
								 vec3(-0.32509218f, -0.94557439f,  0.01428793f),
								 vec3(0.09026238f, -0.27376545f,  0.95755165f),
								 vec3(0.28128598f,  0.42443639f, -0.86065785f),
								 vec3(-0.16852403f,  0.14748697f,  0.97460106f));

const vec4 StratusFactor = vec4(0.0f, 0.1f, 0.2f, 0.3f);
const vec4 CumulusFactor = vec4(0.05f, 0.25f, 0.45f, 0.65f);
const vec4 CumulonumbisFactor = vec4(0.0f, 0.05f, 0.7f, 0.9f);

const float FOG_STEP_SIZE_SHORT = 0.1f;
const float FOG_STEP_SIZE_LONG = 2.0f;

float Remap(float value, float oldMin, float oldMax, float newMin, float newMax)
{
	return newMin + (value - oldMin) * (newMax - newMin) / (oldMax - oldMin);
}

float GetHeightGradient(vec3 pos)
{
	float d = distance(pos, PLANET_CENTER);
	d = (d - cloudMinAltitude - PLANET_RADIUS) * cloudHeightInv;
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

	float a = 0.1f / (cloudMaxAltitude - cloudMinAltitude);
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
	weatherData.r = weatherData.r * cloudCoverageScale;
	weatherData.g = mix(1.0f, 2.0f, weatherData.g); 
	weatherData.g *= cloudPrecipitationScale;
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

bool IsInScreen (vec2 ssPos)
{
	return ssPos.x >= 0.0f && ssPos.x <= 1.0f && ssPos.y >= 0.0f && ssPos.y <= 1.0f;
}

vec2 GetShadowUV (vec4 pos, out int level)
{
	level = 0;
	vec4 smPos = smProMatLv0 * pos;
	vec2 smUV = smPos.xy / smPos.w * 0.5f + 0.5f;
	
	if (!IsInScreen(smUV))
	{
		level = 1;
		smPos = smProMatLv1 * pos;
		smUV = smPos.xy / smPos.w * 0.5f + 0.5f;
		
		if (!IsInScreen(smUV))
			level = -1;
	}

	return smUV;
}

vec4 GetShadowMap(int level, vec2 uv)
{
	if (level == 0)
		return texture2D(smMapLv0, uv);
	else
		return texture2D(smMapLv1, uv);
}

vec4 GetFog (vec3 viewRay, vec3 lightRay, vec3 intersectPos, float intersectDis, bool stensilMark, out float startToCamDis)
{
	//Volumetric fog
	if (fogDensity <= 0.0f)
		return vec4(0.0f);

	vec3 startPos;
	vec3 endPos;

	float sampleStep = FOG_STEP_SIZE_SHORT;
	vec3 sampleStepV = viewRay * sampleStep;
	float fogDensityStep = fogDensity * sampleStep;
	int fogSampleCount = 512;
	int fogSampleCountAccur = 256;
	int fogSampleCountUnaccur = fogSampleCount - fogSampleCountAccur;

	float sampleDisSum;
	float sampleDisMax;
	
	if (wsCamPos.y > fogMaxAltitude)
	{
		float theta = dot(viewRay, vec3(0.0f, -1.0f, 0.0f));

		if (theta > 0)
		{
			sampleDisSum = (wsCamPos.y - fogMaxAltitude) / theta;
			sampleDisSum += 0.1f;
			startPos = wsCamPos + viewRay * sampleDisSum;

			sampleDisMax = sampleDisSum + fogSampleCountAccur * FOG_STEP_SIZE_SHORT + fogSampleCountUnaccur * FOG_STEP_SIZE_LONG;
			endPos = wsCamPos + viewRay * sampleDisMax;
		}
		else
		{
			startToCamDis = 1000000000.0f;
			return vec4(0.0f);
		}
	}
	else
	{
		sampleDisSum = 0.0f;
		startPos = wsCamPos;

		float theta = dot(viewRay, vec3(0.0f, 1.0f, 0.0f));

		if (theta > 0.0f)
			sampleDisMax = (fogMaxAltitude - wsCamPos.y) / theta;
		else
			sampleDisMax = sampleDisSum + fogSampleCountAccur * FOG_STEP_SIZE_SHORT + fogSampleCountUnaccur * FOG_STEP_SIZE_LONG;

		endPos = wsCamPos + viewRay * sampleDisMax;
		
	}
	startToCamDis = sampleDisSum;

	if (intersectDis <= sampleDisMax)
	{
		endPos = intersectPos;
		sampleDisMax = intersectDis;
	}

	float LoU = dot(lightRay, vec3(0.0f, 1.0f, 0.0f));
	float fogSampleDensity = 0.0f;
	vec3 fogLightColor = vec3(0.0f);
	vec3 samplePos = startPos;

	for (int i = 0; i < fogSampleCount; i++)
	{
		if (sampleDisSum >= sampleDisMax)
			samplePos = endPos;

		if (samplePos.y > fogMaxAltitude) break;

		vec3 fogSampleColor = vec3(0.0f);

		vec4 smsPos = smMat * vec4(samplePos, 1.0f);
		float sampleDepth = -smsPos.z;
		int shadowLevel;
		vec2 smUV = GetShadowUV(smsPos, shadowLevel);
		float blkerDepth = GetShadowMap(shadowLevel, smUV).r;
		if (shadowLevel < 0 || sampleDepth <= blkerDepth)
		{
			float lightRayDensity = fogDensity * max(fogMaxAltitude - samplePos.y, 0.0f) / LoU;
			//lightRayDensity = min(1.0f, lightRayDensity);

			fogSampleColor = sunColor * fogColorScale;
			fogSampleColor *= Beers(lightRayDensity, fogPrecipitaion);
			fogSampleColor *= Beers(fogSampleDensity, fogPrecipitaion);
		}

		fogLightColor += fogSampleColor;

		if (i == fogSampleCountAccur)
		{
			sampleStep = FOG_STEP_SIZE_LONG;
			sampleStepV = viewRay * sampleStep;
			fogDensityStep = fogDensity * sampleStep;
		}

		//vec3 uvw = UVW(samplePos + cloudBias, 8.0f);
		float noise = 1.0f;//textureLod(perlinWorleyMap, uvw, 0.0f).g;
		if (sampleDisSum >= sampleDisMax)
		{	
			fogSampleDensity += noise * fogDensityStep * (sampleStep - sampleDisSum + sampleDisMax) / sampleStep;
			break;
		}
		fogSampleDensity += noise * fogDensityStep;

		if (fogSampleDensity >= 1.0f)
			break;

		samplePos += sampleStepV;
		sampleDisSum += sampleStep;
	}
	
	vec4 fogColor;
	fogColor.a = fogSampleDensity;
	fogColor.rgb = fogLightColor;
	return clamp(fogColor, 0.0f, 1.0f);
}

vec4 GetCloud(vec3 viewRay, vec3 lightRay, vec3 intersectPos, float intersectDis, bool stensilMark, out float startToCamDis)
{
	//Volumetric cloud
	vec3 startPos;
	vec3 endPos;
	float sampleDisSum;
	float sampleDisMax;

	if (wsCamPos.y < cloudMinAltitude)
	{
		float L = distance(PLANET_CENTER, wsCamPos);
		float alpha = dot( normalize(PLANET_CENTER - wsCamPos), viewRay);
		float l1 = L * abs(alpha);
		float d = sqrt(L * L - l1 * l1);
		float r = PLANET_RADIUS + cloudMinAltitude;
		float l2 = sqrt(r * r - d * d);
		sampleDisSum = alpha >= 0.0f ? (l1 + l2) : (l2 - l1);
		startPos = wsCamPos + viewRay * sampleDisSum;

		r = PLANET_RADIUS + cloudMaxAltitude;
		l2 = sqrt(r * r - d * d);
		sampleDisMax = alpha >= 0.0f ? (l1 + l2) : (l2 - l1);
		endPos = wsCamPos + viewRay * sampleDisMax;
	}
	else if (wsCamPos.y > cloudMaxAltitude)
	{
		float L = wsCamPos.y + PLANET_RADIUS;
		float alpha = dot(vec3(0.0f, -1.0f, 0.0f), viewRay);
		if (alpha < 0.0) return vec4(0.0);
		float l1 = L * alpha;
		float d = sqrt(L * L - l1 * l1);
		float r = PLANET_RADIUS + cloudMaxAltitude;
		float l2 = sqrt(r * r - d * d);
		sampleDisSum = l1 - l2 + 1.0f;
		startPos = wsCamPos + viewRay * sampleDisSum;

		r = PLANET_RADIUS + cloudMinAltitude;
		l2 = sqrt(r * r - d * d);
		sampleDisMax = l1 - l2;
		endPos = wsCamPos + viewRay * sampleDisMax;
	}
	else
	{
		sampleDisSum = 0.0f;
		sampleDisMax = 1000.0f;
		startPos = wsCamPos;
		endPos = wsCamPos + viewRay * sampleDisMax;
	}
	startToCamDis = sampleDisSum;

	vec3 samplePos = startPos;
	float cloudStepSize = sampleDisMax / CLOUD_SAMPLE_COUNT;
	vec3 sampleStep = viewRay * cloudStepSize;
	vec4 cloudColor = vec4(0.0f);
	float mipmapLev = 0.0f;

	if (intersectDis <= sampleDisMax)
	{
		sampleDisMax = intersectDis;
		endPos = intersectPos;
	}
	
	for(int i = 0; i < CLOUD_SAMPLE_COUNT; i++)
	{
		if (samplePos.y < -100.0f //||
			) break;

		if (sampleDisSum >= sampleDisMax)
			samplePos = endPos;
			
		float sampleDensity = SampleCloudDensity(samplePos, mipmapLev, false);

		if (sampleDensity > 0.0f)
		{
			vec3 sampleColor = CloudLighting(samplePos, sampleDensity, lightRay, cloudStepSize, viewRay, mipmapLev);
			cloudColor.rgb += sampleColor;
			cloudColor.a += sampleDensity;

			if (cloudColor.a >= 1.0f) break;
		}

		if (sampleDisSum >= sampleDisMax)
			break;

		samplePos += sampleStep;
		sampleDisSum += cloudStepSize;
	}

	return clamp(cloudColor, 0.0f, 1.0f);
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
	float intersectDis = distance(wsCamPos, intersectPos);
	
	float fogStartToCamDis;
	vec4 fogColor = GetFog(viewRay, lightRay, intersectPos, intersectDis, stensilMark, fogStartToCamDis);
	fogColor.rgb *= fogColor.a;

	float cloudStartToCamDis;
	vec4 cloudColor = GetCloud(viewRay, lightRay, intersectPos, intersectDis, stensilMark, cloudStartToCamDis);
	//cloudColor.rgb *= cloudColor.a;

	if(fogStartToCamDis <= cloudStartToCamDis)
		cColor.rgb = cloudColor.rgb * (1.0f - fogColor.a) + fogColor.rgb;
	else
		cColor.rgb = cloudColor.rgb + fogColor.rgb * (1.0f - cloudColor.a);

	cColor.a = (1.0f - fogColor.a) * (1.0f - cloudColor.a);
	//cColor.rgb = fogColor.rgb;
	cColor = clamp(cColor, 0.0f, 1.0f);
}