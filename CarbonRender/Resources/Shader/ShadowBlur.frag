#version 430
#define PI 3.14159265359f
#define SIGMA 10.0

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform vec4 steps;
uniform sampler2D shadowMap;

float GausianFacotr (vec2 uv, float s)
{
	return 0.5f * exp( -(uv.x*uv.x + uv.y*uv.y)/(2*s*s) ) / (PI*s*s);
}

void main ()
{
	float sumColor = 0.0f;
	float sampleColor;
	float sumFactor = 0.0f;
	float gFactor;
	vec2 sampleBias;
	
	gFactor = GausianFacotr(steps.zw * 0, SIGMA);
	sumFactor += gFactor;
	vec4 sMap = texture2D(shadowMap, uv);
	sumColor += sMap.r * gFactor;

	for (int i = 1; i < 5; i++)
	{
		gFactor = GausianFacotr(steps.zw * i, SIGMA);
		sumFactor += gFactor;
		sumColor += texture2D(shadowMap, uv + steps.xy * i).r * gFactor;

		gFactor = GausianFacotr(steps.zw * -i, SIGMA);
		sumFactor += gFactor;
		sumColor += texture2D(shadowMap, uv + steps.xy * -i).r * gFactor;
	}

	sColor = vec4(sumColor.x / sumFactor, 0.0f, sMap.b, 1.0f);
}