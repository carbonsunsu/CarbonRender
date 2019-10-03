#version 430
#define PI 3.14159265359f
#define SIGMA 10.0f

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform vec3 stepUnit;
uniform sampler2D shadowMap;
uniform sampler2D stenMap;
uniform sampler2D nMap;
uniform vec2 depthClampPara;

float GetWeight (float dis)
{
	return exp(-0.5f * pow(dis / SIGMA, 2.0f)) / sqrt(2.0f * PI * SIGMA * SIGMA);
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}

	vec2 blurResult = vec2(0.0f);
	vec2 sumWeight = vec2(0.0f);
	vec2 weight = vec2(0.0f);
	
	weight.rg = GetWeight(0).rr;
	sumWeight += weight;
	vec4 sMap = texture2D(shadowMap, uv);
	vec4 normalDepth = texture2D(nMap, uv);
	blurResult += sMap.rb * weight;
	//float linearDepth01 = (normalDepth.a - depthClampPara.x) * depthClampPara.y;
	//float scale = mix(1.0f, 1.0f, pow(linearDepth01, 100.0f));

	for (int i = 1; i < 9; i++)
	{
		vec4 sampleUV;
		sampleUV.xy = uv + stepUnit.xy * i;
		sampleUV.zw = uv + stepUnit.xy * -i;
		vec4 normalDepth0 = texture2D(nMap, sampleUV.xy);
		vec4 normalDepth1 = texture2D(nMap, sampleUV.zw);
		vec2 mask = vec2(1.0f);
		mask.r = (1.0f - min(abs(normalDepth.w - normalDepth0.w), 1.0f)) * max(dot(normalDepth0.xyz, normalDepth.xyz), 0.0f);
		mask.g = (1.0f - min(abs(normalDepth.w - normalDepth1.w), 1.0f)) * max(dot(normalDepth1.xyz, normalDepth.xyz), 0.0f);

		vec4 sample0 = texture2D(shadowMap, sampleUV.xy);
		vec4 sample1 = texture2D(shadowMap, sampleUV.zw);
		
		weight.g = GetWeight(stepUnit.z * i);
		blurResult.g += sample0.b * weight.g * mask.r;
		sumWeight.g += weight.g * mask.r;
		blurResult.g += sample1.b * weight.g * mask.g;
		sumWeight.g += weight.g * mask.g;

		weight.r = GetWeight(stepUnit.z * i);
		blurResult.r += sample0.r * weight.r * mask.r;
		sumWeight.r += weight.r * mask.r;
		blurResult.r += sample1.r * weight.r * mask.g;
		sumWeight.r += weight.r * mask.g;
	}
	blurResult = blurResult / sumWeight;

	sColor = vec4(blurResult.r, sMap.g, blurResult.g, 1.0f);
}