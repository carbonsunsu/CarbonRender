#version 430

in vec3 wsN;
in vec3 msT;

out vec4 fColor;

void main()
{
	vec3 L = vec3(0.0f, 1.0f, 0.0f);
	float NoL = dot(normalize(wsN), L);
	fColor = vec4(NoL,NoL,NoL, 1.0f);
}