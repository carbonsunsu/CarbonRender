#version 430

layout(location = 0) in vec4 msPos;
layout(location = 1) in vec3 msN;
layout(location = 2) in vec3 msT;
layout(location = 3) in vec4 uvs;
layout(location = 4) in vec4 vColor;
layout(location = 5) in vec3 msB;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 proMat;
uniform mat3 normalMat;
uniform vec2 uvTilling0;

out vec4 wsP;
out vec2 uvD;
out vec3 wsN;

void main ()
{
	wsP = modelMat * msPos;
	gl_Position = proMat * viewMat * wsP;

	wsN = normalize(normalMat * msN);
	uvD = uvs.xy * uvTilling0.xy;
}