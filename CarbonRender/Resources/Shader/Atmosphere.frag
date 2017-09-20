#version 430

layout(location = 0) out vec4 bColor;

uniform float t;
uniform float exposure;
uniform float thetaS;
uniform vec4 wsSunPos;
uniform vec3 zenith;
uniform sampler2D milkwayTex;

in vec3 wsP;
in vec2 uv;

void main()
{
	float SunSize = 0.9997;
	float e = 2.718281828459f;
	float PI = 3.1415926f;
	vec3 up = vec3(0.0f, 1.0f, 0.0f);

	vec3 vP = normalize(wsP);
	
	vec4 dayColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 nightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//nightColor = texture2D(milkwayTex, uv);
	//nightColor.rgb = pow(nightColor.rgb, vec3(2.2f));
	//night 
	if (wsSunPos.y <= 0.0f)
	{
		bColor = nightColor;//vec4 (0.0f, 0.0f, 0.0f, 1.0f);
		bColor.rgb = pow(bColor.rgb, vec3(0.45454545f));
		return;
	}

	if (vP.y < -0.01f)
	{
		bColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
		return;
	}

	vP.y = abs(vP.y);
	float cosTheta =dot(vP, up);
	vec3 vS = normalize(wsSunPos.xyz);
	float vSovP = dot(vS, vP);
	float gamma = acos(vSovP);

	vec3 A = vec3(-0.0193f*t-0.2592f, -0.0167f*t-0.2608f, 0.1787f*t-1.4630f);
	vec3 B = vec3(-0.0665f*t+0.0008f, -0.0950f*t+0.0092f, -0.3554f*t+0.4275f);
	vec3 C = vec3(-0.0004f*t+0.2125f, -0.0079f*t+0.2102f, -0.0227f*t+5.3251f);
	vec3 D = vec3(-0.0641f*t-0.8989f, -0.0441f*t-1.6537f, 0.1206f*t-2.5771f);
	vec3 E = vec3(-0.0033f*t+0.0452f, -0.0109f*t+0.0529f, -0.0670f*t+0.3703f);

	vec3 F1 = (1.0f+A*exp(B/cosTheta))*(1.0f+C*exp(D*gamma)+E*pow(vSovP, 2.0f));
	vec3 F2 = (1.0f+A*exp(B))*(1.0f+C*exp(D*thetaS)+E*pow(cos(thetaS), 2.0f));
	vec3 xyY = zenith * (F1/F2);
	xyY.z = 1.0f-exp((1.0f/-exposure)*xyY.z);

	//xyY to XYZ
	vec3 XYZ = vec3(xyY.x*xyY.z / xyY.y, xyY.z, (1.0f - xyY.x - xyY.y) * xyY.z/xyY.y);

	//XYZ to RGB
	dayColor = vec4(3.240479f*XYZ.x-1.53715f*XYZ.y-0.49853f*XYZ.z, 
					-0.969256f*XYZ.x+1.875991f*XYZ.y+0.041556f*XYZ.z,
					0.055648f*XYZ.x-0.204043f*XYZ.y+1.057311f*XYZ.z,
					1.0f);

	if (vSovP > SunSize)
		dayColor.rgb = dayColor.rgb*2.5f;

	float lum = dot(dayColor, vec4(0.22, 0.707, 0.071, 0.0));
	bColor = dayColor + nightColor * (1.0f - lum);
	//bColor.rgb = pow(bColor.rgb, vec3(0.45454545f));
}