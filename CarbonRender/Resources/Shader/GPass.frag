#version 430

layout(location = 0) out vec4 aColor;
layout(location = 1) out vec4 nColor;
layout(location = 2) out vec4 pColor;
layout(location = 3) out vec4 sColor;

in vec3 wsP;
in vec2 uv;
in mat3 TBN;
in float d;

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
	albedo.rgb = pow(albedo.rgb, vec3(2.2f));

	vec3 wsN = normalMat * TBN * N;

	wsN = normalize(wsN);

	aColor = vec4(albedo.xyz, ms.g);
	nColor = vec4(wsN.x, wsN.y, wsN.z, d);
	pColor = vec4(wsP.x, wsP.y, wsP.z, ms.r);
	sColor = vec4(1,0,0,0);
}