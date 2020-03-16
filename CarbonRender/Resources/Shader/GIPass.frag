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
uniform mat4 smViewMat;
uniform mat4 smProMat;
uniform vec2 stepUnit;
uniform vec3 sunColor;

const float vplLod = 0.0f;
const int r = 100;
const int sampleUnitCount = 4;

float Random (vec2 i, float seed)
{
	//golden noise ranged from -1 to 1
	float phi = 1.61803398874989484820459 * 00000.1; // Golden Ratio   
	float pi  = 3.14159265358979323846264 * 00000.1; // PI
	float srt = 1.41421356237309504880169 * 10000.0; // Square Root of Two

	return fract(tan(distance(i * (seed + phi), vec2(phi, pi))) * srt) * 2.0f - 1.0f;
}

bool IsInScreen (vec2 ssPos)
{
	return ssPos.x >= 0.0f && ssPos.x <= 1.0f && ssPos.y >= 0.0f && ssPos.y <= 1.0f;
}

vec3 SampleVPL (vec2 uv, vec3 wsP, vec3 wsN)
{
	if (!IsInScreen(uv))
		return vec3(0.0f, 0.0f, 0.0f);

	vec3 vplPos = texture2D(vplPMap, uv).xyz;
	vec3 vplColor = texture2D(vplAMap, uv).xyz;
	vec3 vplN = texture2D(vplNMap, uv).xyz;
	vec3 v = normalize(wsP - vplPos);
	float d = max(distance(wsP, vplPos), 1.0f);

	float intensity = max(0.0f, dot(vplN, v)) * max(0.0f, dot(wsN, -v)) / (d*d);
	return intensity * vplColor * sunColor * 4.0f;
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
	vec3 smP = (smProMat * (smViewMat * vec4(wsP, 1.0f))).xyz;
	vec2 smUV = smP.xy * 0.5f + 0.5f;
	vec3 gi = vec3(0.0f, 0.0f, 0.0f);
	vec2 sampleR = stepUnit * r;
	int sampleCountSum = sampleUnitCount * 5;
	float unitCountInv = 1.0f / sampleUnitCount;

	for (int i = 0; i < sampleCountSum; i++)
	{
		float lx = floor(i * unitCountInv) * unitCountInv * 2.0f - 1.0f;
		float ly = mod(i, sampleUnitCount) * unitCountInv * 2.0f - 1.0f;
		vec2 subl = sampleR * unitCountInv;
		float rx = Random(smUV, i * 20.0f) * subl.x;
		float ry = Random(smUV, -i * 5.0f) * subl.y;
		vec2 vplUv = smUV + vec2(sampleR.x * lx + rx, sampleR.y * ly + ry);

		gi += SampleVPL(vplUv, wsP, wsN);
	}

	giColor = vec4(gi, 1.0f);
}