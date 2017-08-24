#version 430

in vec2 uv;

out vec4 fColor;

uniform sampler2D skyBuffer;
uniform sampler2D lightBuffer;

void main ()
{
	vec4 sBuffer = texture2D(skyBuffer, uv);
	vec4 lBuffer = texture2D(lightBuffer, uv);

	fColor = lBuffer + sBuffer * (1.0f - lBuffer.a);
	fColor.a = 1.0f;

	//to linear space
	fColor.x = pow(fColor.x, 1.0f/2.2f);
	fColor.y = pow(fColor.y, 1.0f/2.2f);
	fColor.z = pow(fColor.z, 1.0f/2.2f);
}