#version 430

layout(location = 0) out vec4 lColor;

in vec2 uv;

uniform vec4 zenithColor;
uniform vec4 sunColor;
uniform vec3 wsSunPos;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D sMap;

void main ()
{
	vec4 albedo = texture2D(albedoMap, uv);
	vec4 N = texture2D(normalMap, uv);
	N = N * 2.0f - 1.0f;
	vec4 S = texture2D(sMap, uv);

	vec3 wsN = normalize(N.xyz);
	vec3 wsP = vec3(N.a, S.b, S.a);
	float metal = S.r;
	float smoothness = S.g;
	
	vec3 wsL = wsSunPos;
	wsL = normalize(wsL);
	float NoL = clamp(dot(wsN, wsL), 0.0f, 1.0f);

	lColor.xyz = vec3(NoL);// * zenithColor;
	lColor.a = albedo.a;
}