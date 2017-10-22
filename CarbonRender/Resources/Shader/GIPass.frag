#version 430
#define PI 3.14159265359f

layout(location = 0) out vec4 giColor;

in vec2 uv;

uniform sampler2D pMap;
uniform sampler2D nMap;
uniform sampler2D vplPMap;
uniform sampler2D vplAMap;
uniform sampler2D vplNMap;
uniform sampler2D stenMap;
uniform mat4 smMat;
uniform vec2 stepUnit;

const float vplLod = 5.5f;
const int r = 50;
const int sampleNum = 8;

float Random(vec3 p, float seed)
{
	float a = 12.9898f;
    float b = 78.233f;
    float c = 43758.5453f;
    float dt= dot(p.xy * seed,vec2(a,b));
    float sn= mod(dt, 3.14f);
    return fract(sin(sn) * c);
}

vec3 SampleVPL (vec2 uv, vec3 wsP, vec3 wsN)
{
	vec3 vplPos = textureLod(vplPMap, uv, vplLod).xyz;
	vec3 vplColor = texture2D(vplAMap, uv).xyz;
	vec3 vplN = textureLod(vplNMap, uv, vplLod).xyz;
	vec3 v = normalize(wsP - vplPos);
	float d = max(distance(wsP, vplPos), 1.0f);

	float intensity = max(0.0f, dot(vplN, v)) * max(0.0f, dot(wsN, -v)) / (d*d);
	return intensity * vplColor;
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}

	vec3 wsP = texture2D(pMap, uv).xyz;
	vec3 wsN = texture2D(nMap, uv).xyz;
	vec3 smP = (smMat * vec4(wsP, 1.0f)).xyz;
	vec2 smUV = smP.xy * 0.5f + 0.5f;
	vec3 gi = vec3(0.0f, 0.0f, 0.0f);
	vec2 sampleR = stepUnit * r;
	int sampleNum2 = sampleNum * sampleNum;

	for (int i = 0; i < sampleNum2; i++)
	{
		float lx = floor(i / sampleNum) / sampleNum * 2.0f - 1.0f;
		float ly = mod(i, sampleNum) / sampleNum * 2.0f - 1.0f;
		vec2 subl = sampleR * 2.0f / sampleNum;
		float rx = Random(wsP, i * 20.0f) * subl.x;
		float ry = Random(wsP, i * 10.0f) * subl.y;
		vec2 vplUv = smUV + vec2(sampleR.x * lx + rx, sampleR.y * ly + ry);

		gi += SampleVPL(vplUv, wsP, wsN);
	}

	giColor = vec4(gi / sampleNum, 1.0f);
}