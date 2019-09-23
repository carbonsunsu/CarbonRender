#version 430

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform sampler2D pMap;
uniform sampler2D smMap;
uniform sampler2D stenMap;
uniform mat4 smViewMat;
uniform mat4 smProMat;
uniform vec2 stepUnit;
uniform float depthClampPara;

const int rad = 1;

float Random(vec3 p, float seed)
{
	float a = 12.9898f;
    float b = 78.233f;
    float c = 43758.5453f;
    float dt= dot(p.xy * seed,vec2(a,b));
    float sn= mod(dt, 3.14f);
    return fract(sin(sn) * c);
}

bool IsInScreen (vec2 ssPos)
{
	return ssPos.x >= 0.0f && ssPos.x <= 1.0f && ssPos.y >= 0.0f && ssPos.y <= 1.0f;
}

void main ()
{
	vec4 stencil = texture2D(stenMap, uv);
	if (stencil.r <= 0)
	{
		discard;
		return;
	}
	
	vec4 pMapSample = texture2D(pMap, uv);
	vec3 wsPos = pMapSample.xyz;
	vec4 smPos = smViewMat * vec4(wsPos, 1.0f);
	float d = -smPos.z;
	smPos = smProMat * smPos;
	vec2 smUV = smPos.xy * 0.5f + 0.5f;
	
	float sFactor = 1.0f;
	if (IsInScreen(smUV))
	{	
		float occluderDepth = texture2D(smMap, smUV).a;
		float multiSampleDepth = occluderDepth;
		
		
		int sampleCount = 4;
		for (int i = 0; i < sampleCount; i++)
		{
			vec2 random = vec2(Random(wsPos, i * 10.0f), Random(wsPos, i * 20.0f));
			vec2 sampleUV = smUV + random * stepUnit * rad;
			if (IsInScreen(sampleUV))
				multiSampleDepth += texture2D(smMap, sampleUV).a;
			else
				multiSampleDepth += occluderDepth;
		}
		multiSampleDepth /= (sampleCount + 1);

		sFactor = multiSampleDepth - d + 0.1f < 0.0f ? 0.0f : 1.0f;
		//sFactor = exp(1.0f * (multiSampleDepth - d));
	}

	float lightSize = 10.0f;
	float searchR = 10.0f - 1.0f / abs(d);

	sColor = vec4(sFactor, d, 1.0f, 1.0f);
}