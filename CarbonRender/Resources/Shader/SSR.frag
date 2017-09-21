#version 430

layout(location = 0) out vec4 rColor;

in vec2 uv;

uniform sampler2D colorMap;
uniform sampler2D nMap;
uniform sampler2D pMap;
uniform sampler2D paraMap;
uniform samplerCube cubeMap;
uniform mat4 viewMat;
uniform mat4 projectMat;
uniform vec4 wsCamPos;

const float maxStepSize = 1.0f;
const int MaxStep = 16;
const float edgeFadingStart = 0.75f;

vec3 IndirectSpecular (samplerCube tex, vec3 r, float roughness)
{
	float s = roughness * (1.7f - 0.7f * roughness);
	float mip = roughness * 6;

	return textureLod(cubeMap, r, mip).rgb;
}

float Luminance(vec3 color)
{
	return dot(color, vec3(0.0396819152f, 0.458021790f, 0.00609653955f));
}

void main ()
{
	vec4 para = texture2D(paraMap, uv);
	vec4 wsP = texture2D(pMap, uv);
	float roughness = wsP.a;
	vec4 wsN = texture2D(nMap, uv);
	vec3 wsV = wsP.xyz - wsCamPos.xyz;
	wsV = normalize(wsV);
	vec3 wsR = reflect(wsV, wsN.xyz);
	wsR = normalize(wsR);

	vec4 reflection = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec3 v = vec3(0.0f, 0.0f, 0.0f);
	int stepCount = 0;
	float stepSize = maxStepSize;
	while (stepCount <= MaxStep)
	{
		 stepCount++;

		 v += wsR * stepSize;
		 vec4 ssP = projectMat * (viewMat * vec4(wsP.xyz + v, 1.0f));
		 ssP.xy = ssP.xy/ssP.w;
		 vec2 sampleUV = ssP.xy * 0.5f + 0.5f;
		 if (sampleUV.x > 1.0f || sampleUV.x < 0.0f || sampleUV.y > 1.0f || sampleUV.y < 0.0f) continue;
		 vec4 sampleN = texture2D(nMap, sampleUV);
		 if (abs(ssP.w) - sampleN.a > 0.0f)
		 {
			v -= wsR * stepSize;
			stepSize *= 0.5f;
			continue;
		 }
		 else if (abs(abs(ssP.w) - sampleN.a) <= 0.1f)
		 {
			float d = distance(v, vec3(0.0f));
			float fadingFactor = 1.0f;
			fadingFactor *= (0.9 - abs(ssP.x)) / (1.0f - edgeFadingStart);
			fadingFactor *= (0.9 - abs(ssP.y)) / (1.0f - edgeFadingStart);
			fadingFactor *=  1.0f - dot(wsN.xyz, sampleN.xyz);
			fadingFactor *= 1.0f / (d + 1.0f);
			fadingFactor = clamp(fadingFactor, 0.0f, 1.0f);
			reflection.rgb = textureLod(colorMap, sampleUV, roughness * 10.0f).rgb;
			reflection.a = 1.0f;
			reflection *= fadingFactor;
			break;
		 }
		 else
		 {
			stepSize = maxStepSize;
		 }
	}
	vec3 cube = IndirectSpecular(cubeMap, wsR, roughness);

	rColor.rgb = mix(cube, reflection.rgb, reflection.a) * para.rgb;
}