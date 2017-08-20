#version 430

layout(location = 0) out vec4 bColor;

in vec3 wsN;
in vec3 msT;

void main()
{
	vec3 L = vec3(0.0f, 1.0f, 0.0f);
	float NoL = dot(normalize(wsN), L);
	bColor = vec4(NoL,NoL,NoL, 1.0f);
}