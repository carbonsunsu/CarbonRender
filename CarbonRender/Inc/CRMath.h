#ifndef CR_MATH
#define CR_MATH

#include "..\Inc\CRGloble.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef e
#define e 2.718281828459f
#endif


#ifndef A2R
#define A2R PI/180.0f
#endif

#ifndef R2A
#define R2A 180.0f/PI
#endif

class float3;
class float4;
class Matrix3x3;
class Matrix4x4;
class Quaternion;

class float3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float3();
	float3(float a);
	float3(float a, float b, float c);
	float3(FbxDouble3 a);
	float3(FbxDouble4 a);
	float3(float4 a);
	float3(FbxColor a);

	float3 normalize();
	float Length();

	void operator = (FbxDouble3 a);
	void operator = (FbxDouble4 a);
	void operator = (float3 a);
	void operator = (float4 a);
	void operator = (FbxColor a);
};

class float4
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	float4();
	float4(float a);
	float4(float a, float b, float c, float d);
	float4(float3 a, float d);
	float4(FbxColor a);

	float4 normalize();

	void operator = (float4 a);
	void operator = (FbxColor a);
};

class Matrix3x3
{
public:
	/*
	0, 3, 6,
	1, 4, 7,
	2, 5, 8
	*/
	float matrix[9] = { 0.0f };

	Matrix3x3();

	Matrix3x3(float a[9]);
	Matrix3x3(Matrix4x4 &mat);

	void operator = (float a[9]);
	void operator = (Matrix3x3 mat);
	void operator = (Matrix4x4 mat);
};

class Matrix4x4
{
public:
	/*
	0, 4, 8, 12
	1, 5, 9, 13
	2, 6, 10, 14
	3, 7, 11, 15
	*/
	float matrix[16] = {0.0f};

	Matrix4x4();

	Matrix4x4(float a[16]);

	void operator = (float a[16]);
	void operator = (Matrix4x4 mat);
};

class Quaternion
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	Quaternion();
	Quaternion(float a, float b, float c, float d);

	Quaternion Normailze();
	Matrix4x4 ToMatrix();
};

class Noise
{
private:
	static float4 Permute(float4 a);
	static float3 Fade(float3 a);
	static float4 Fade(float4 a);

public:
	static float PerlinNoise(float3 pos, float freq);
	static float PerlinFbm(float3 pos, float freq, int octaveCount);

	static float WorleyNoise(float3 pos, float cellCount);
	static float WorleyFbm(float3 pos, float cellCount, float freqs[3]);

	static float CurlNoise(float3 pos);
};

class Random
{
public:
	static float Hash(float a);
	static float3 Hash(float3 a);
	static float Lcg(float a);
	static float3 Lcg(float3 a);
};

class Math
{
public:
	static float3 UnitAxisX();
	static float3 UnitAxisY();
	static float3 UnitAxisZ();

	static float Dot(float3 a, float3 b);
	static float Dot(float4 a, float4 b);
	static float Exp(float a);
	static float3 Exp(float3 v);
	static float4 Exp(float4 v);
	static float Sin(float a);
	static float3 Sin(float3 v);
	static float4 Sin(float4 v);
	static float FastInvSqrt(float a);
	static float3 FastInvSqrt(float3 a);
	static float4 FastInvSqrt(float4 a);

	static Matrix4x4 Translate(float x, float y, float z);
	static Matrix4x4 Scale(float x, float y, float z);
	static Quaternion Rotate(float3 axis, float angle);
	static Matrix4x4 CalculateModelMatrix(float* localCoord, float3 trans, float3 rota, float3 scal, bool useLocalAxis = true);
	static Matrix4x4 CalculateModelMatrix(float3 trans, float3 rota, float3 scal);

	static float Distance(float3 a, float3 b);
	static float Distance(float4 a, float4 b);

	static float Floor(float a);
	static float3 Floor(float3 a);
	static float4 Floor(float4 a);
	static float Mod(float a, float b);
	static float3 Mod(float3 a, float b);
	static float4 Mod(float4 a, float b);
	static float Fract(float a);
	static float3 Fract(float3 a);
	static float4 Fract(float4 a);
	static float Max(float a, float b);
	static float3 Max(float3 a, float3 b);
	static float4 Max(float4 a, float4 b);
	static float Min(float a, float b);
	static float3 Min(float3 a, float3 b);
	static float4 Min(float4 a, float4 b);
	static float Clamp(float a, float min, float max);
	static float3 Clamp(float3 a, float min, float max);
	static float4 Clamp(float4 a, float min, float max);
	static float Abs(float a);
	static float3 Abs(float3 a);
	static float4 Abs(float4 a);
	static float Lerp(float a, float b, float c);
	static float3 Lerp(float3 a, float3 b, float c);
	static float4 Lerp(float4 a, float4 b, float c);
	static float Step(float a, float b);
	static float3 Step(float3 a, float b);
	static float4 Step(float4 a, float b);
	static float3 Step(float3 a, float3 b);
	static float4 Step(float4 a, float4 b);

	static float Remap(float a, float oldMin, float oldMax, float newMin, float newMax);

	static float4 xyY2RGB(float3 xyY);

};

Matrix4x4 operator * (Matrix4x4 m1, Matrix4x4 m2);
float4 operator * (float4 v, Matrix4x4 m);
float4 operator * (float4 v, Quaternion r);
float3 operator * (float s, float3 v);
float3 operator * (float3 v, float s);
float4 operator * (float s, float4 v);
float4 operator * (float4 v, float s);
float3 operator * (float3 v1, float3 v2);
float4 operator * (float4 v1, float4 v2);
float3 operator / (float3 v, float s);
float4 operator / (float4 v, float s);
float3 operator / (float3 v1, float3 v2);
float4 operator / (float4 v1, float4 v2);
float3 operator + (float a, float3 v);
float3 operator + (float3 v, float a);
float4 operator + (float a, float4 v);
float4 operator + (float4 v, float a);
float3 operator + (float3 v1, float3 v2);
float4 operator + (float4 v1, float4 v2);
float3 operator - (float3 v1, float3 v2);
float4 operator - (float4 v1, float4 v2);

#endif
