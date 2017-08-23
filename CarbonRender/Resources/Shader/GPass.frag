#version 430

layout(location = 0) out vec4 nColor;
layout(location = 1) out vec4 aColor;
layout(location = 2) out vec4 pColor;
layout(location = 3) out vec4 tColor;

in vec3 wsP;
in vec3 T;
in vec2 uv;

uniform sampler2D albedoMap;
uniform sampler2D msMap;
uniform sampler2D normalMap;

void main ()
{
	vec4 n = texture2D(normalMap, uv);
	vec4 ms = texture2D(msMap, uv);
	vec4 albedo = texture2D(albedoMap, uv);

	nColor = vec4(n.x, n.y, ms.x, ms.y);
	aColor = vec4(albedo.xyz, 1.0f);
	pColor = vec4(wsP.xyz, 1.0f);
	tColor = vec4(T.xyz, 1.0f);
}