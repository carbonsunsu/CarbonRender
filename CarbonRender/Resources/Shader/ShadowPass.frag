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
uniform vec3 depthClampPara;//(nearClip, farClip, 1.0/farClip)
uniform vec3 lightPos;
uniform float lightSize;
uniform vec3 shadowMapSizes;

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

vec4 GetShadowMap(int level, vec2 uv, vec4 pos, vec2 uvBias)
{
	if (level == 0)
	{	
		if (!IsInScreen(uv))
		{
			vec4 smPos = smProMatLv1 * pos;
			vec2 newUV = smPos.xy / smPos.w * 0.5f + 0.5f;
			newUV += uvBias * (0.5f / shadowMapSizes.y);
			return texture2D(smMapLv1, newUV);
		}
		else
			return texture2D(smMapLv0, uv);
	}
	else if (level == 1)
	{	
		if (!IsInScreen(uv))
		{
			vec4 smPos = smProMatLv2 * pos;
			vec2 newUV = smPos.xy / smPos.w * 0.5f + 0.5f;
			newUV += uvBias * (0.5f / shadowMapSizes.z);
			return texture2D(smMapLv2, newUV);
		}
		else
			return texture2D(smMapLv1, uv);
	}
	else
		return texture2D(smMapLv2, clamp(uv, 0.0f, 1.0f));
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
	
	if (shadowLv >= 0)
	{	
		//change bias according angle between normal and light vector
		vec3 lightV = lightPos;
		lightV = normalize(lightV);
		vec4 N = texture2D(nMap, uv);
		float depthBias = abs(dot(N.xyz, lightV));
		depthBias = 1.0f - min(depthBias, 1.0f);
		depthBias = clamp(depthBias, 0.1f, 1.0f);
		depthBias *= 0.05f * depthClampPara.y;

		//calculate avarage blocker depth
		float searchR = lightSize * (rcverDepth - nearClip01) * 0.5f / rcverDepth;
		float avgBlkerDepth = 0.0f;
		int blkerCount = 0;
		int sampleNum = 6;
		float sampleNumInv = 1.0f / sampleNum;
		for (int i = -sampleNum; i <= sampleNum; i++)
		{
			for (int j = -sampleNum; j <= sampleNum; j++)
			{
				vec2 sampleBias = stepUnit * vec2(i, j) * sampleNumInv * searchR;
				vec2 sampleUV = smUV + sampleBias * searchRadiusScaler;
				float blkerDepth = GetShadowMap(shadowLv, sampleUV, smPos, sampleBias).r;
				if (blkerDepth < rcverDepth - depthBias)
				{
					avgBlkerDepth += blkerDepth;
					blkerCount += 1;
				}
			}
		}
		if (blkerCount == 0){sColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);return;}

		avgBlkerDepth /= blkerCount;
		
		//calculate penumbra size
		float penumbraSize = lightSize * (rcverDepth - avgBlkerDepth) * 0.5f / avgBlkerDepth;

		//sample blokers by penumbra size and calculate shadow
		float sFactor = 0.0f;
		sampleNum = 6;
		sampleNumInv = 1.0f / sampleNum;
		for (int i = -sampleNum; i <= sampleNum; i++)
		{
			for (int j = -sampleNum; j <= sampleNum; j++)
			{
				vec2 sampleBias = stepUnit * vec2(i, j) * sampleNumInv * penumbraSize;
				vec2 sampleUV = smUV + sampleBias * searchRadiusScaler;
				float blkerDepth = GetShadowMap(shadowLv, sampleUV, smPos, sampleBias).r;
				sFactor += blkerDepth < rcverDepth - depthBias ? 0.0f : 1.0f;
			}
		}

		sFactor /= (sampleNum * 2 + 1) * (sampleNum * 2 + 1);
		sColor = vec4(clamp(sFactor, 0.0f, 1.0f), 0.0f, 0.0f, 0.0f);
		return;
	}

	sColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);
}