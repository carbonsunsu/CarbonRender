#version 430
#define PI 3.14159265359f
#define SIGMA 10.0

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform vec4 steps;
uniform sampler2D shadowMap;
uniform sampler2D stenMap;

float GausianFacotr (vec2 uv, float s)
{
	return 0.5f * exp( -(uv.x*uv.x + uv.y*uv.y)/(2*s*s) ) / (PI*s*s);
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
	float sumFactor = 0.0f;
	float gFactor;
	vec2 sampleBias;
	
	gFactor = GausianFacotr(steps.zw * 0, SIGMA);
	sumFactor += gFactor;
	vec4 sMap = texture2D(shadowMap, uv);
	sumColor += sMap.rb * gFactor;

	for (int i = 1; i < 5; i++)
	{
		gFactor = GausianFacotr(steps.zw * i, SIGMA);
		sumFactor += gFactor;
		sumColor += texture2D(shadowMap, uv + steps.xy * i).rb * gFactor;

		gFactor = GausianFacotr(steps.zw * -i, SIGMA);
		sumFactor += gFactor;
		sumColor += texture2D(shadowMap, uv + steps.xy * -i).rb * gFactor;
	}
	sumColor = sumColor / sumFactor;

	sColor = vec4(sumColor.r, 0.0f, sumColor.g, 1.0f);
}