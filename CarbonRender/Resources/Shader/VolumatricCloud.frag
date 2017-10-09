#version 430

layout(location = 0) out vec4 cColor;

in vec3 wsP;

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

uniform vec4 cloudBoxSize;
uniform vec3 cloudBias;

const int SAMPLE_COUNT = 128;
const float stepSizeMin = 15.0f;
const float stepSizeMax = 30.0f;
const float Pi = 3.1415926f;

float Remap(float value, float oldMin, float oldMax, float newMin, float newMax)
{
	return newMin + (value - oldMin) / (oldMax - oldMin) * (newMax - newMin);
}

float GetHeightAttenFactor(vec3 pos)
{
	float a = pos.y - (cloudBoxSize.w - cloudBoxSize.y);
	a = a / (cloudBoxSize.y * 2.0f);
	a = clamp(a, 0.0f, 1.0f);
	a = pow(a, 0.3f);
	return a;
}

float GetCloudTypeFactor(vec3 pos, vec4 typePara)
{
	float a = pos.y - (cloudBoxSize.w - cloudBoxSize.y);
	a = a / (cloudBoxSize.y * 2.0f);
	a = Remap(a, typePara.x, typePara.y, 0.0f, 1.0f) * Remap(a, typePara.z, typePara.w, 1.0, 0.0);
	a = clamp(a, 0.0f, 1.0f);

	return a;	
}

bool IsInCloud(vec3 pos)
{
	return abs(pos.x) < cloudBoxSize.x && 
		   abs(pos.z) < cloudBoxSize.z && 
		   pos.y < cloudBoxSize.y + cloudBoxSize.w &&
		   pos.y > cloudBoxSize.w - cloudBoxSize.y;
}

vec3 UVW(vec3 p, float scale)
{
	vec3 uvw = p * 0.00002f * 0.5f + 0.5f;
	uvw *= scale;

	return uvw;
}

float PowderBeers (float d, float p)
{
	float Powder = 1.0f - exp(-d * 2.0f);
	float Beers = exp(-d * p);// max(exp(-d), exp(-d * 0.25f) * 0.7f);

	return clamp(2.0f * Powder * Beers, 0.0f, 1.0f);
}

float HG (vec3 v, vec3 l)
{
	float VoL = dot(v, l);
	float g = 0.5f;
	float HG = (1.0f - g*g) / (4.0f * Pi * pow(1.0f + g*g - 2.0f * g * VoL, 1.5f));

	return clamp(HG, 0.0f, 1.0f);
}

float SampleCloudDensity(vec3 pos, vec4 weather)
{
	vec3 samplePos = pos;
	vec3 uvw = UVW(samplePos + cloudBias, 16.0f);
	vec4 noise = texture(perlinWorleyMap, uvw);
	float lfFbm = noise.g * 0.625f + noise.b * 0.25f + noise.a * 0.125f;
	lfFbm = Remap(1.0f - noise.r, -(1.0f - lfFbm), 1.0f, 0.0f, 1.0f);

	float stratusF = GetCloudTypeFactor(samplePos, vec4(0.0f, 0.15f, 0.15f, 0.3f));
	float cumulusF = GetCloudTypeFactor(samplePos, vec4(0.1f, 0.3f, 0.4f, 0.6f));
	float cumulonumbisF = GetCloudTypeFactor(samplePos, vec4(0.0f, 0.2f, 0.7f, 0.9f));
	float heightF = GetHeightAttenFactor(samplePos);
	float typeFactor;
	if (weather.z <= 0) typeFactor = stratusF;
	else if (weather.z >= 1) typeFactor = cumulonumbisF;
	else typeFactor = cumulusF;
	lfFbm *= typeFactor * heightF;

	lfFbm = Remap(lfFbm, weather.r, 1.0f, 0.0f, 1.0f);
	lfFbm *= 1.0f - weather.r;

	//do curl
	//samplePos.xy += vec2(0.0f, 0.0f);

	noise = texture(worleyMap, uvw);
	float hfFbm = noise.r * 0.625f + noise.g * 0.25f + noise.b * 0.125f;
	hfFbm = mix(hfFbm, 1.0f - hfFbm, clamp(heightF * 10.0f, 0.0f, 1.0f));
	
	float density = Remap(lfFbm, hfFbm * 0.2f, 1.0f, 0.0f, 1.0f);

	return density;
}

void main ()
{
	
	float Coverage = 0.9f;
	float Precipitation = 1.0f;
	vec4 weatherData = texture2D(weatherMap, UVW(wsP, 1.0f).xz);
	weatherData.r = 1.0f - weatherData.r * Coverage;
	weatherData.g = mix(3.0f, 9.0f, weatherData.g);
	weatherData.g *= Precipitation;
	weatherData.a = 1.0f;

	vec3 ray = normalize(wsP - wsCamPos);
	float VoU = dot(ray, vec3(0.0f, 1.0f, 0.0f));
	float stepLengthScale = mix(4.0f, 1.0f, VoU);

	vec3 samplePos = IsInCloud(wsCamPos) ? wsCamPos : wsP - ray;
	float stepSize = stepSizeMax;
	int stepCount = 0;
	vec4 cloudColor = vec4(0.0f);

	while(stepCount <= SAMPLE_COUNT)
	{
		vec4 ssP = (proMat * (viewMat * vec4(samplePos.x, samplePos.y, samplePos.z, 1.0f)));
		ssP.xy = ssP.xy / ssP.w;
		ssP.xy = ssP.xy * 0.5f + 0.5f;

		vec4 objN = texture2D(depthMap, ssP.xy);
		if (ssP.w >= objN.w && distance(objN.xyz, vec3(0.0f)) > 0.0f) break;
		if (!IsInCloud(samplePos))
		{
			samplePos += ray * stepSize * stepLengthScale;
			stepCount++;
			continue;
		}

		float baseCloudDensity = SampleCloudDensity(samplePos, weatherData);

		if (baseCloudDensity > 0.0f)
		{
			//Light
			vec3 lightSamplePos = samplePos;
			float lightRayDensity = 0.0f;
			vec3 lightRay = normalize(wsSunPos - lightSamplePos);
			
			for (int j = 0; j < 5; j++)
			{
				lightSamplePos += lightRay * stepSizeMax;

				lightRayDensity += SampleCloudDensity(lightSamplePos, weatherData);
			}
			
			float pb = PowderBeers(lightRayDensity, weatherData.y);
			float hg = HG(-ray, normalize(-wsSunPos));
			vec3 sampleColor = sunColor * hg * pb + zenithColor * pb;

			//sampleColor *= baseCloudDensity;
			sampleColor *= max(1.0f - cloudColor.a, 0.1f);
			cloudColor.rgb += sampleColor;

			cloudColor.a += baseCloudDensity;
			stepSize = stepSizeMin;

			if (cloudColor.a >= 1.0f) break;
		}
		else
		{
			stepSize = stepSizeMax;
		}
		
		samplePos += ray * stepSize * stepLengthScale;
		stepCount++;
	}

	cColor = cloudColor;//texture(perlinWorleyMap, UVW(wsP)).rrr;//
	cColor = clamp(cColor, 0.0f, 1.0f);
}