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
const int MaxStep = 512;

vec3 IndirectSpecular (samplerCube tex, vec3 r, float roughness)
{
	float s = roughness * (1.7f - 0.7f * roughness);
	float mip = roughness * 6;

	return textureLod(cubeMap, r, mip).rgb;
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
		 vec2 sampleUV = ssP.xy/ssP.w;
		 sampleUV = sampleUV * 0.5f + 0.5f;
		 if (sampleUV.x > 1.0f || sampleUV.x < 0.0f || sampleUV.y > 1.0f || sampleUV.y < 0.0f) continue;
		 float sampleD = texture2D(nMap, sampleUV).a;
		 if (abs(ssP.w) - sampleD > 0.0f)
		 {
			v -= wsR * stepSize;
			stepSize *= 0.5f;
			continue;
		 }
		 else if (abs(abs(ssP.w) - sampleD) <= 0.1f)
		 {
			reflection.rgb = textureLod(colorMap, sampleUV, roughness * 6.0f).rgb;
			reflection.a = 1.0f;
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