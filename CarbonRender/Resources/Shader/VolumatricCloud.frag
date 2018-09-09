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

uniform vec3 cloudBias;

const float PLANET_RADIUS = 6500000.0f;
const vec3 PLANET_CENTER = vec3(0.0f, -PLANET_RADIUS, 0.0f);
const float MIN_CLOUD_ALTITUDE = 1500.0f;
const float MAX_CLOUD_ALTITUDE = 8000.0f;
const float CLOUD_HEIGHT = MAX_CLOUD_ALTITUDE - MIN_CLOUD_ALTITUDE;

const int SAMPLE_COUNT_MAX = 128;
const int SAMPLE_COUNT_MIN = 64;
const float STEP_SIZE_SHORT = 64.0f;
const float STEP_SIZE_LONG = 256.0f;
const float Pi = 3.1415926f;
const vec3 randomV[5] = vec3[5](vec3(-0.010735935, 0.01647018, 0.0062425877),
			                     vec3(-0.06533369, 0.3647007, -0.13746321),
								 vec3(-0.6539235, -0.016726388, -0.53000957),
								 vec3(0.40958285, 0.0052428036, -0.5591124),
								 vec3(-0.1465366, 0.09899267, 0.15571679));

float Remap(float value, float oldMin, float oldMax, float newMin, float newMax)
{
	return newMin + (value - oldMin) / (oldMax - oldMin) * (newMax - newMin);
}

float GetHeightGradient(vec3 pos)
{
	float a = distance(pos, PLANET_CENTER);
	a = (a - MIN_CLOUD_ALTITUDE - PLANET_RADIUS) / CLOUD_HEIGHT;
	a = clamp(a, 0.0f, 1.0f);

	return a;
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
	return a > MIN_CLOUD_ALTITUDE && a < MAX_CLOUD_ALTITUDE;
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
	float Beers = exp(-d * p);

	return 2.0f * Beers * Powder;
}

float HG (vec3 v, vec3 l)
{
	float VoL = dot(v, l);
	float g = 0.25f;
	float HG = (1.0f - g*g) / (4.0f * Pi * pow(1.0f + g*g - 2.0f * g * VoL, 1.5f));

	return HG;
}

float SampleCloudDensity(vec3 pos, vec4 weather, float mipmap, bool useCheapWay)
{
	vec3 samplePos = pos;
	vec3 uvw = UVW(samplePos + cloudBias, 4.0f);
	vec4 noise = textureLod(perlinWorleyMap, uvw, mipmap);
	float lfFbm = noise.g * 0.625f + noise.b * 0.25f + noise.a * 0.125f;
	lfFbm = Remap(noise.r, lfFbm - 1.0f, 1.0f, 0.0f, 1.0f);
	lfFbm = clamp(lfFbm, 0.0f, 1.0f);

	float heightGrad = GetHeightGradient(samplePos);

	vec4 stratusF = vec4(0.0f, 0.1f, 0.2f, 0.3f);
	stratusF *= 1.0f - clamp(weather.b * 2.0f, 0.0f, 1.0f);
	vec4 cumulusF =vec4(0.05f, 0.25f, 0.45f, 0.65f);
	cumulusF *= 1.0f - abs(weather.b - 0.5f) * 2.0f;
	vec4 cumulonumbisF = vec4(0.0f, 0.2f, 0.7f, 0.9f);
	cumulonumbisF *= clamp(weather.b - 0.5f, 0.0f, 1.0f) * 2.0f;
	float typeGrad = GetCloudTypeGradient(heightGrad, stratusF + cumulusF + cumulonumbisF);
	
	lfFbm *= typeGrad;

	lfFbm = Remap(lfFbm, weather.r, 1.0f, 0.0f, 1.0f);
	lfFbm *= weather.r;

	if (!useCheapWay)
	{
		//do curl
		vec4 curlNoise = texture2D(curlMap, uvw.xy);
		//uvw.xy += curlNoise.xy * (1.0f - heightGrad) * 1.0f;

		noise = textureLod(worleyMap, uvw * 0.1f, mipmap);
		float hfFbm = noise.r * 0.625f + noise.g * 0.25f + noise.b * 0.125f;
		hfFbm = mix(hfFbm, 1.0f - hfFbm, clamp(heightGrad * 10.0f, 0.0f, 1.0f));
		lfFbm = Remap(lfFbm, hfFbm * 0.2f, 1.0f, 0.0f, 1.0f);
	}

	return clamp(lfFbm, 0.0f, 1.0f);
}

void main ()
{
	if (wsP.y <= 0.0f)
	{
		cColor = vec4(0.0f);
		return;
	}

	float Coverage = 1.0f;
	float Precipitation = 1.0f;

	vec3 ray = normalize(wsP - wsCamPos);
	vec3 lightRay = normalize(wsSunPos);
	vec3 vL = normalize(PLANET_CENTER - wsCamPos);
	float L = distance(PLANET_CENTER, wsCamPos);
	float alpha = dot(vL, ray);
	float l1 = L * abs(alpha);
	float d = sqrt(L * L - l1 * l1);
	float r = PLANET_RADIUS + MIN_CLOUD_ALTITUDE;
	float l2 = sqrt(r * r - d * d);
	vec3 nearPos = alpha >= 0.0f ? wsCamPos + ray * (l1 + l2) : wsCamPos + ray * (l2 - l1);

	float VoU = dot(ray, vec3(0.0f, 1.0f, 0.0f));
	float sampleCount = mix(SAMPLE_COUNT_MAX, SAMPLE_COUNT_MIN, VoU);

	vec3 samplePos = IsInCloud(wsCamPos) ? wsCamPos : nearPos - ray;
	float stepSize = STEP_SIZE_LONG;
	vec4 cloudColor = vec4(0.0f);
	float densityTest = 0.0f;
	int zeroCount = 0;
	
	for(int i = 0; i < sampleCount; i++)
	{
		vec4 ssP = (proMat * (viewMat * vec4(samplePos.x, samplePos.y, samplePos.z, 1.0f)));
		ssP.xy = ssP.xy / ssP.w;
		ssP.xy = ssP.xy * 0.5f + 0.5f;

		vec4 objN = texture2D(depthMap, ssP.xy);
		if ((ssP.w >= objN.w && distance(objN.xyz, vec3(0.0f)) > 0.0f)
			|| samplePos.y < 0.0f
			//|| cloudColor.a >= 1.0f
			) break;

		if (!IsInCloud(samplePos))
		{
			samplePos += ray * STEP_SIZE_LONG;
			continue;
		}

		vec4 weatherData = texture2D(weatherMap, UVW(samplePos + cloudBias * 0.5f, 1.0f).xz);//R - coverage; G - precipitation; B - cloud type
		weatherData.r = weatherData.r * Coverage;
		weatherData.g = mix(3.0f, 3.0f, weatherData.g); 
		weatherData.g *= Precipitation;
		weatherData.a = 1.0f;

		weatherData.r = 0.15f;
		weatherData.b = 1.0f;
		float mipmapLev = 0.0f;

		if (densityTest > 0.0f)
		{
			float sampleDensity = SampleCloudDensity(samplePos, weatherData, mipmapLev, false);

			if (sampleDensity <= 0.0f)
				zeroCount++;

			if (zeroCount < 6)
			{
				cloudColor.a += sampleDensity;
				if (sampleDensity > 0.0f)
				{
					//Light
					vec3 lightSamplePos = samplePos;
					float lightRayDensity = 0.0f;
			
					for (int j = 0; j < 6; j++)
					{
						vec3 bias = randomV[j];
						bias = normalize(bias);
						lightSamplePos = samplePos + lightRay * 64.0f * (j+1) + bias * (j+1) * 5.0f;
						float density;

						if (lightRayDensity < 0.3f)
							density = SampleCloudDensity(lightSamplePos, weatherData, mipmapLev + 1.0f, false);
						else
							density = SampleCloudDensity(lightSamplePos, weatherData, mipmapLev + 1.0f, true);
						lightRayDensity += density;
						//if (lightRayDensity >= 1.0f) break;
					}

					float pb0 = PowderBeers(lightRayDensity + cloudColor.a, weatherData.g);
					float pb1 = PowderBeers(cloudColor.a, weatherData.g);
					float hg = HG(-ray, normalize(-wsSunPos));
					vec3 sampleColor = sunColor * pb0 * hg ;
			
					//ambient
					//float heightGrad = GetHeightGradient(samplePos);
					//vec3 ambientColor = mix(zenithColor, sunColor, heightGrad);
					//ambientColor = ambientColor * pb1;
					//sampleColor += ambientColor;

					cloudColor.rgb += sampleColor;
				}
				
				samplePos += ray * STEP_SIZE_SHORT;
			}
			else
			{
				densityTest = 0.0f;
				zeroCount = 0;
			}
		}
		else
		{
			densityTest = SampleCloudDensity(samplePos, weatherData, mipmapLev, true);
			if (densityTest <= 0.0f)
				samplePos += ray * STEP_SIZE_LONG;
		}
	}

	cColor = cloudColor;
	cColor = clamp(cColor, 0.0f, 1.0f);
}