#version 430

layout(location = 0) in vec4 ssPos;
layout(location = 3) in vec2 uvs;

out vec2 uv;

void main ()
{
	gl_Position = ssPos;
	uv = uvs.xy;
}