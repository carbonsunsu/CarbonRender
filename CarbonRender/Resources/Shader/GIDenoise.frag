#version 430
#define PI 3.14159265359f
#define SIGMA_D 10.0f
#define SIGMA_C 1.0f

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform vec3 stepUnit;
uniform sampler2D stenMap;
uniform sampler2D giMap;
uniform sampler2D nMap;

float GetWeight (float dis, float s)
{
	return exp(-0.5f * pow(dis / s, 2.0f)) / sqrt(2.0f * PI * s * s);;
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}

	vec3 sumColor = vec3(0.0f);
	float sumWeight = 0.0f;
	float weight;
	
	vec3 giColor = texture2D(giMap, uv).rgb;
	weight = GetWeight(0.0f, SIGMA_C) * GetWeight(0.0f, SIGMA_D);
	sumWeight += weight;
	sumColor += giColor * weight;

	for (int i = 1; i < 9; i++)
	{
		float gaussianWeight = GetWeight(length(stepUnit.z * i), SIGMA_D);
		vec3 sampleColor = texture2D(giMap, uv + stepUnit.xy * i).rgb;
		weight = GetWeight(distance(giColor, sampleColor), SIGMA_C) * gaussianWeight;
		sumWeight += weight;
		sumColor += sampleColor * weight;

		sampleColor = texture2D(giMap, uv + stepUnit.xy * -i).rgb;
		weight = GetWeight(distance(giColor, sampleColor), SIGMA_C) * gaussianWeight;
		sumWeight += weight;
		sumColor += sampleColor * weight;
	}
	sumColor = sumColor / sumWeight;

	sColor = vec4(sumColor, 1.0f);
}