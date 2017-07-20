#include "..\Inc\CRMath.h"

Matrix4x4::Matrix4x4(){}

Matrix4x4::Matrix4x4(float a[16])
{
	for (int i = 0; i < 16; i++)
		matrix[i] = a[i];
}

void Matrix4x4::set(float a[16])
{
	for (int i = 0; i < 16; i++)
		matrix[i] = a[i];
}

float4 Euler2Quaternion()
{
	return float4();
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
