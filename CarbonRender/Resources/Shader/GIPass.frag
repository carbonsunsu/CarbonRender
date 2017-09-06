#version 430
#define PI 3.14159265359f

layout(location = 0) out vec4 giColor;

in vec2 uv;

uniform sampler2D pMap;
uniform sampler2D nMap;
uniform sampler2D vplPMap;
uniform sampler2D vplAMap;
uniform sampler2D vplNMap;
uniform mat4 smMat;
uniform vec2 scale;

const float vplLod = 5.5f;
const int r = 100;
const int sampleNum = 32;
const float random0[32] = {59, -47, 2, 92, -48, 76, -90, 68, -93, 86, -21, 67, -19, 23, -6, 16, -40, -25, 1, 56, -28, 100, 50, -83, 49, -98, 37, 55, -29, 53, -91, 10};
const float random1[32] = {-56, 67, 1, -37, 17, -70, 52, -89, 57, -98, 46, 59, 83, 28, -43, -58, 19, 61, 72, -41, 39, 23, -71, 80, 20, 25, -22, 94, -26, 12, 65, -76};

void main ()
{
	vec3 wsP = texture2D(pMap, uv).xyz;
	vec3 wsN = texture2D(nMap, uv).xyz;
	vec3 smP = (smMat * vec4(wsP, 1.0f)).xyz;
	float d = smP.z;
	vec2 smUV = smP.xy * 0.5f + 0.5f;
	vec3 gi = vec3(0.0f, 0.0f, 0.0f);
	vec2 sampleR = scale * r;

	for (int i = 1; i < sampleNum; i++)
	{
		float r0 = random0[i];
		float r1 = random1[i];
		vec2 vplUv = smUV + vec2(sampleR.x*r0*0.01f*sin(2*PI*r1*0.1f), sampleR.y*r1*0.01f*cos(2*PI*r0*0.1f));
		vec3 vplPos = textureLod(vplPMap, vplUv, vplLod).xyz;
		vec3 vplColor = textureLod(vplAMap, vplUv, vplLod).xyz;
		vec3 vplN = textureLod(vplNMap, vplUv, vplLod).xyz;
		vec3 v = normalize(wsP - vplPos);
		float d = distance(wsP, vplPos);

		if (d > 1.0f)
			gi += vplColor * max(0.0f, dot(vplN, v)) * max(0.0f, dot(wsN, -v)) / (d*d*d*d);
	}

	giColor = vec4(gi, 1.0f);
}