#version 430

in vec2 uv;

out vec4 fColor;

uniform sampler2D skyBuffer;
uniform sampler2D lightBuffer;
uniform sampler2D cubeBuffer;
uniform sampler2D ssrBuffer;
uniform sampler2D stencilBuffer;
uniform sampler2D cloudBuffer;

void main ()
{
	vec4 skyColor = texture(skyBuffer, uv);
	vec4 lightColor = texture(lightBuffer, uv);
	vec4 cubeColor = texture(cubeBuffer, uv);
	vec4 ssrColor = texture(ssrBuffer, uv, cubeColor.a * 10.0f);
	vec4 stencil = texture(stencilBuffer, uv);
	vec4 cloudColor = texture(cloudBuffer, uv);

	fColor.rgb = mix(skyColor.rgb,
					 lightColor.rgb + mix(cubeColor.rgb, ssrColor.rgb, ssrColor.a), 
					 stencil.r);
	
	fColor.rgb = fColor.rgb * cloudColor.a + cloudColor.rgb;
	fColor.rgb = pow(fColor.rgb, vec3(0.45454545f));
	fColor.a = 1.0f;
}