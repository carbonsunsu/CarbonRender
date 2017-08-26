#version 430

layout(location = 0) out vec4 lColor;

in vec2 uv;

uniform vec4 zenithColor;
uniform vec4 sunColor;
uniform vec3 wsSunPos;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D pMap;
uniform sampler2D sMap;

void main ()
{
	vec4 albedo = texture2D(albedoMap, uv);
	vec4 N = texture2D(normalMap, uv);
	vec4 P = texture2D(pMap, uv);
	float sFactor = texture2D(sMap, uv).x;

	vec3 wsN = normalize(N.xyz);
	vec3 wsP = P.xyz;
	float metal = N.a;
	float smoothness = P.a;
	
	vec3 wsL = wsSunPos;
	wsL = normalize(wsL);
	float NoL = clamp(dot(wsN, wsL), 0.0f, 1.0f);
	float NoU = clamp(dot(wsN, vec3(0.0f, 1.0f, 0.0f)), 0.0f, 1.0f);

	lColor = NoL * sunColor * albedo * sFactor + NoU * zenithColor * albedo;
	lColor.a = albedo.a;
}