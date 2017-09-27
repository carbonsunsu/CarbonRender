#version 430

layout(location = 0) out vec4 cColor;

in vec3 wsP;
in vec2 uv;

uniform sampler3D perlinWorleyMap;
uniform sampler3D worleyMap;
uniform sampler2D curlMap;
uniform vec3 wsCamPos;
uniform vec4 cloudBoxSize;

const float stepLength = 50.0f;

float GetHeightAttenFactor(vec3 pos)
{
	float a = pos.y - 1500.0f;
	a = a / 7500.0f;
	a = clamp(a, 0.0f, 1.0f);
	a = pow(a, 0.3f);
	return a;
}

float GetStratusFactor(vec3 pos)
{
	return 1.0f;
}

float GetCumulusFactor(vec3 pos)
{
	return pos.y <= 3500.0f ? pow(1.0f - clamp(abs(pos.y - 2500.0f) / 1500.0f, 0.0f, 1.0f), 0.5f) : 0.0f;
}

float GetCumulonumbisFactor(vec3 pos)
{
	return 1.0f;
}

bool IsInCloud(vec3 pos)
{
	return abs(pos.x) < cloudBoxSize.x && 
		   abs(pos.z) < cloudBoxSize.z && 
		   pos.y < cloudBoxSize.y + cloudBoxSize.w;
		   pos.y > cloudBoxSize.w - cloudBoxSize.y;
}

void main ()
{
	vec3 ray = normalize(wsP - wsCamPos);

	vec3 sampleP = wsCamPos;//IsInCloud(wsCamPos) ? wsP : wsCamPos;
	float sLength = stepLength;
	int stepCount = 0;
	vec3 cloudColor = vec3(0.0f);
	int sampleCount = 0;

	while(stepCount <= 256)
	{
		if (!IsInCloud(sampleP))
		{
			sampleP += ray * sLength;
			stepCount++;
			continue;
		}
		vec3 uvw = sampleP * 0.00005f + 0.25f;
		vec4 pwNoise = texture(perlinWorleyMap, uvw);
		float cloudDensity = pwNoise.x + pwNoise.y + pwNoise.z + pwNoise.w;
		if (cloudDensity > 0.0f)
		{
			cloudDensity *= GetCumulusFactor(sampleP);
			//cloudDensity = clamp(cloudDensity, 0.0f, 1.0f);
			cloudDensity *= 0.25f;
			cloudDensity *= 0.5f * GetHeightAttenFactor(sampleP);
			cloudColor += cloudDensity.rrr;
			sampleCount++;
		}
		
		sampleP += ray * sLength;
		stepCount++;
	}
	cloudColor = cloudColor / sampleCount;

	cColor.rgb = clamp(cloudColor.rrr, 0.0f, 1.0f);
	cColor.a = cloudColor.r;
}