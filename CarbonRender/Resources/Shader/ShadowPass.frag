#version 430

layout(location = 0) out vec4 sColor;

in vec2 uv;

uniform sampler2D pMap;
uniform sampler2D smMap;
uniform mat4 smMat;

uniform sampler2D albedoMap;

void main ()
{
	vec4 P = texture2D(pMap, uv);
	vec3 wsP = P.xyz;
	vec4 smP = smMat * vec4(wsP, 1.0f);
	float d = smP.z;
	vec2 smUV = smP.xy * 0.5f + 0.5f;
	float sFactor = 1.0f;
	if (smUV.x >= 0.0f && 
		smUV.x <= 1.0f &&
		smUV.y >= 0.0f &&
		smUV.y <= 1.0f )
	{	
		vec4 sMap = texture2D(smMap, smUV);
		float smD = sMap.a;
		sFactor = exp(20000.0f * (smD - d));
	}

	float lightSize = 10.0f;
	float searchR = 10.0f - 1.0f / abs(d);

	sColor = vec4(sFactor, 0.0f, 1.0f, 1.0f);
}