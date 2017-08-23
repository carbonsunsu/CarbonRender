#version 430

in vec2 uv;

out vec4 fColor;

uniform sampler2D skyRT;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D wsPMap;
uniform sampler2D tMap;

void main ()
{
	vec4 albedo = texture2D(albedoMap, uv);
	vec4 n = texture2D(normalMap, uv);
	vec4 wsP = texture2D(wsPMap, uv);
	vec4 skyColor = texture2D(skyRT, uv) * (1.0f - albedo.a);

	fColor = skyColor + wsP;
	fColor.a = 1.0f;
}