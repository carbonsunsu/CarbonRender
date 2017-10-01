#version 430

layout(location = 0) out vec4 cColor;

in vec3 wsP;

uniform sampler3D perlinWorleyMap;
uniform sampler3D worleyMap;
uniform sampler2D curlMap;
uniform sampler2D depthMap;
uniform vec3 wsCamPos;
uniform vec4 cloudBoxSize;
uniform vec3 sunColor;
uniform vec3 zenithColor;
uniform mat4 viewMat;
uniform mat4 proMat;

const float stepLength = 20.0f;

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

void main ()
{
	float Coverage = 0.65f;
	float realCoverage = 1.0f - Coverage;

	vec3 ray = normalize(wsP - wsCamPos);

	vec3 sampleP = IsInCloud(wsCamPos) ? wsCamPos : wsP - ray;
	float sLength = stepLength;
	int stepCount = 0;
	int sampleCount = 0;
	float cloudDensity = 0.0f;

	while(stepCount <= 128)
	{
		vec4 ssP = (proMat * (viewMat * vec4(sampleP.x,sampleP.y,sampleP.z, 1.0f)));
		ssP.xy = ssP.xy / ssP.w;
		ssP.xy = ssP.xy * 0.5f + 0.5f;

		vec4 objN = texture2D(depthMap, ssP.xy);
		if (ssP.w >= objN.w && distance(objN.xyz, vec3(0.0f)) > 0.0f) break;
		if (!IsInCloud(sampleP))
		{
			sampleP += ray * sLength;
			stepCount++;
			continue;
		}
		vec3 uvw = sampleP * 0.00002f * 0.5f + 0.5f;
		uvw *= 8.0f;
		vec4 pwNoise = texture(perlinWorleyMap, uvw);
		float lfFbm = pwNoise.g * 0.625f + pwNoise.b * 0.25f + pwNoise.a * 0.125f;
		float baseCloudDensity = Remap(1.0f - pwNoise.r, -(1.0f - lfFbm), 1.0f, 0.0f, 1.0f);

		float stratusF = GetCloudTypeFactor(sampleP, vec4(0.0f, 0.15f, 0.15f, 0.3f));
		float cumulusF = GetCloudTypeFactor(sampleP, vec4(0.1f, 0.3f, 0.4f, 0.6f));
		float cumulonumbisF = GetCloudTypeFactor(sampleP, vec4(0.0f, 0.2f, 0.7f, 0.9f));
		float heightF = GetHeightAttenFactor(sampleP);
		baseCloudDensity *= cumulusF * heightF;

		baseCloudDensity = Remap(baseCloudDensity, realCoverage, 1.0f, 0.0f, 1.0f);
		baseCloudDensity *= realCoverage;

		//do curl
		sampleP.xy += vec2(0.0f, 0.0f);

		uvw *= 4.0f;
		pwNoise = texture(worleyMap, uvw);
		float hfFbm = pwNoise.r * 0.625f + pwNoise.g * 0.25f + pwNoise.b * 0.125f;
		hfFbm = mix(hfFbm, 1.0f - hfFbm, clamp(heightF * 10.0f, 0.0f, 1.0f));
		//baseCloudDensity = Remap(baseCloudDensity, hfFbm * 0.2f, 1.0f, 0.0f, 1.0f);

		if (baseCloudDensity > 0.0f)
		{
			cloudDensity += baseCloudDensity;
			sampleCount++;
		}
		
		sampleP += ray * sLength;
		stepCount++;
	}

	float Powder = 1.0f - exp(-cloudDensity * 2.0f);
	float Beers = max(exp(-cloudDensity), exp(-cloudDensity * 0.25f) * 0.7f);
	cloudDensity = min(cloudDensity, 1.0f);
	cColor.rgb = cloudDensity.rrr * clamp(Beers * Powder, 0.0f, 1.0f) * sunColor;
	cColor.rgb = clamp(cColor.rgb, 0.0f, 1.0f);
	cColor.a = cloudDensity;
	return;
	vec4 tex = texture(worleyMap, wsP * 0.0002f * 0.5f + 0.5f);
	cColor.rgb = (tex.r * 0.625f + tex.g * 0.25f + tex.b * 0.125f).rrr;
	cColor.a = 1.0f;
}