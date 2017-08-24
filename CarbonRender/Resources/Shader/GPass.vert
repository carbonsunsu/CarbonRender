#version 430

layout(location = 0) in vec4 msPos;
layout(location = 1) in vec3 msN;
layout(location = 2) in vec3 msT;
layout(location = 3) in vec4 uvs;
layout(location = 4) in vec4 vColor;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 proMat;

out vec3 wsP;
out vec2 uv;
out mat3 TBN;
out vec3 n;

void main ()
{
	gl_Position = proMat * (viewMat * (modelMat * msPos));
	vec3 T = normalize(msT);
	vec3 N = normalize(msN);
	vec3 msB = cross(msN, msT);
	vec3 B = normalize(msB);
	TBN = (mat3(T.x, B.x, N.x,
				T.y, B.y, N.y,
				T.z, B.z, N.z));
	
	wsP = (modelMat * msPos).xyz;
	uv = uvs.xy;

	n = msN;
}