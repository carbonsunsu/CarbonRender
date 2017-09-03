#version 430
#define PI 3.14159265359f
#define PI_INV 0.31830988618f

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform sampler2D nMap;
uniform sampler2D pMap;
uniform sampler2D sMap;
uniform sampler2D rnMap;
uniform vec3 scalePara;
uniform mat3 viewMat;

const int minRadius = 1;
const int maxRadius = 10;
const float scale = 0.5f;
const float k = 3.0f;
const float bias = 0.0001;
const float epsilon = 0.01;
const int sampleNum = 12;
const vec3 vSphere[12] = vec3[](vec3(-0.010735935, 0.01647018, 0.0062425877),
			                     vec3(-0.06533369, 0.3647007, -0.13746321),
								 vec3(-0.6539235, -0.016726388, -0.53000957),
								 vec3(0.40958285, 0.0052428036, -0.5591124),
								 vec3(-0.1465366, 0.09899267, 0.15571679),
								 vec3(-0.44122112, -0.5458797, 0.04912532),
								 vec3(0.03755566, -0.10961345, -0.33040273),
								 vec3(0.019100213, 0.29652783, 0.066237666),
								 vec3(0.8765323, 0.011236004, 0.28265962),
								 vec3(0.29264435, -0.40794238, 0.15964167),
								 vec3(-0.1465366, 0.0052428036, -0.6539235),
								 vec3(0.019100213, -0.09899267, 0.5591124));

void main ()
{
	vec2 rnUV = uv * 20.0f;
	vec4 shadow = texture2D(sMap, uv);
	vec4 wsN = texture2D(nMap, uv);
	vec3 rn = texture2D(rnMap, uv).rgb;
	vec3 wsPos = texture2D(pMap, uv).rgb;

	float aoFactor = 0.0f;
	float s = scale * PI_INV;
	float r = mix(maxRadius, minRadius, wsN.a / scalePara.z);

	for (int i = 0; i < sampleNum; i++)
	{
		vec2 ray = normalize(reflect(vSphere[i], rn).xy) * vec2(r * scalePara.x, r * scalePara.y);
		vec3 samplePos = texture2D(pMap, uv + sign(dot(ray, wsN.xy)) * ray).rgb;
		vec3 v = samplePos - wsPos;

		aoFactor += max(0.0f, dot(wsN.xyz, v) - bias)/(dot(v, v) + epsilon);
	}
	aoFactor = 1.0f - 2 * s / sampleNum * aoFactor;
	aoFactor = pow(aoFactor, k);
	aoFactor = max(0.5f, aoFactor);

	sColor = vec4(shadow.r, 0.0f, aoFactor, 1.0f);
}