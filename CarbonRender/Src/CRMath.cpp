#include "..\Inc\CRMath.h"

float3::float3() {}

float3::float3(float a)
{
	x = a;
	y = a;
	z = a;
}

float3::float3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

float3::float3(FbxDouble3 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

float3::float3(FbxDouble4 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

float3::float3(float4 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

float3::float3(FbxColor a)
{
	x = a.mRed;
	y = a.mGreen;
	z = a.mBlue;
}

float3 float3::normalize()
{
	float l = sqrt(x*x + y*y + z*z);

	return float3(x / l,y / l,z / l);
}

void float3::operator=(FbxDouble3 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

void float3::operator=(FbxDouble4 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

void float3::operator=(float3 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

void float3::operator=(float4 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

void float3::operator=(FbxColor a)
{
	x = a.mRed;
	y = a.mGreen;
	z = a.mBlue;
}

float4::float4() {}

float4::float4(float a)
{
	x = a;
	y = a;
	z = a;
	w = a;
}

float4::float4(float a, float b, float c, float d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

float4::float4(float3 a, float d)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = d;
}

float4::float4(FbxColor a)
{
	x = a.mRed;
	y = a.mGreen;
	z = a.mBlue;
	w = a.mAlpha;
}

float4 float4::normalize()
{
	float l = sqrt(x*x + y*y + z*z + w*w);

	return float4(x / l, y / l, z / l, w / l);
}

void float4::operator=(float4 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;
}

void float4::operator=(FbxColor a)
{
	x = a.mRed;
	y = a.mGreen;
	z = a.mBlue;
	w = a.mAlpha;
}

Matrix3x3::Matrix3x3() {}

Matrix3x3::Matrix3x3(float a[9])
{
	for (int i = 0; i < 9; i++)
		matrix[i] = a[i];
}

Matrix3x3::Matrix3x3(Matrix4x4 &mat)
{
	for (int i = 0; i < 3; i++)
	{
		matrix[i * 3] = mat.matrix[i * 4];
		matrix[i * 3 + 1] = mat.matrix[i * 4 + 1];
		matrix[i * 3 + 2] = mat.matrix[i * 4 + 2];
	}
}

void Matrix3x3::operator=(float a[9])
{
	for (int i = 0; i < 9; i++)
		matrix[i] = a[i];
}

void Matrix3x3::operator=(Matrix3x3 mat)
{
	for (int i = 0; i < 9; i++)
		matrix[i] = mat.matrix[i];
}

void Matrix3x3::operator=(Matrix4x4 mat)
{
	for (int i = 0; i < 3; i++)
	{
		matrix[i * 3] = mat.matrix[i * 4];
		matrix[i * 3 + 1] = mat.matrix[i * 4 + 1];
		matrix[i * 3 + 2] = mat.matrix[i * 4 + 2];
	}
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

void Matrix4x4::operator=(Matrix4x4 mat)
{
	for (int i = 0; i < 16; i++)
		matrix[i] = mat.matrix[i];
}

Quaternion::Quaternion() {}

Quaternion::Quaternion(float a, float b, float c, float d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

Quaternion Quaternion::Normailze()
{
	float lengthR = 1.0f / sqrtf(x*x + y*y + z*z + w*w);
	return Quaternion(x*lengthR, y*lengthR, z*lengthR, w*lengthR);
}

Matrix4x4 Quaternion::ToMatrix()
{
	float r[16] = { 1.0f - 2 * (y*y + z*z), 2 * (x*y - z*w), 2 * (x*z + y*w), 0.0f,
					2 * (x*y + z*w), 1.0f -  2 * (x*x + z*z), 2 * (y*z - x*w), 0.0f,
					2 * (x*z - y*w), 2 * (y*z + x*w), 1.0f - 2 * (x*x + y*y), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
	return Matrix4x4(r);
}

const int Noise::perm[256] =
{
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,
	117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,
	165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
	105,92,41,55,46,245,40,244,102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,
	187,208, 89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,
	3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,
	227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152, 2,44,154,163,
	70,221,153,101,155,167, 43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,
	178,185, 112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,
	81,51,145,235,249,14,239,107,49,192,214, 31,181,199,106,157,184, 84,204,176,
	115,121,50,45,127, 4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,
	195,78,66,215,61,156,180
};

const float3 Noise::grab[16] =
{
	float3(1.0f,1.0f,0.0f), float3(-1.0f,1.0f,0.0f), float3(1.0f,-1.0f,0.0f), float3(-1.0f,-1.0f,0.0f),
	float3(1.0f,0.0f,1.0f), float3(-1.0f,0.0f,1.0f), float3(1.0f,0.0f,-1.0f), float3(-1.0f,0.0f,-1.0f),
	float3(0.0f,1.0f,1.0f), float3(0.0f,-1.0f,1.0f), float3(0.0f,1.0f,-1.0f), float3(0.0f,-1.0f,-1.0f),
	float3(1.0f,1.0f,0.0f), float3(0.0f,-1.0f,1.0f), float3(-1.0f,1.0f,0.0f), float3(0.0f,-1.0f,-1.0f),
};

float3 Noise::PerlinFade(float3 a)
{
	return a * a * a * (a * (a * 6.0f - 15.0f) + 10.0f);
}

float Noise::PerlinGrab(int a, float3 b)
{
	return Dot(b, grab[a % 16]);
}

float Noise::PerlinPermute(int a)
{
	return perm[a % 256];
}

float3 Noise::WorleyPermute(float3 a)
{
	return Mod(a * a * 34.0f + a, 289.0f);
}

float4 Noise::WorleyPermute(float4 a)
{
	return Mod(a * a * 34.0f + a, 289.0f);
}

float3 Noise::Length(float3 a, float3 b, float3 c)
{
	return a * a + b * b + c * c;
}

float4 Noise::Length(float4 a, float4 b, float4 c)
{
	return a * a + b * b + c * c;
}

float Noise::PerlinNoise(float3 uv)
{
	float3 P = Mod(Floor(uv), 256.0f);
	uv = uv - Floor(uv);
	float3 f = PerlinFade(uv);

	float A = PerlinPermute(P.x) + P.y;
	float AA = PerlinPermute(A) + P.z;
	float AB = PerlinPermute(A + 1) + P.z;
	float B = PerlinPermute(P.x + 1) + P.y;
	float BA = PerlinPermute(B) + P.z;
	float BB = PerlinPermute(B + 1) + P.z;

	float n = Lerp(
		Lerp(Lerp(PerlinGrab(PerlinPermute(AA), uv),
			PerlinGrab(PerlinPermute(BA), uv + float3(-1.0f, 0.0f, 0.0f)), f.x),
			Lerp(PerlinGrab(PerlinPermute(AB), uv + float3(0.0f, -1.0f, 0.0f)),
				PerlinGrab(PerlinPermute(BB), uv + float3(-1.0f, -1.0f, 0.0f)), f.x), f.y),
		Lerp(Lerp(PerlinGrab(PerlinPermute(AA + 1), uv + float3(0.0f, 0.0f, -1.0f)),
			PerlinGrab(PerlinPermute(BA + 1), uv + float3(-1.0f, 0.0f, -1.0f)), f.x),
			Lerp(PerlinGrab(PerlinPermute(AB + 1), uv + float3(0.0f, -1.0f, -1.0f)),
				PerlinGrab(PerlinPermute(BB + 1), uv + float3(-1.0f, -1.0f, -1.0f)), f.x), f.y),
		f.z);

	return n;
}

float Noise::PerlinFbm(float3 uv)
{
	float fbm = PerlinNoise(uv * 5.0f) +
		0.5f * PerlinNoise(uv * 10.0f) +
		0.25f * PerlinNoise(uv * 20.0f) +
		0.125f * PerlinNoise(uv * 40.0f) +
		0.0625f * PerlinNoise(uv * 80.0f);
	return fbm * 0.25f + 0.5f;
}

float Noise::WorleyNoise(float3 uv)
{
	const float jitter = 1.0f;
	const float K = 0.142857f; // 1/7
	const float Ko = 0.428571f; // 1/2-K/2
	const float K2 = 0.020408f; // 1/(7*7)
	const float Kz = 0.166667f; // 1/6
	const float Kzo = 0.416667f; // 1/2-1/6*2

	float3 Pi = Mod(Floor(uv), 289.0f);
	float3 Pf = Fract(uv) - 0.5;

	float3 Pfx = Pf.x + float3(1.0f, 0.0f, -1.0f);
	float3 Pfy = Pf.y + float3(1.0f, 0.0f, -1.0f);
	float3 Pfz = Pf.z + float3(1.0f, 0.0f, -1.0f);

	float3 p = WorleyPermute(Pi.x + float3(-1.0f, 0.0f, 1.0f));
	float3 p1 = WorleyPermute(p + Pi.y - 1.0f);
	float3 p2 = WorleyPermute(p + Pi.y);
	float3 p3 = WorleyPermute(p + Pi.y + 1.0f);

	float3 p11 = WorleyPermute(p1 + Pi.z - 1.0f);
	float3 p12 = WorleyPermute(p1 + Pi.z);
	float3 p13 = WorleyPermute(p1 + Pi.z + 1.0f);

	float3 p21 = WorleyPermute(p2 + Pi.z - 1.0f);
	float3 p22 = WorleyPermute(p2 + Pi.z);
	float3 p23 = WorleyPermute(p2 + Pi.z + 1.0f);

	float3 p31 = WorleyPermute(p3 + Pi.z - 1.0f);
	float3 p32 = WorleyPermute(p3 + Pi.z);
	float3 p33 = WorleyPermute(p3 + Pi.z + 1.0f);

	float3 ox11 = Fract(p11*K) - Ko;
	float3 oy11 = Mod(Floor(p11*K), 7.0f)*K - Ko;
	float3 oz11 = Floor(p11*K2)*Kz - Kzo;

	float3 ox12 = Fract(p12*K) - Ko;
	float3 oy12 = Mod(Floor(p12*K), 7.0f)*K - Ko;
	float3 oz12 = Floor(p12*K2)*Kz - Kzo;

	float3 ox13 = Fract(p13*K) - Ko;
	float3 oy13 = Mod(Floor(p13*K), 7.0f)*K - Ko;
	float3 oz13 = Floor(p13*K2)*Kz - Kzo;

	float3 ox21 = Fract(p21*K) - Ko;
	float3 oy21 = Mod(Floor(p21*K), 7.0f)*K - Ko;
	float3 oz21 = Floor(p21*K2)*Kz - Kzo;

	float3 ox22 = Fract(p22*K) - Ko;
	float3 oy22 = Mod(Floor(p22*K), 7.0f)*K - Ko;
	float3 oz22 = Floor(p22*K2)*Kz - Kzo;

	float3 ox23 = Fract(p23*K) - Ko;
	float3 oy23 = Mod(Floor(p23*K), 7.0f)*K - Ko;
	float3 oz23 = Floor(p23*K2)*Kz - Kzo;

	float3 ox31 = Fract(p31*K) - Ko;
	float3 oy31 = Mod(Floor(p31*K), 7.0f)*K - Ko;
	float3 oz31 = Floor(p31*K2)*Kz - Kzo;

	float3 ox32 = Fract(p32*K) - Ko;
	float3 oy32 = Mod(Floor(p32*K), 7.0f)*K - Ko;
	float3 oz32 = Floor(p32*K2)*Kz - Kzo;

	float3 ox33 = Fract(p33*K) - Ko;
	float3 oy33 = Mod(Floor(p33*K), 7.0f)*K - Ko;
	float3 oz33 = Floor(p33*K2)*Kz - Kzo;

	float3 dx11 = Pfx + jitter*ox11;
	float3 dy11 = Pfy.x + jitter*oy11;
	float3 dz11 = Pfz.x + jitter*oz11;

	float3 dx12 = Pfx + jitter*ox12;
	float3 dy12 = Pfy.x + jitter*oy12;
	float3 dz12 = Pfz.y + jitter*oz12;

	float3 dx13 = Pfx + jitter*ox13;
	float3 dy13 = Pfy.x + jitter*oy13;
	float3 dz13 = Pfz.z + jitter*oz13;

	float3 dx21 = Pfx + jitter*ox21;
	float3 dy21 = Pfy.y + jitter*oy21;
	float3 dz21 = Pfz.x + jitter*oz21;

	float3 dx22 = Pfx + jitter*ox22;
	float3 dy22 = Pfy.y + jitter*oy22;
	float3 dz22 = Pfz.y + jitter*oz22;

	float3 dx23 = Pfx + jitter*ox23;
	float3 dy23 = Pfy.y + jitter*oy23;
	float3 dz23 = Pfz.z + jitter*oz23;

	float3 dx31 = Pfx + jitter*ox31;
	float3 dy31 = Pfy.z + jitter*oy31;
	float3 dz31 = Pfz.x + jitter*oz31;

	float3 dx32 = Pfx + jitter*ox32;
	float3 dy32 = Pfy.z + jitter*oy32;
	float3 dz32 = Pfz.y + jitter*oz32;

	float3 dx33 = Pfx + jitter*ox33;
	float3 dy33 = Pfy.z + jitter*oy33;
	float3 dz33 = Pfz.z + jitter*oz33;

	float3 d11 = Length(dx11, dy11, dz11);
	float3 d12 = Length(dx12, dy12, dz12);
	float3 d13 = Length(dx13, dy13, dz13);
	float3 d21 = Length(dx21, dy21, dz21);
	float3 d22 = Length(dx22, dy22, dz22);
	float3 d23 = Length(dx23, dy23, dz23);
	float3 d31 = Length(dx31, dy31, dz31);
	float3 d32 = Length(dx32, dy32, dz32);
	float3 d33 = Length(dx33, dy33, dz33);

	float3 d1a = Min(d11, d12);
	d12 = Max(d11, d12);
	d11 = Min(d1a, d13);
	d13 = Max(d1a, d13);
	d12 = Min(d12, d13);
	float3 d2a = Min(d21, d22);
	d22 = Max(d21, d22);
	d21 = Min(d2a, d23);
	d23 = Max(d2a, d23);
	d22 = Min(d22, d23);
	float3 d3a = Min(d31, d32);
	d32 = Max(d31, d32);
	d31 = Min(d3a, d33);
	d33 = Max(d3a, d33);
	d32 = Min(d32, d33);
	float3 da = Min(d11, d21);
	d21 = Max(d11, d21);
	d11 = Min(da, d31);
	d31 = Max(da, d31);
	d11.x = (d11.x < d11.y) ? d11.x : d11.y;
	d11.y = (d11.x < d11.y) ? d11.y : d11.x;
	d11.x = (d11.x < d11.z) ? d11.x : d11.z;
	d11.z = (d11.x < d11.z) ? d11.z : d11.x;
	d12 = Min(d12, d21);
	d12 = Min(d12, d22);
	d12 = Min(d12, d31);
	d12 = Min(d12, d32);
	d11.y = fminf(d11.y, d12.x);
	d11.z = fminf(d11.z, d12.y);
	d11.y = fminf(d11.y, d12.z);
	d11.y = fminf(d11.y, d11.z);
	return 1.0f - sqrt(d11.x);
}

float Noise::WorleyNoiseFast(float3 uv)
{
	const float jitter = 0.8f;
	const float K = 0.142857f; // 1/7
	const float Ko = 0.428571f; // 1/2-K/2
	const float K2 = 0.020408f; // 1/(7*7)
	const float Kz = 0.166667f; // 1/6
	const float Kzo = 0.416667f; // 1/2-1/6*2

	float3 Pi = Mod(Floor(uv), 289.0f);
	float3 Pf = Fract(uv);

	float4 Pfx = Pf.x + float4(0.0f, -1.0f, 0.0f, -1.0f);
	float4 Pfy = Pf.y + float4(0.0f, 0.0f, -1.0f, -1.0f);

	float4 p = WorleyPermute(float4(0.0f, 1.0f, 0.0f, 1.0f) + Pi.x);
	p = WorleyPermute(p + Pi.y + float4(0.0f, 0.0f, 1.0f, 1.0f));
	float4 p1 = WorleyPermute(p + Pi.z);
	float4 p2 = WorleyPermute(p + Pi.z + float4(1.0f));

	float4 ox1 = Fract(p1*K) - Ko;
	float4 oy1 = Mod(Floor(p1*K), 7.0f)*K - Ko;
	float4 oz1 = Floor(p1*K2)*Kz - Kzo;
	float4 ox2 = Fract(p2*K) - Ko;
	float4 oy2 = Mod(Floor(p2*K), 7.0f)*K - Ko;
	float4 oz2 = Floor(p2*K2)*Kz - Kzo;

	float4 dx1 = Pfx + jitter*ox1;
	float4 dy1 = Pfy + jitter*oy1;
	float4 dz1 = Pf.z + jitter*oz1;
	float4 dx2 = Pfx + jitter*ox2;
	float4 dy2 = Pfy + jitter*oy2;
	float4 dz2 = Pf.z - 1.0f + jitter*oz2;

	float4 d1 = Length(dx1, dy1, dz1);
	float4 d2 = Length(dx2, dy2, dz2);

	float4 d = Min(d1, d2);
	d2 = Max(d1, d2);

	d.x = (d.x < d.y) ? d.x : d.y;
	d.y = (d.x < d.y) ? d.y : d.x;
	d.x = (d.x < d.z) ? d.x : d.z;
	d.z = (d.x < d.z) ? d.z : d.x;
	d.x = (d.x < d.w) ? d.x : d.w;
	d.w = (d.x < d.w) ? d.w : d.x;
	d.y = fminf(d.y, d2.y);
	d.z = fminf(d.z, d2.z);
	d.w = fminf(d.w, d2.w);

	d.y = fminf(d.y, d.z);
	d.y = fminf(d.y, d.w);
	d.y = fminf(d.y, d2.x);

	return 1.0f - sqrt(d.x);
}

float Noise::WorleyFbm(float3 uv)
{
	float fbm = WorleyNoise(uv) +
		0.5f * WorleyNoise(uv * 5.0f) +
		0.25f * WorleyNoise(uv * 10.0f) +
		0.125f * WorleyNoise(uv * 20.0f) +
		0.0625f * WorleyNoise(uv * 40.0f);
	return fbm * 0.5f;
}

float Noise::CurlNoise(float3 uv)
{
	return 0.0f;
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

float4 operator*(float4 v, Quaternion r)
{
	return float4(v.x*r.x, v.y*r.y, v.z*r.z, v.w*r.w);
}

float3 operator*(float3 v, float s)
{
	return float3(v.x*s, v.y*s, v.z*s);
}

float4 operator*(float4 v, float s)
{
	return float4(v.x*s, v.y*s, v.z*s, v.w*s);
}

float3 operator*(float3 v1, float3 v2)
{
	return float3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

float4 operator*(float4 v1, float4 v2)
{
	return float4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
}

float3 operator/(float3 v, float s)
{
	float sT = 1.0f / s;
	return float3(v.x*sT, v.y*sT, v.z*sT);
}

float4 operator/(float4 v, float s)
{
	float sT = 1.0f / s;
	return float4(v.x*sT, v.y*sT, v.z*sT, v.w*sT);
}

float3 operator/(float3 v1, float3 v2)
{
	return float3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

float4 operator/(float4 v1, float4 v2)
{
	return float4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

float3 operator+(float a, float3 v)
{
	return float3(a + v.x, a + v.y, a + v.z);
}

float4 operator+(float a, float4 v)
{
	return float4(a + v.x, a + v.y, a + v.z, a + v.w);
}

float3 operator+(float3 v1, float3 v2)
{
	return float3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

float4 operator+(float4 v1, float4 v2)
{
	return float4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

float3 operator-(float3 v1, float3 v2)
{
	return float3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

float4 operator-(float4 v1, float4 v2)
{
	return float4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

float Dot(float3 a, float3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float Dot(float4 a, float4 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

float3 exp(float3 v)
{
	return float3(exp(v.x), exp(v.y), exp(v.z));
}

Matrix4x4 Translate(float x, float y, float z)
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

Quaternion Rotate(float3 axis, float angle)
{
	float r = angle * 0.5f * A2R;
	return Quaternion(axis.x * sin(r),
					  axis.y * sin(r),
					  axis.z * sin(r),
					  cos(r));
}

Matrix4x4 CalculateModelMatrix(float* localCoord, float3 trans, float3 rota, float3 scal)
{
	float4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
	float4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
	float4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

	Matrix4x4 mMatrix = Scale(scal.x, scal.y, scal.z) *
						Rotate(xAxis, rota.x).Normailze().ToMatrix();

	yAxis = yAxis * Rotate(xAxis, rota.x).Normailze().ToMatrix();
	zAxis = zAxis * Rotate(xAxis, rota.x).Normailze().ToMatrix();

	mMatrix = mMatrix * Rotate(float3(yAxis), rota.y).Normailze().ToMatrix();

	xAxis = xAxis * Rotate(float3(yAxis), rota.y).Normailze().ToMatrix();
	zAxis = zAxis * Rotate(float3(yAxis), rota.y).Normailze().ToMatrix();

	mMatrix = mMatrix * Rotate(float3(zAxis), rota.z).Normailze().ToMatrix();

	xAxis = xAxis * Rotate(float3(zAxis), rota.z).Normailze().ToMatrix();
	yAxis = zAxis * Rotate(float3(zAxis), rota.z).Normailze().ToMatrix();

	mMatrix = mMatrix * Translate(trans.x, trans.y, trans.z);

	localCoord[0] = xAxis.x;
	localCoord[1] = xAxis.y;
	localCoord[2] = xAxis.z;

	localCoord[3] = yAxis.x;
	localCoord[4] = yAxis.y;
	localCoord[5] = yAxis.z;

	localCoord[6] = zAxis.x;
	localCoord[7] = zAxis.y;
	localCoord[8] = zAxis.z;

	return  mMatrix;
}

Matrix4x4 CalculateModelMatrix(float3 trans, float3 rota, float3 scal)
{
	float4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
	float4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
	float4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

	Matrix4x4 mMatrix = Scale(scal.x, scal.y, scal.z) *
		Rotate(yAxis, rota.y).Normailze().ToMatrix() *
		Rotate(xAxis, rota.x).Normailze().ToMatrix() *
		Rotate(zAxis, rota.z).Normailze().ToMatrix() *
		Translate(trans.x, trans.y, trans.z);

	return  mMatrix;
}

float Distance(float3 a, float3 b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;
	return sqrtf(x*x + y*y + z*z);
}

float Distance(float4 a, float4 b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;
	float w = a.w - b.w;
	return sqrtf(x*x + y*y + z*z + w*w);
}

float3 Floor(float3 a)
{
	return float3(floorf(a.x), floorf(a.y), floorf(a.z));
}

float4 Floor(float4 a)
{
	return float4(floorf(a.x), floorf(a.y), floorf(a.z), floorf(a.w));
}

float3 Mod(float3 a, float b)
{
	return float3(fmodf(a.x, b), fmodf(a.y, b), fmodf(a.z, b));
}

float4 Mod(float4 a, float b)
{
	return float4(fmodf(a.x, b), fmodf(a.y, b), fmodf(a.z, b), fmodf(a.w, b));
}

float3 Fract(float3 a)
{
	return a - Floor(a);
}

float4 Fract(float4 a)
{
	return a - Floor(a);
}

float3 Max(float3 a, float3 b)
{
	return float3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
}

float4 Max(float4 a, float4 b)
{
	return float4(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z), fmaxf(a.w, b.w));
}

float3 Min(float3 a, float3 b)
{
	return float3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
}

float4 Min(float4 a, float4 b)
{
	return float4(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z), fminf(a.w, b.w));
}

float3 Abs(float3 a)
{
	return float3(fabsf(a.x), fabsf(a.y), fabsf(a.z));
}

float4 Abs(float4 a)
{
	return float4(fabsf(a.x), fabsf(a.y), fabsf(a.z), fabsf(a.w));
}

float Lerp(float a, float b, float c)
{
	return a + (b - a) * c;
}

float3 Lerp(float3 a, float3 b, float c)
{
	return a + (b - a) * c;
}

float4 Lerp(float4 a, float4 b, float c)
{
	return a + (b - a) * c;
}

float Remap(float a, float oldMin, float oldMax, float newMin, float newMax)
{
	return newMin + (newMax - newMin) * (a - oldMin) / (oldMax - oldMin);
}

float4 xyY2RGB(float3 xyY)
{
	//xyY to XYZ
	float3 XYZ = float3(xyY.x*xyY.z / xyY.y, xyY.z, (1.0f - xyY.x - xyY.y)*xyY.z / xyY.y);

	//XYZ to rgb
	return float4(3.240479f*XYZ.x - 1.53715f*XYZ.y - 0.49853f*XYZ.z,
				-0.969256f*XYZ.x + 1.875991f*XYZ.y + 0.041556f*XYZ.z,
				0.055648f*XYZ.x - 0.204043f*XYZ.y + 1.057311f*XYZ.z,
				1.0f);
}