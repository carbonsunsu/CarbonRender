#version 430
#extension GL_NV_shadow_samplers_cube : enable
#define PI 3.14159265359f
#define PI_INV 0.31830988618f

#define StandardDielectricCoef vec4(0.04f, 0.04f, 0.04f, 0.96f)

layout(location = 0) out vec4 pureLColor;
layout(location = 1) out vec4 refColor;
layout(location = 2) out vec4 para;

in vec2 uv;

uniform vec4 zenithColor;
uniform vec4 sunColor;
uniform vec3 wsSunPos;
uniform vec3 wsCamPos;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D pMap;
uniform sampler2D sMap;
uniform sampler2D stenMap;
uniform samplerCube cubeMap;
uniform sampler2D giMap;

vec3 IndirectSpecular (samplerCube tex, vec3 r, float roughness)
{
	float s = roughness * (1.7f - 0.7f * roughness);
	float mip = roughness * 6;

	return textureLod(cubeMap, r, mip).rgb;
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
	pureLight = diff * (indDiff + lColor * diffTerm)
				+ specTerm * lColor * FresnelTerm (spec, LoH);
	indSpecPara = reduction * FresnelLerp (spec, grazingTerm.xxx, NoV);
}

float Luminance(vec3 color)
{
	return dot(color, vec3(0.0396819152f, 0.458021790f, 0.00609653955f));
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}
	
	vec4 albedo = texture2D(albedoMap, uv);
	vec4 N = texture2D(normalMap, uv);
	vec4 P = texture2D(pMap, uv);
	vec4 shadowFactor = texture2D(sMap, uv);
	vec4 gi = texture2D(giMap, uv);

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

	vec3 lightColor = sunColor.rgb * directShadow;
	vec3 diffColor;
	vec3 specColor;
	float oneMinusMetallic;
	GetDiffSpec(albedo.rgb, metallic, diffColor, specColor, oneMinusMetallic);
	vec3 indirectDiff = (gi.rgb + textureLod(cubeMap, wsN, 10).rgb * 1.0f) * sunColor.rgb;
	vec3 inditectSpec = IndirectSpecular (cubeMap, wsR, roughness);

	BRDF(diffColor, specColor, oneMinusMetallic, roughness, 
		wsN, wsV, wsL, lightColor, 
		indirectDiff, inditectSpec, pureLColor.rgb, para.rgb);

	para.rgb *= indirectShadow;
	refColor.rgb = inditectSpec * para.rgb;
	refColor.a = roughness;
}