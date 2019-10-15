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
	vec4 skyColor = texture2D(skyBuffer, uv);
	vec4 lightColor = texture2D(lightBuffer, uv);
	vec4 cubeColor = texture2D(cubeBuffer, uv);
	vec4 ssrColor = textureLod(ssrBuffer, uv, cubeColor.a * 6.0f);
	vec4 stencil = texture2D(stencilBuffer, uv);
	vec4 cloudColor = texture2D(cloudBuffer, uv);

	fColor.rgb = mix(skyColor.rgb,
					 lightColor.rgb + mix(cubeColor.rgb, ssrColor.rgb, ssrColor.a), 
					 stencil.r);
	
	fColor.rgb = mix(fColor.rgb, cloudColor.rgb, cloudColor.a);
	fColor.rgb = pow(fColor.rgb, vec3(0.45454545f));
	fColor.a = 1.0f;
	//fColor.rgb = texture2D(lightBuffer, uv).rgb;
}