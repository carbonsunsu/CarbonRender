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
uniform vec3 depthClampPara;//(nearClip, 1.0/(farClip - nearClip))
uniform vec3 lightPos;
uniform float lightSize;
uniform vec3 shadowMapSizes;

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

vec2 GetShadowUV (vec4 pos, out int level, out float scaler)
{
	level = 0;
	scaler = 0.5f / shadowMapSizes.x;
	vec4 smPos = smProMatLv0 * pos;
	vec2 smUV = smPos.xy / smPos.w * 0.5f + 0.5f;
	
	if (!IsInScreen(smUV))
	{
		level = 1;
		scaler = 0.5f / shadowMapSizes.y;
		smPos = smProMatLv1 * pos;
		smUV = smPos.xy / smPos.w * 0.5f + 0.5f;
		
		if (!IsInScreen(smUV))
		{
			level = 2;
			scaler = 0.5f / shadowMapSizes.z;
			smPos = smProMatLv2 * pos;
			smUV = smPos.xy / smPos.w * 0.5f + 0.5f;

			if (!IsInScreen(smUV))
				level = -1;
		}
	}

	return smUV;
}

vec4 GetShadowMap(int level, vec2 uv)
{
	if (level == 0)
		return texture2D(smMapLv0, uv);
	else if (level == 1)
		return texture2D(smMapLv1, uv);
	else
		return texture2D(smMapLv2, uv);
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0){discard;return;}
	
	vec4 pMapSample = texture2D(pMap, uv);
	vec3 wsPos = pMapSample.xyz;
	vec4 smPos = smViewMat * vec4(wsPos, 1.0f);
	float rcverDepth = -smPos.z;
	rcverDepth = rcverDepth * depthClampPara.y;
	rcverDepth = clamp(rcverDepth, 0.0f, 1.0f);
	float nearClip01 = depthClampPara.x * depthClampPara.y;
	int shadowLv = 0;
	float searchRadiusScaler = 1.0f;
	vec2 smUV = GetShadowUV(smPos, shadowLv, searchRadiusScaler);

	//change bias according angle between normal and light vector
	vec3 lightV = lightPos;
	lightV = normalize(lightV);
	vec4 N = texture2D(nMap, uv);
	float depthBias = abs(dot(N.xyz, lightV));
	depthBias = 1.0f - min(depthBias, 1.0f);
	depthBias = clamp(depthBias, 0.1f, 1.0f);
	depthBias *= 0.05f * depthClampPara.y;
	
	if (shadowLv >= 0)
	{	
		//calculate avarage blocker depth
		float searchR = lightSize * (rcverDepth - nearClip01) * searchRadiusScaler / rcverDepth;
		float avgBlkerDepth = 0.0f;
		int blkerCount = 0;
		int sampleNum = 6;
		float sampleNumInv = 1.0f / sampleNum;
		for (int i = -sampleNum; i <= sampleNum; i++)
		{
			for (int j = -sampleNum; j <= sampleNum; j++)
			{
				vec2 sampleUV = smUV + stepUnit * vec2(i, j) * sampleNumInv * searchR;
				if (IsInScreen(sampleUV))
				{
					float blkerDepth = GetShadowMap(shadowLv, sampleUV).r;
					if (blkerDepth < rcverDepth - depthBias)
					{
						avgBlkerDepth += blkerDepth;
						blkerCount += 1;
					}
				}
			}
		}
		if (blkerCount == 0){sColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);return;}

		avgBlkerDepth /= blkerCount;
		
		//calculate penumbra size
		float penumbraSize = lightSize * (rcverDepth - avgBlkerDepth) * searchRadiusScaler / avgBlkerDepth;

		//sample blokers by penumbra size and calculate shadow
		float sFactor = 0.0f;
		blkerCount = 0;
		sampleNum = 6;
		sampleNumInv = 1.0f / sampleNum;
		for (int i = -sampleNum; i <= sampleNum; i++)
		{
			for (int j = -sampleNum; j <= sampleNum; j++)
			{
				vec2 sampleUV = smUV + stepUnit * vec2(i, j) * sampleNumInv * penumbraSize;
				if (IsInScreen(sampleUV))
				{
					float blkerDepth = GetShadowMap(shadowLv, sampleUV).r;
					sFactor += blkerDepth < rcverDepth - depthBias ? 0.0f : 1.0f;
					blkerCount += 1;
				}
			}
		}
		if (blkerCount == 0){sColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);return;}

		sFactor /= blkerCount;
		sColor = vec4(clamp(sFactor, 0.0f, 1.0f), 0.0f, 1.0f, 1.0f);
		return;
	}

	sColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}