#version 430

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform sampler2D pMap;
uniform sampler2D smMapLv0;
uniform sampler2D smMapLv1;
uniform sampler2D smMapLv2;
uniform sampler2D stenMap;
uniform sampler2D nMap;
uniform mat4 smViewMat;
uniform mat4 smProMatLv0;
uniform mat4 smProMatLv1;
uniform mat4 smProMatLv2;
uniform vec2 stepUnit;
uniform vec2 depthClampPara;//(nearClip, 1.0/(farClip - nearClip))
uniform vec3 lightPos;
uniform float lightSize;

float Random (vec2 i, float seed)
{
	//golden noise ranged from -1 to 1
	float phi = 1.61803398874989484820459 * 00000.1; // Golden Ratio   
	float pi  = 3.14159265358979323846264 * 00000.1; // PI
	float srt = 1.41421356237309504880169 * 10000.0; // Square Root of Two

	return fract(tan(distance(i * (seed + phi), vec2(phi, pi))) * srt) * 2.0f - 1.0f;
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
		{
			level = 2;
			smPos = smProMatLv2 * pos;
			smUV = smPos.xy / smPos.w * 0.5f + 0.5f;
		}
	}

	return smUV;
}

sampler2D GetShadowSampler(int level)
{
	if (level == 0)
		return smMapLv0;
	else if (level == 1)
		return smMapLv1;
	else
		return smMapLv2;
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}
	
	vec4 pMapSample = texture2D(pMap, uv);
	vec3 wsPos = pMapSample.xyz;
	vec4 smPos = smViewMat * vec4(wsPos, 1.0f);
	float rcverDepth = -smPos.z;
	int shadowLv = 0;
	vec2 smUV = GetShadowUV(smPos, shadowLv);

	//change bias according angle between normal and light vector
	vec3 lightV = lightPos;
	lightV = normalize(lightV);
	vec4 N = texture2D(nMap, uv);
	float shadowBias = dot(N.xyz, lightV);
	shadowBias = 1.0f - abs(shadowBias);
	shadowBias = max(5.0f * shadowBias, 1.0f) * 0.05f;
	
	float sFactor = 1.0f;
	if (IsInScreen(smUV))
	{	
		float searchR = (lightSize * rcverDepth) / (rcverDepth - depthClampPara.x);
		float avgBlkerDepth = 0.0f;
		int sampleCount = 0;
		int sampleNum = shadowLv == 0 ? 2 : 0;
		for (int i = -sampleNum; i <= sampleNum; i++)
		{
			for (int j = -sampleNum; j <= sampleNum; j++)
			{
				vec2 sampleUV = smUV + stepUnit * vec2(i, j) * searchR * 2.0f / lightSize;
				if (IsInScreen(sampleUV))
				{
					float blkerDepth = texture2D(GetShadowSampler(shadowLv), sampleUV).r;
					if (blkerDepth < rcverDepth - shadowBias)
					{
						avgBlkerDepth += blkerDepth;
						sampleCount += 1;
					}
				}
			}
		}
		if (sampleCount <= 0)
		{
			sColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
			return;
		}
		avgBlkerDepth /= sampleCount;
		
		float penumbraSize = lightSize * (rcverDepth - avgBlkerDepth) / avgBlkerDepth;
		penumbraSize = shadowLv == 0 ? penumbraSize : 0.0f;
		sFactor = 0.0f;
		sampleCount = 0;
		sampleNum = shadowLv == 0 ? 4 : 0;
		for (int i = -sampleNum; i <= sampleNum; i++)
		{
			for (int j = -sampleNum; j <= sampleNum; j++)
			{
				vec2 sampleUV = smUV + stepUnit * vec2(i, j) * penumbraSize;
				if (IsInScreen(sampleUV))
				{
					float blkerDepth = texture2D(GetShadowSampler(shadowLv), sampleUV).r;
					sFactor += blkerDepth < rcverDepth - shadowBias ? 0.0f : 1.0f;
					sampleCount += 1;
				}
			}
		}
		sFactor /= sampleCount;
		sColor = vec4(sFactor, 0.0f, 1.0f, 1.0f);
		return;
	}

	sColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}