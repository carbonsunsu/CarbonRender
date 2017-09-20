#version 430

in vec2 uv;

out vec4 fColor;

uniform sampler2D skyBuffer;
uniform sampler2D lightBuffer;
uniform sampler2D reflectionBuffer;
uniform sampler2D stencilBuffer;

void main ()
{
	vec4 sBuffer = texture2D(skyBuffer, uv);
	vec4 lBuffer = texture2D(lightBuffer, uv);
	vec4 rBuffer = texture2D(reflectionBuffer, uv);
	vec4 stencil = texture2D(stencilBuffer, uv);

	fColor.rgb = mix(sBuffer.rgb, lBuffer.rgb + rBuffer.rgb, stencil.r);
	fColor.rgb = pow(fColor.rgb, vec3(0.45454545f));
	fColor.a = 1.0f;
}