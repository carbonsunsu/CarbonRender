#version 430
#define PI 3.14159265359f
#define SIGMA 10.0f

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform vec3 stepUnit;
uniform sampler2D shadowMap;
uniform sampler2D stenMap;

float GetWeight (float dis)
{
	return exp(-0.5f * pow(dis / SIGMA, 2.0f));
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}

	vec2 sumColor = vec2(0.0f);
	float sumWeight = 0.0f;
	float weight;
	
	weight = GetWeight(0);
	sumWeight += weight;
	vec4 sMap = texture2D(shadowMap, uv);
	sumColor += sMap.rb * weight;

	for (int i = 1; i < 9; i++)
	{
		weight = GetWeight(stepUnit.z * i);
		sumWeight += weight * 2.0f;
		sumColor += texture2D(shadowMap, uv + stepUnit.xy * i).rb * weight;
		sumColor += texture2D(shadowMap, uv + stepUnit.xy * -i).rb * weight;
	}
	sumColor = sumColor / sumWeight;

	sColor = vec4(sumColor.r, 0.0f, sumColor.g, 1.0f);
}