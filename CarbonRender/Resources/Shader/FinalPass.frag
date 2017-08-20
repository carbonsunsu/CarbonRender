#version 430

in vec2 uv;

out vec4 fColor;

uniform sampler2D skyRT;

void main ()
{
	fColor = texture2D(skyRT, uv);
}