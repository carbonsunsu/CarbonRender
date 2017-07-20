#include "..\Inc\CRMath.h"

float3::float3() {}

float3::float3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

float4::float4() {}

float4::float4(float a, float b, float c, float d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

Matrix3x3::Matrix3x3() {}

Matrix3x3::Matrix3x3(float a[9])
{
	for (int i = 0; i < 9; i++)
		matrix[i] = a[i];
}

void Matrix3x3::operator=(float a[9])
{
	for (int i = 0; i < 9; i++)
		matrix[i] = a[i];
}

Matrix4x4::Matrix4x4(){}

Matrix4x4::Matrix4x4(float a[16])
{
	for (int i = 0; i < 16; i++)
		matrix[i] = a[i];
}

void Matrix4x4::operator=(float a[16])
{
	for (int i = 0; i < 16; i++)
		matrix[i] = a[i];
}

Matrix4x4 operator*(Matrix4x4 m1, Matrix4x4 m2)
{
	Matrix4x4 resultM;
	for (int i = 0; i < 16; i++)
	{
		float temp = (float)i / 4.0f;
		int r = (int)temp * 4;
		int l = (int)i % 4;
		resultM.matrix[i] = m1.matrix[0 + r] * m2.matrix[0 + l] +
							m1.matrix[1 + r] * m2.matrix[4 + l] +
							m1.matrix[2 + r] * m2.matrix[8 + l] +
							m1.matrix[3 + r] * m2.matrix[12 + l];
	}

	return resultM;
}

float4 operator*(float4 v, Matrix4x4 m)
{
	float4 result;
	result.x = v.x*m.matrix[0] + v.y*m.matrix[4] + v.z*m.matrix[8] + v.w*m.matrix[12];
	result.y = v.x*m.matrix[1] + v.y*m.matrix[5] + v.z*m.matrix[9] + v.w*m.matrix[13];
	result.z = v.x*m.matrix[2] + v.y*m.matrix[6] + v.z*m.matrix[10] + v.w*m.matrix[14];
	result.w = v.x*m.matrix[3] + v.y*m.matrix[7] + v.z*m.matrix[11] + v.w*m.matrix[15];

	return result;
}

Matrix4x4 Translation(float x, float y, float z)
{
	float tArray[16] = {1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						x, y, z, 1.0f };
	Matrix4x4 t(tArray);

	return t;
}

Matrix4x4 Scale(float x, float y, float z)
{
	float sArray[16] = {x, 0.0f, 0.0f, 0.0f,
						0.0f, y, 0.0f, 0.0f,
						0.0f, 0.0f, z, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4x4 s(sArray);

	return s;
}