#version 430
#extension GL_NV_shadow_samplers_cube : enable
#define PI 3.14159265359f
#define PI_INV 0.31830988618f

#define StandardDielectricCoef vec4(0.04f, 0.04f, 0.04f, 0.96f)

layout(location = 0) out vec4 pureLColor;
layout(location = 1) out vec4 refColor;
layout(location = 2) out vec4 para;

in vec2 uv;

uniform vec3 sunColor;
uniform vec3 wsSunPos;
uniform vec3 wsCamPos;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D pMap;
uniform sampler2D sMap;
uniform sampler2D stenMap;
uniform samplerCube cubeMap;
uniform sampler2D giMap;

float Random (vec2 i, float seed)
{
	//golden noise ranged from -1 to 1
	float phi = 1.61803398874989484820459 * 00000.1; // Golden Ratio   
	float pi  = 3.14159265358979323846264 * 00000.1; // PI
	float srt = 1.41421356237309504880169 * 10000.0; // Square Root of Two

	return fract(tan(distance(i * (seed + phi), vec2(phi, pi))) * srt) * 2.0f - 1.0f;
}

vec3 Random (vec3 p, float seed)
{
	float x = Random(p.xy, seed * 10.0f);
	float y = Random(p.yz, -seed * 20.0f);
	float z = Random(p.zx, seed * 30.0f);
	return vec3(x,y,z);
}

vec3 ConeTracingCube (vec3 uvw, float roughness)
{
	return texture(cubeMap, vec3(uvw.x, max(uvw.y, 0.0f), uvw.z), roughness * 10.0f).rgb;/*
	vec3 ambientColor = vec3(0.0f);
	for (int i = 0; i < 4; i++)
	{
		vec3 wsNoiseR = normalize(Random(uvw, i * 10.0f + 10.0f));
		wsNoiseR = uvw + wsNoiseR * roughness;
		wsNoiseR = normalize(wsNoiseR);
		ambientColor += texture(cubeMap, wsNoiseR).rgb;
	}
	ambientColor *= 0.25f;

	return ambientColor;*/
}

void GetDiffSpec (vec3 albedo, float metallic, out vec3 diffColor, out vec3 specColor, out float oneMinusMetallic)
{
	specColor = mix(StandardDielectricCoef.rgb, albedo, metallic);
	oneMinusMetallic = StandardDielectricCoef.a - StandardDielectricCoef.a * metallic;
	diffColor = albedo * oneMinusMetallic;
}

float DisneyDiffuse(float NoV, float NoL, float LoH, float roughness)
{
    float fd90 = 0.5 + 2 * LoH * LoH * roughness;
    float lightScatter   = (1 + (fd90 - 1) * pow(1 - NoL, 5));
    float viewScatter    = (1 + (fd90 - 1) * pow(1 - NoV, 5));

    return lightScatter * viewScatter;
}

float GGXV(float NoL, float NoV, float roughness)
{
	float a = roughness;
    float a2	= a * a;

    float V = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    float L = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);

    return 0.5f / (V + L + 0.00001f);
}

float GGX(float NoH, float roughness)
{
	float a2 = roughness * roughness;
    float d = (NoH * a2 - NoH) * NoH + 1.0f;
    return PI_INV * a2 / (d * d + 0.0000001f);
}

vec3 FresnelTerm (vec3 F0, float cosA)
{
    float t = pow(1.0f - cosA, 5.0f);
    return F0 + (1.0f - F0) * t;
}

vec3 FresnelLerp (vec3 F0, vec3 F90, float cosA)
{
    float t = pow(1.0f - cosA, 5.0f);
    return mix(F0, F90, t);
}

void BRDF(vec3 diff, vec3 spec, float oneMinusMetallic, float roughness, 
			vec3 wsN, vec3 wsV, vec3 wsL, 
			vec3 lColor, vec3 indDiff, vec3 indSpec,
			out vec3 pureLight, out vec3 indSpecPara)
{
	vec3 wsH = normalize(wsV + wsL);

	float shift = dot(wsN, wsV);
	wsN = shift < 0.0f ? wsN + wsV * (-shift + 0.00001f) : wsN;
	wsN = normalize(wsN);
	float NoV = clamp(dot(wsN, wsV), 0.0f, 1.0f);
	float NoL = clamp(dot(wsN, wsL), 0.0f, 1.0f);
	float NoH = clamp(dot(wsN, wsH), 0.0f, 1.0f);
	float LoV = clamp(dot(wsL, wsV), 0.0f, 1.0f);
	float LoH = clamp(dot(wsL, wsH), 0.0f, 1.0f);

	float diffTerm = DisneyDiffuse(NoV, NoL, LoH, roughness) * NoL;

	float roughness2 = roughness * roughness;
	float V = GGXV(NoL, NoV, roughness2);
	float D = GGX(NoH, roughness2);
	float specTerm = D*V*PI;
	specTerm = max(0.0f, specTerm*NoL);
	specTerm *= spec.x > 0.0f || spec.y > 0.0f || spec.z > 0.0f ? 1.0f : 0.0f;
	float reduction = 1.0f / (roughness2*roughness2 + 1.0f); 

	float grazingTerm = clamp(1.0f - roughness + (1-oneMinusMetallic), 0.0f, 1.0f);
	pureLight = diff * indDiff + lColor * diffTerm * diff
				+ FresnelTerm (spec, LoH) * (specTerm * lColor + indSpec);
	indSpecPara = reduction * FresnelLerp (spec, grazingTerm.xxx, NoV);
}

float Luminance(vec3 color)
{
	return dot(color, vec3(0.0396819152f, 0.458021790f, 0.00609653955f));
}

void main ()
{
	vec4 stencil = texture(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}
	
	vec4 albedo = texture(albedoMap, uv);
	vec4 N = texture(normalMap, uv);
	vec4 P = texture(pMap, uv);
	vec4 shadowFactor = texture(sMap, uv);
	vec4 gi = texture(giMap, uv);

	vec3 wsN = N.xyz;
	vec3 wsP = P.xyz;
	float metallic = albedo.a;
	float roughness = P.a;
	float directShadow = shadowFactor.r;
	float indirectShadow = shadowFactor.b;
	
	vec3 wsL = wsSunPos;
	wsL = normalize(wsL);
	vec3 wsV = wsCamPos - wsP;
	wsV = normalize(wsV);
	vec3 wsR = reflect(-wsV, wsN);
	
	float NoL = clamp(dot(wsN, wsL), 0.0f, 1.0f);
	float NoU = clamp(dot(wsN, vec3(0.0f, 1.0f, 0.0f)), 0.0f, 1.0f);

	vec3 lightColor = sunColor.rgb * 1.5f * directShadow;
	vec3 diffColor;
	vec3 specColor;
	float oneMinusMetallic;
	GetDiffSpec(albedo.rgb, metallic, diffColor, specColor, oneMinusMetallic);
	
	vec3 inditectSpec = ConeTracingCube(wsR, roughness) * indirectShadow;
	vec3 indirectDiff = ConeTracingCube(wsN, 1.0f) * indirectShadow;
	indirectDiff += gi.rgb * indirectShadow;

	BRDF(diffColor, specColor, oneMinusMetallic, roughness, 
		wsN, wsV, wsL, lightColor, 
		indirectDiff, inditectSpec, pureLColor.rgb, para.rgb);

	refColor.rgb = inditectSpec * para.rgb;
	refColor.a = roughness;
}