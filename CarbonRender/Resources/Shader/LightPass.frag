#version 430
#extension GL_NV_shadow_samplers_cube : enable

layout(location = 0) out vec4 lColor;

in vec2 uv;

uniform vec4 zenithColor;
uniform vec4 sunColor;
uniform vec3 wsSunPos;
uniform vec3 wsCamPos;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D pMap;
uniform sampler2D sMap;
uniform samplerCube cubeMap;

vec3 IndirectSpecular (samplerCube tex, vec3 r, float smoothness)
{
	float s = smoothness * (1.7f - 0.7f * smoothness);
	float mip = smoothness * 6;

	return textureLod(cubeMap, r, mip).rgb;
}

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
	vec3 wsE = wsCamPos - wsP;
	wsE = normalize(wsE);
	vec3 wsR = reflect(-wsE, wsN);
	
	float NoL = clamp(dot(wsN, wsL), 0.0f, 1.0f);
	float NoU = clamp(dot(wsN, vec3(0.0f, 1.0f, 0.0f)), 0.0f, 1.0f);

	lColor.rgb = (NoL * sunColor * albedo * sFactor + NoU * zenithColor * albedo).rgb;
	lColor.a = albedo.a;
}