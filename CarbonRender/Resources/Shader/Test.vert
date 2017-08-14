#version 430

layout(location = 0) in vec4 msPos;
layout(location = 1) in vec3 msN;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 proMat;

varying vec4 wsN;

void main ()
{
	gl_Position = proMat * (viewMat * (modelMat * msPos));
}