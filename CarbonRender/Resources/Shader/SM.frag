#version 430

layout(location = 0) out vec4 posDepthColor;
layout(location = 1) out vec3 vplAlbedoColor;
layout(location = 2) out vec3 vplNormalColor;

in vec3 wsP;
in vec2 uv;
in float d;
in vec3 wsN;

uniform sampler2D albedoMap;

void main ()
{
	vec4 albedo = texture2D(albedoMap, uv);
	albedo.rgb = pow(albedo.rgb, vec3(2.2f));

	posDepthColor = vec4(wsP, d);
	vplAlbedoColor = albedo.xyz;
	vplNormalColor = wsN;
}