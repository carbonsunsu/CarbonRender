#version 430

layout(location = 0) out float depthMap;
layout(location = 1) out vec3 vplPosColor;
layout(location = 2) out vec3 vplAlbedoColor;
layout(location = 3) out vec3 vplNormalColor;

in vec4 wsP;
in vec2 uv;
in vec3 wsN;

uniform vec4 albedoScaler;
uniform sampler2D albedoMap;
uniform bool depthOnly;
uniform mat4 viewMat;
uniform vec2 depthClampPara;

void main ()
{
	vec4 albedo = texture(albedoMap, uv);
	albedo.rgb = pow(albedo.rgb, vec3(2.2f));
	albedo.rgb *= albedoScaler.rgb;

	float d = -(viewMat * wsP).z;
	d = d * depthClampPara.y;
	d = clamp(d, 0.0f, 1.0f);
	depthMap = d;
	if (!depthOnly)
	{
		vplPosColor = wsP.xyz;
		vplAlbedoColor = albedo.xyz;
		vplNormalColor = wsN;
	}
}