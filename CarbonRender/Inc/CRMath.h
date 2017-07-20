#ifndef CR_MATH
#define CR_MATH

#include <cmath>

#define pi 3.1415926f;
#define A2R 0.0174533f;

class float3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float3();
	float3(float a, float b, float c);
};

class float4
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	float4();
	float4(float a, float b, float c, float d);
};

class Matrix3x3
{
public:
	float matrix[9] = { 0.0f };

	Matrix3x3();

	Matrix3x3(float a[9]);

	void operator = (float a[9]);
};

class Matrix4x4
{
public:
	float matrix[16] = {0.0f};

	Matrix4x4();

	Matrix4x4(float a[16]);

	void operator = (float a[16]);
};

Matrix4x4 operator * (Matrix4x4 m1, Matrix4x4 m2);
float4 operator * (float4 v, Matrix4x4 m);

Matrix4x4 Translation(float x, float y, float z);
Matrix4x4 Scale(float x, float y, float z);
Matrix4x4 Rotate(float x, float y, float z);

#endif
