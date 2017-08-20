#version 430

layout(location = 0) out vec4 bColor;

uniform float t;
uniform float exposure;
uniform float thetaS;
uniform vec4 wsSunPos;
uniform float Yz;
uniform float yz;
uniform float xz;

in vec3 wsP;

void main()
{
	float SunSize = 0.9997;
	float e = 2.718281828459f;
	float PI = 3.1415926f;
	vec3 up = vec3(0.0f, 1.0f, 0.0f);

	vec3 vP = normalize(wsP);
	vP.y = abs(vP.y);
	float cosTheta =dot(vP, up);
	vec3 vS = normalize(wsSunPos.xyz);
	float vSovP = dot(vS, vP);
	float gamma = acos(vSovP);

	vec4 skyColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//Y
	float A = 0.1787f*t-1.4630f;
	float B = -0.3554f*t+0.4275f;
	float C = -0.0227f*t+5.3251f;
	float D = 0.1206f*t-2.5771f;
	float E = -0.0670f*t+0.3703f;

	float F1 = (1.0f+A*exp(B/cosTheta))*(1.0f+C*exp(D*gamma)+E*pow(vSovP, 2.0f));
	float F2 = (1.0f+A*exp(B))*(1.0f+C*exp(D*thetaS)+E*pow(cos(thetaS), 2.0f));
	float Y = Yz*(F1/F2);
	Y = 1.0f-exp((1.0f/-exposure)*Y);

	//x
	A = -0.0193f*t-0.2592f;
	B = -0.0665f*t+0.0008f;
	C = -0.0004f*t+0.2125f;
	D = -0.0641f*t-0.8989f;
	E = -0.0033f*t+0.0452f;
				
	F1 = (1.0f+A*exp(B/cosTheta))*(1.0f+C*exp(D*gamma)+E*pow(vSovP, 2.0f));
	F2 = (1.0f+A*exp(B))*(1.0f+C*exp(D*thetaS)+E*pow(cos(thetaS), 2.0f));
	float x = xz*(F1/F2);
				
	//y
	A = -0.0167f*t-0.2608f;
	B = -0.0950f*t+0.0092f;
	C = -0.0079f*t+0.2102f;
	D = -0.0441f*t-1.6537f;
	E = -0.0109f*t+0.0529f;								
					
	F1 = (1.0f+A*exp(B/cosTheta))*(1.0f+C*exp(D*gamma)+E*pow(vSovP, 2.0f));
	F2 = (1.0f+A*exp(B))*(1.0f+C*exp(D*thetaS)+E*pow(cos(thetaS), 2.0f));
	float y = yz*(F1/F2);

	//Yxy to XYZ
	vec3 XYZ = vec3(x*Y/y, Y, (1.0f-x-y)*Y/y);

	//XYZ to RGB
	skyColor = vec4(3.240479f*XYZ.x-1.53715f*XYZ.y-0.49853f*XYZ.z, 
					-0.969256f*XYZ.x+1.875991f*XYZ.y+0.041556f*XYZ.z,
					0.055648f*XYZ.x-0.204043f*XYZ.y+1.057311f*XYZ.z,
					1.0f);

	if (dot(normalize(wsSunPos.xyz), vP) > SunSize)
		skyColor = skyColor*5.0f;

	//to linear space
	skyColor.x = pow(skyColor.x, 1.0f/2.2f);
	skyColor.y = pow(skyColor.y, 1.0f/2.2f);
	skyColor.z = pow(skyColor.z, 1.0f/2.2f);

	bColor = skyColor;
}