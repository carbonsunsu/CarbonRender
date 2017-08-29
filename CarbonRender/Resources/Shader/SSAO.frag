#version 430

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform sampler2D nMap;
uniform sampler2D pMap;
uniform sampler2D sMap;
uniform sampler2D rnMap;
uniform float scalePara;

const float maxDiff = 0.02;
const float minDiff = 0.001;
const float rad = 0.05;
const float offset = 18.0;
const vec3 vSphere[10] = vec3[](vec3(-0.010735935, 0.01647018, 0.0062425877),
			                     vec3(-0.06533369, 0.3647007, -0.13746321),
								 vec3(-0.6539235, -0.016726388, -0.53000957),
								 vec3(0.40958285, 0.0052428036, -0.5591124),
								 vec3(-0.1465366, 0.09899267, 0.15571679),
								 vec3(-0.44122112, -0.5458797, 0.04912532),
								 vec3(0.03755566, -0.10961345, -0.33040273),
								 vec3(0.019100213, 0.29652783, 0.066237666),
								 vec3(0.8765323, 0.011236004, 0.28265962),
								 vec3(0.29264435, -0.40794238, 0.15964167));
#define SAMPLE_NUM 10

void main ()
{
	vec4 shadow = texture2D(sMap, uv);
	vec4 N = texture2D(nMap, uv);
	vec3 rn = texture2D(rnMap, uv * 18.0f).rgb;
	vec3 wsPos = texture2D(pMap, uv).rgb;

	float aoFactor = 1.0f;
	rn = rn * 2.0f - 1.0f;
	float depth = N.a;

	vec3 ray;
	for (int i = 0; i < SAMPLE_NUM; i++)
	{
		ray = rad * reflect(vSphere[i], rn);
		vec4 oSample = texture2D(nMap, uv + sign(dot(ray, N.xyz)) * ray.xy);
		float depthDiff = depth - oSample.a;
		aoFactor += (1.0f - dot(oSample.xyz, N.xyz)) * step(minDiff, depthDiff) * (1.0f - smoothstep(minDiff, maxDiff, depthDiff));
	}
	aoFactor = 1.0f - aoFactor*0.138f;

	sColor = vec4(shadow.r, 0.0f, aoFactor, 1.0f);
}