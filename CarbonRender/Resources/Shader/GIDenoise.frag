#version 430
#define PI 3.14159265359f
#define SIGMA_D 10.0f
#define SIGMA_DEPTH 1.0f
#define SIGMA_N 1.0f

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform vec3 stepUnit;
uniform sampler2D stenMap;
uniform sampler2D giMap;
uniform sampler2D nMap;

float GetWeight (float dis, float s)
{
	return exp(-0.5f * pow(max(dis, 0.0f) / s, 2.0f)) / sqrt(2.0f * PI * s * s);
}

void main ()
{
	vec4 stencil = texture(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}

	vec3 sumColor = vec3(0.0f);
	float sumWeight = 0.0f;
	float weight;
	
	vec4 normalDepth = texture(nMap, uv);
	vec3 giColor = texture(giMap, uv).rgb;
	weight = GetWeight(0.0f, SIGMA_D) * GetWeight(0.0f, SIGMA_N);
	sumWeight += weight;
	sumColor += giColor * weight;

	for (int i = 1; i < 9; i++)
	{
		vec4 sampleUV;
		sampleUV.xy = uv + stepUnit.xy * i;
		sampleUV.zw = uv + stepUnit.xy * -i;
		vec4 normalDepth0 = texture(nMap, sampleUV.xy);
		vec4 normalDepth1 = texture(nMap, sampleUV.zw);
		
		float gaussianWeight = GetWeight(length(stepUnit.z * i), SIGMA_D);
		vec3 sampleColor = texture(giMap, sampleUV.xy).rgb;
		weight = gaussianWeight;
		weight *= GetWeight(distance(normalDepth0.xyz, normalDepth.xyz), SIGMA_N);
		weight *= GetWeight(abs(normalDepth0.a - normalDepth.a), SIGMA_DEPTH);
		sumWeight += weight;
		sumColor += sampleColor * weight;

		sampleColor = texture(giMap, sampleUV.zw).rgb;
		weight = gaussianWeight;
		weight *= GetWeight(distance(normalDepth1.xyz, normalDepth.xyz), SIGMA_N);
		weight *= GetWeight(abs(normalDepth1.a - normalDepth.a), SIGMA_DEPTH);
		sumWeight += weight;
		sumColor += sampleColor * weight;
	}
	sumColor = sumColor / sumWeight;

	sColor = vec4(sumColor, 1.0f);
}