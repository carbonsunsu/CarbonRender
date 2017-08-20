#include "..\Inc\CRWeatherSystem.h"

float4 WeatherSystem::Yxy2RGB(float Y, float x, float y)
{
	float3 XYZ = float3(x*Y / y, Y, (1.0f - x - y)*Y / y);
	return float4( 3.240479f*XYZ.x - 1.53715f*XYZ.y - 0.49853f*XYZ.z,
					-0.969256f*XYZ.x + 1.875991f*XYZ.y + 0.041556f*XYZ.z,
					0.055648f*XYZ.x - 0.204043f*XYZ.y + 1.057311f*XYZ.z,
					1.0f);
}
