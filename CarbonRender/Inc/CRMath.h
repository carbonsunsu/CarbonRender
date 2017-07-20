#ifndef CR_MATH
#define CR_MATH

#include <cmath>

static float pi = 3.1415926f;
static float a2r = pi / 180.0f;

struct float3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct float4
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

class Matrix4x4
{
public:
	float matrix[16] = {0.0f};

	Matrix4x4();

	Matrix4x4(float a[16]);

	void set(float a[16]);
};

//static float[4] Euler2Quaternion();

static Matrix4x4 operator * (Matrix4x4 m1, Matrix4x4 m2);

#endif
