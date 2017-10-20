#version 430

layout(location = 0) out vec4 rColor;

in vec2 uv;

uniform sampler2D pureLightMap;
uniform sampler2D refMap;
uniform sampler2D paraMap;
uniform sampler2D nMap;
uniform sampler2D pMap;
uniform sampler2D stenMap;
uniform mat4 viewMat;
uniform mat4 projectMat;
uniform vec4 wsCamPos;

const float maxStepSize = 1.0f;
const int MaxStep = 32;
const float edgeFadingStart = 0.75f;

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}

	vec4 para = texture2D(paraMap, uv);
	vec4 wsP = texture2D(pMap, uv);
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
			reflection.rgb = texture2D(pureLightMap, sampleUV).rgb + texture2D(refMap, sampleUV).rgb;
			reflection.a = fadingFactor;
			break;
		 }
		 else
		 {
			stepSize = maxStepSize;
		 }
	}

	rColor.rgb = reflection.rgb * para.rgb;
	rColor.a = reflection.a;
}