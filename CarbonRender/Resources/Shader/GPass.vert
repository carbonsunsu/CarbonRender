#version 430

layout(location = 0) in vec4 msPos;
layout(location = 1) in vec3 msN;
layout(location = 2) in vec3 msT;
layout(location = 3) in vec4 uvs;
layout(location = 4) in vec4 vColor;
layout(location = 5) in vec3 msB;

uniform mat4 modelMat;
uniform mat3 normalMat;
uniform mat4 viewMat;
uniform mat4 proMat;

out vec4 wsP;
out vec2 uv;
out mat3 TBN;
out vec4 vertexColor;

void main ()
{
	wsP = modelMat * msPos;
	gl_Position = proMat * viewMat * wsP;
	vec3 T = normalize(normalMat * msT);
	vec3 N = normalize(normalMat * msN);
	vec3 B = normalize(normalMat * msB);
	TBN = mat3(T, B, N);
	
	uv = uvs.xy;
	vertexColor = vColor;
}