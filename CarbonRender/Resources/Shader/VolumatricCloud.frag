#version 430

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

const float PLANET_RADIUS = 6500000.0f;
const vec3 PLANET_CENTER = vec3(0.0f, -PLANET_RADIUS, 0.0f);
const float MIN_CLOUD_ALTITUDE = 1500.0f;
const float MAX_CLOUD_ALTITUDE = 4000.0f;
const float CLOUD_HEIGHT = MAX_CLOUD_ALTITUDE - MIN_CLOUD_ALTITUDE;
const float CLOUD_HEIGHT_INV = 1.0f / CLOUD_HEIGHT;

const int SAMPLE_COUNT = 128;
const float Pi = 3.1415926f;
const vec3 randomV[5] = vec3[5](vec3(-0.010735935, 0.01647018, 0.0062425877),
			                     vec3(-0.06533369, 0.3647007, -0.13746321),
								 vec3(-0.6539235, -0.016726388, -0.53000957),
								 vec3(0.40958285, 0.0052428036, -0.5591124),
								 vec3(-0.1465366, 0.09899267, 0.15571679));

const vec4 StratusFactor = vec4(0.0f, 0.1f, 0.2f, 0.3f);
const vec4 CumulusFactor =vec4(0.05f, 0.25f, 0.45f, 0.65f);
const vec4 CumulonumbisFactor = vec4(0.0f, 0.2f, 0.7f, 0.9f);

const float Coverage = 0.15;
const float Precipitation = 1.0f;

float Remap(float value, float oldMin, float oldMax, float newMin, float newMax)
{
	return newMin + (value - oldMin) / (oldMax - oldMin) * (newMax - newMin);
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

float SampleCloudDensity(vec3 pos, vec3 weather, float heightF, float mipmap, bool useCheapWay)
{
	vec3 samplePos = pos;
	vec3 uvw = UVW(samplePos + cloudBias, 16.0f);
	vec4 noise = textureLod(perlinWorleyMap, uvw, mipmap);
	float lfFbm = noise.g * 0.625f + noise.b * 0.25f + noise.a * 0.125f;
	lfFbm = Remap(noise.r, lfFbm - 1.0f, 1.0f, 0.0f, 1.0f);
	lfFbm = clamp(lfFbm, 0.0f, 1.0f);

	vec4 sf = StratusFactor * (1.0f - clamp(weather.b * 2.0f, 0.0f, 1.0f));
	vec4 cf = CumulusFactor * (1.0f - abs(weather.b - 0.5f) * 2.0f);
	vec4 cbf = CumulonumbisFactor * clamp(weather.b - 0.5f, 0.0f, 1.0f) * 2.0f;
	float typeGrad = GetCloudTypeGradient(heightF, sf + cf + cbf);	
	lfFbm *= typeGrad;

	float coverage = weather.r * min((1.0f - heightF) + 0.2, 1.0f);
	lfFbm = Remap(lfFbm, coverage, 1.0f, 0.0f, 1.0f);
	lfFbm *= coverage;

	if (!useCheapWay)
	{
		//do curl
		//vec4 curlNoise = texture2D(curlMap, uvw.xy);
		//uvw.xy += curlNoise.xy * (1.0f - heightGrad) * 1.0f;

		noise = textureLod(worleyMap, uvw * 0.1f, mipmap);
		float hfFbm = noise.r * 0.625f + noise.g * 0.25f + noise.b * 0.125f;
		hfFbm = mix(hfFbm, 1.0f - hfFbm, clamp(heightF * 10.0f, 0.0f, 1.0f));
		lfFbm = Remap(lfFbm, hfFbm * 0.2f, 1.0f, 0.0f, 1.0f);
	}

	return clamp(lfFbm, 0.0f, 1.0f);
}

vec3 ConeTracingLight(vec3 pos, float density, float heightF, vec3 lightRay, vec3 viewRay, vec3 weatherData, float mipmapLev)
{
	vec3 lightSamplePos;
	float lightRayDensity = 0.0f;
			
	for (int j = 0; j < 4; j++)
	{
		vec3 bias = randomV[j];
		bias = normalize(bias);
		lightSamplePos = pos + lightRay * pow(2.0f, (j + 0)) + bias * (j+1) * 2.0f;
		float heightF = GetHeightGradient(lightSamplePos);

		lightRayDensity += SampleCloudDensity(lightSamplePos, weatherData, heightF, mipmapLev + 1.0f, false);//lightRayDensity >= 0.3f);

		if (lightRayDensity >= 1.0f) break;
	}

	float pb0 = PowderBeers(lightRayDensity + density, weatherData.g);
	float pb1 = PowderBeers(density, weatherData.g);
	float hg = HG(-viewRay, normalize(-wsSunPos));
	vec3 sampleColor = sunColor * pb0 * pb0 * hg * 3.0f;
	
	//ambient
	vec3 ambientColor = sunColor * heightF;
	ambientColor = ambientColor * pb1 * pb1;
	sampleColor += ambientColor * 2.0f;
	
	return sampleColor;
}

void main ()
{
	if (wsP.y <= 0.0f)
	{
		cColor = vec4(0.0f);
		return;
	}
	
	vec3 viewRay = normalize(wsP.xyz - wsCamPos);
	vec3 lightRay = normalize(wsSunPos);
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

	int sampleCount = SAMPLE_COUNT;
	float stepSizeShort = distance(nearPos, farPos) * 0.5f / sampleCount;
	float stepSizeLong = stepSizeShort * 2.0f;

	vec3 samplePos = IsInCloud(wsCamPos) ? wsCamPos : nearPos;
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
			|| (distance(samplePos, PLANET_CENTER) - PLANET_RADIUS) > MAX_CLOUD_ALTITUDE
			) break;
			
		float heightF = GetHeightGradient(samplePos);
		vec3 weatherData = texture2D(weatherMap, UVW(samplePos + cloudBias * 2.0f, 1.0f).xz).rgb;//R - coverage; G - precipitation; B - cloud type
		weatherData.r = weatherData.r * Coverage;
		weatherData.g = mix(3.0f, 5.0f, weatherData.g); 
		weatherData.g *= Precipitation;

		//weatherData.r = 0.1f;
		weatherData.g = 4.0f;
		weatherData.b = 1.0f;
		float mipmapLev = 0.0f;
		
		if (densityTest > 0.0f)
		{
			float sampleDensity = SampleCloudDensity(samplePos, weatherData, heightF, mipmapLev, false);

			if (sampleDensity <= 0.0f)
				zeroCount++;

			if (zeroCount < 6)
			{
				cloudColor.a += sampleDensity;

				if (sampleDensity > 0.0f)
					cloudColor.rgb += ConeTracingLight(samplePos, cloudColor.a, heightF, lightRay, viewRay, weatherData, mipmapLev);
				
				samplePos += viewRay * stepSizeShort;
			}
			else
			{
				densityTest = 0.0f;
				zeroCount = 0;
			}
		}
		else
		{
			densityTest = SampleCloudDensity(samplePos, weatherData, heightF, mipmapLev, true);
			if (densityTest <= 0.0f)
				samplePos += viewRay * stepSizeLong;
		}

		if (cloudColor.a >= 1.0f) break;
	}

	cColor = cloudColor;
	cColor = clamp(cColor, 0.0f, 1.0f);
}