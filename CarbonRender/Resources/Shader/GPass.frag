#version 430

layout(location = 0) out vec4 aColor;
layout(location = 1) out vec4 nColor;
layout(location = 2) out vec4 pColor;
layout(location = 3) out vec4 sColor;
layout(location = 4) out vec4 eColor;

in vec4 wsP;
in vec2 uvD;
in vec2 uvN;
in vec2 uvS;
in vec4 vertexColor;
in vec3 N;
in vec3 T;
in vec3 B;

uniform vec4 albedoScaler;
uniform sampler2D albedoMap;
uniform sampler2D rmMap;
uniform sampler2D normalMap;
uniform float roughnessScale;
uniform float metallicScale;
uniform mat4 viewMat;
uniform mat3 normalMat;
uniform vec2 depthClampPara;
uniform vec3 emissionColor;

void main ()
{
	vec3 tsN = texture(normalMap, uvN).xyz;
	tsN = tsN * 2.0f - 1.0f;
	vec4 rm = texture(rmMap, uvS);
	vec4 albedo = texture(albedoMap, uvD);
	albedo.rgb = pow(albedo.rgb, vec3(2.2f));
	albedo.rgb *= albedoScaler.rgb;

	vec3 wsT = normalize(normalMat * T);
	vec3 wsN = normalize(normalMat * N);
	vec3 wsB = normalize(normalMat * B);
	mat3 TBN = mat3(wsT, wsB, wsN);
	wsN = TBN * tsN;
	wsN = normalize(wsN);

	float d = -(viewMat * wsP).z;

	aColor = vec4(albedo.xyz, min(rm.g * metallicScale, 1.0f));
	nColor = vec4(wsN.x, wsN.y, wsN.z, d);
	pColor = vec4(wsP.x, wsP.y, wsP.z, min(rm.r * roughnessScale, 1.0f));
	sColor = vec4(1,0,0,0);
	eColor = vec4(emissionColor, 0.0f);
}