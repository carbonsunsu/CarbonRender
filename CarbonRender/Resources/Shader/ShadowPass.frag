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
	float d = smP.z / smP.w;
	//d -= 0.00003f;
	vec2 smUV = smP.xy * 0.5f + 0.5f;
	float smD = texture2D(smMap, smUV).a;
	float sFactor = 1.0f;
	if (smUV.x >= 0.0f && 
		smUV.x <= 1.0f &&
		smUV.y >= 0.0f &&
		smUV.y <= 1.0f )
		sFactor = exp(200.0f * (smD - d));

	sColor = sFactor.xxxx;
}