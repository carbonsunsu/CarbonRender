#version 430

in vec3 wsN;

out vec4 fColor;

void main()
{
	fColor = vec4(wsN, 1.0f);//vec4(1.0f, 0.0f, 0.0f, 1.0f);
}