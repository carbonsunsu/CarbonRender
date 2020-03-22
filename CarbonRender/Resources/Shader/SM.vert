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

uniform vec2 depthClampPara;

out vec3 wsP;
out vec2 uv;
out float d;
out vec3 wsN;

void main ()
{
	gl_Position = viewMat * (modelMat * msPos);
	d = -gl_Position.z;
	d = d * depthClampPara.y;
	d = clamp(d, 0.0f, 1.0f);
	gl_Position = proMat * gl_Position;
	
	wsP = (modelMat * msPos).xyz;
	wsN = normalize(normalMat * msN);
	uv = uvs.xy;
}