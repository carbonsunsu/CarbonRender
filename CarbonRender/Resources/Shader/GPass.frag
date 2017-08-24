#version 430

layout(location = 0) out vec4 aColor;
layout(location = 1) out vec4 nColor;
layout(location = 2) out vec4 pColor;

in vec3 wsP;
in vec2 uv;
in mat3 TBN;
in vec3 n;

uniform mat3 normalMat;
uniform sampler2D albedoMap;
uniform sampler2D msMap;
uniform sampler2D normalMap;

void main ()
{
	vec3 N = texture2D(normalMap, uv).xyz;
	N = N * 2.0f - 1.0f;
	vec4 ms = texture2D(msMap, uv);
	vec4 albedo = texture2D(albedoMap, uv);
	albedo = albedo * (albedo * (albedo * 0.305306011f + 0.682171111f) + 0.012522878f);

	vec3 wsN = normalMat * TBN * N;

	wsN = normalize(wsN);

	aColor = vec4(albedo.xyz, 1.0f);
	nColor = vec4(wsN.x, wsN.y, wsN.z, ms.g);
	pColor = vec4(wsP.x, wsP.y, wsP.z, ms.r);
}