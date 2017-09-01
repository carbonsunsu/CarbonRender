#version 430

in vec2 uv;

out vec4 fColor;

uniform sampler2D skyBuffer;
uniform sampler2D lightBuffer;

void main ()
{
	vec4 sBuffer = texture2D(skyBuffer, uv);
	vec4 lBuffer = texture2D(lightBuffer, uv);

	fColor.rgb = mix(sBuffer.rgb, lBuffer.rgb, lBuffer.a);
	fColor.a = 1.0f;
}