#version 430

layout(location = 0) out float depthMap;
layout(location = 1) out vec3 vplPosColor;
layout(location = 2) out vec3 vplAlbedoColor;
layout(location = 3) out vec3 vplNormalColor;

in vec3 wsP;
in vec2 uv;
in float d;
in vec3 wsN;

uniform sampler2D albedoMap;
uniform bool depthOnly;

void main ()
{
	vec4 albedo = texture2D(albedoMap, uv);
	albedo.rgb = pow(albedo.rgb, vec3(2.2f));

	depthMap = d;
	if (!depthOnly)
	{
		vplPosColor = wsP;
		vplAlbedoColor = albedo.xyz;
		vplNormalColor = wsN;
	}
}