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
	float l = sqrtf(x*x + y*y + z*z);

	return float3(x / l,y / l,z / l);
}

float float3::Length()
{
	return sqrtf(x*x + y*y + z*z);
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
	float l = sqrtf(x*x + y*y + z*z + w*w);

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

float4 Noise::Permute(float4 a)
{
	return Mod(a * a * 34.0f + a, 289.0f);
}

float3 Noise::Fade(float3 a)
{
	return (a * a * a) * (a * a * 6.0f - 15.0f * a + 10.0f);
}

float4 Noise::Fade(float4 a)
{
	return (a * a * a) * (a * a * 6.0f - 15.0f * a + 10.0f);
}

float Noise::PerlinNoise(float3 pos, float freq)
{
	pos = pos * freq;
	float4 p = float4(pos, 0.0f);

	float4 Pi0 = Mod(Floor(p), freq);
	float4 Pi1 = Mod(Pi0 + 1.0f, freq);
	float4 Pf0 = Fract(p);
	float4 Pf1 = Pf0 - 1.0f;
	float4 ix = float4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
	float4 iy = float4(Pi0.y, Pi0.y, Pi1.y, Pi1.y);
	float4 iz0 = float4(Pi0.z);
	float4 iz1 = float4(Pi1.z);
	float4 iw0 = float4(Pi0.w);
	float4 iw1 = float4(Pi1.w);

	float4 ixy = Noise::Permute(Noise::Permute(ix) + iy);
	float4 ixy0 = Noise::Permute(ixy + iz0);
	float4 ixy1 = Noise::Permute(ixy + iz1);
	float4 ixy00 = Noise::Permute(ixy0 + iw0);
	float4 ixy01 = Noise::Permute(ixy0 + iw1);
	float4 ixy10 = Noise::Permute(ixy1 + iw0);
	float4 ixy11 = Noise::Permute(ixy1 + iw1);

	float4 gx00 = ixy00 / 7.0f;
	float4 gy00 = Floor(gx00) / 7.0f;
	float4 gz00 = Floor(gy00) / 6.0f;
	gx00 = Fract(gx00) - 0.5f;
	gy00 = Fract(gy00) - 0.5f;
	gz00 = Fract(gz00) - 0.5f;
	float4 gw00 = 0.5f - Abs(gx00) - Abs(gy00) - Abs(gz00);
	float4 sw00 = Step(float4(0.0f), gw00);
	gx00 = gx00 - sw00 * (Step(gx00, 0.0f) - 0.5f);
	gy00 = gy00 - sw00 * (Step(gy00, 0.0f) - 0.5f);

	float4 gx01 = ixy01 / 7.0f;
	float4 gy01 = Floor(gx01) / 7.0f;
	float4 gz01 = Floor(gy01) / 6.0f;
	gx01 = Fract(gx01) - 0.5f;
	gy01 = Fract(gy01) - 0.5f;
	gz01 = Fract(gz01) - 0.5f;
	float4 gw01 = 0.5f - Abs(gx01) - Abs(gy01) - Abs(gz01);
	float4 sw01 = Step(float4(0.0f), gw01);
	gx01 = gx01 - sw01 * (Step(gx01, 0.0f) - 0.5f);
	gy01 = gy01 - sw01 * (Step(gy01, 0.0f) - 0.5f);

	float4 gx10 = ixy10 / 7.0f;
	float4 gy10 = Floor(gx10) / 7.0f;
	float4 gz10 = Floor(gy10) / 6.0f;
	gx10 = Fract(gx10) - 0.5f;
	gy10 = Fract(gy10) - 0.5f;
	gz10 = Fract(gz10) - 0.5f;
	float4 gw10 = 0.5f - Abs(gx10) - Abs(gy10) - Abs(gz10);
	float4 sw10 = Step(float4(0.0f), gw10);
	gx10 = gx10 - sw10 * (Step(gx10, 0.0f) - 0.5f);
	gy10 = gy10 - sw10 * (Step(gy10, 0.0f) - 0.5f);

	float4 gx11 = ixy11 / 7.0f;
	float4 gy11 = Floor(gx11) / 7.0f;
	float4 gz11 = Floor(gy11) / 6.0f;
	gx11 = Fract(gx11) - 0.5f;
	gy11 = Fract(gy11) - 0.5f;
	gz11 = Fract(gz11) - 0.5f;
	float4 gw11 = 0.5f - Abs(gx11) - Abs(gy11) - Abs(gz11);
	float4 sw11 = Step(float4(0.0f), gw11);
	gx11 = gx11 - sw11 * (Step(gx11, 0.0f) - 0.5f);
	gy11 = gy11 - sw11 * (Step(gy11, 0.0f) - 0.5f);

	float4 g0000 = float4(gx00.x, gy00.x, gz00.x, gw00.x);
	float4 g1000 = float4(gx00.y, gy00.y, gz00.y, gw00.y);
	float4 g0100 = float4(gx00.z, gy00.z, gz00.z, gw00.z);
	float4 g1100 = float4(gx00.w, gy00.w, gz00.w, gw00.w);

	float4 g0010 = float4(gx10.x, gy10.x, gz10.x, gw10.x);
	float4 g1010 = float4(gx10.y, gy10.y, gz10.y, gw10.y);
	float4 g0110 = float4(gx10.z, gy10.z, gz10.z, gw10.z);
	float4 g1110 = float4(gx10.w, gy10.w, gz10.w, gw10.w);

	float4 g0001 = float4(gx01.x, gy01.x, gz01.x, gw01.x);
	float4 g1001 = float4(gx01.y, gy01.y, gz01.y, gw01.y);
	float4 g0101 = float4(gx01.z, gy01.z, gz01.z, gw01.z);
	float4 g1101 = float4(gx01.w, gy01.w, gz01.w, gw01.w);

	float4 g0011 = float4(gx11.x, gy11.x, gz11.x, gw11.x);
	float4 g1011 = float4(gx11.y, gy11.y, gz11.y, gw11.y);
	float4 g0111 = float4(gx11.z, gy11.z, gz11.z, gw11.z);
	float4 g1111 = float4(gx11.w, gy11.w, gz11.w, gw11.w);

	float4 norm00 = FastInvSqrt(float4(Dot(g0000, g0000), Dot(g0100, g0100), Dot(g1000, g1000), Dot(g1100, g1100)));
	g0000 = g0000 * norm00.x;
	g0100 = g0100 * norm00.y;
	g1000 = g1000 * norm00.z;
	g1100 = g1100 * norm00.w;
	float4 norm01 = FastInvSqrt(float4(Dot(g0001, g0001), Dot(g0101, g0101), Dot(g1001, g1001), Dot(g1101, g1101)));
	g0001 = g0001 * norm01.x;
	g0101 = g0101 * norm01.y;
	g1001 = g1001 * norm01.z;
	g1101 = g1101 * norm01.w;

	float4 norm10 = FastInvSqrt(float4(Dot(g0010, g0010), Dot(g0110, g0110), Dot(g1010, g1010), Dot(g1110, g1110)));
	g0010 = g0010 * norm10.x;
	g0110 = g0110 * norm10.y;
	g1010 = g1010 * norm10.z;
	g1110 = g1110 * norm10.w;

	float4 norm11 = FastInvSqrt(float4(Dot(g0011, g0011), Dot(g0111, g0111), Dot(g1011, g1011), Dot(g1111, g1111)));
	g0011 = g0011 * norm11.x;
	g0111 = g0111 * norm11.y;
	g1011 = g1011 * norm11.z;
	g1111 = g1111 * norm11.w;

	float n0000 = Dot(g0000, Pf0);
	float n1000 = Dot(g1000, float4(Pf1.x, Pf0.y, Pf0.z, Pf0.w));
	float n0100 = Dot(g0100, float4(Pf0.x, Pf1.y, Pf0.z, Pf0.w));
	float n1100 = Dot(g1100, float4(Pf1.x, Pf1.y, Pf0.z, Pf0.w));
	float n0010 = Dot(g0010, float4(Pf0.x, Pf0.y, Pf1.z, Pf0.w));
	float n1010 = Dot(g1010, float4(Pf1.x, Pf0.y, Pf1.z, Pf0.w));
	float n0110 = Dot(g0110, float4(Pf0.x, Pf1.y, Pf1.z, Pf0.w));
	float n1110 = Dot(g1110, float4(Pf1.x, Pf1.y, Pf1.z, Pf0.w));
	float n0001 = Dot(g0001, float4(Pf0.x, Pf0.y, Pf0.z, Pf1.w));
	float n1001 = Dot(g1001, float4(Pf1.x, Pf0.y, Pf0.z, Pf1.w));
	float n0101 = Dot(g0101, float4(Pf0.x, Pf1.y, Pf0.z, Pf1.w));
	float n1101 = Dot(g1101, float4(Pf1.x, Pf1.y, Pf0.z, Pf1.w));
	float n0011 = Dot(g0011, float4(Pf0.x, Pf0.y, Pf1.z, Pf1.w));
	float n1011 = Dot(g1011, float4(Pf1.x, Pf0.y, Pf1.z, Pf1.w));
	float n0111 = Dot(g0111, float4(Pf0.x, Pf1.y, Pf1.z, Pf1.w));
	float n1111 = Dot(g1111, Pf1);

	float4 fade = Noise::Fade(Pf0);
	float4 n0w = Lerp(float4(n0000, n1000, n0100, n1100), float4(n0001, n1001, n0101, n1101), fade.w);
	float4 n1w = Lerp(float4(n0010, n1010, n0110, n1110), float4(n0011, n1011, n0111, n1111), fade.w);
	float4 nzw = Lerp(n0w, n1w, fade.z);
	float3 nyzw = Lerp(float3(nzw.x, nzw.y, 0.0f), float3(nzw.z, nzw.w, 0.0f), fade.y);
	float nxyzw = Lerp(nyzw.x, nyzw.y, fade.x);

	return 2.2f * nxyzw * 0.5f + 0.5f;
}

float Noise::PerlinFbm(float3 pos, float freq, int octaveCount)
{
	float sum = 0.0f;
	float weightSum = 0.0f;
	float weight = 0.5f;
	
	for (int i = 0; i < octaveCount; i++)
	{
		float noise = Noise::PerlinNoise(pos, freq);

		sum += noise * weight;
		weightSum += weight;
		
		weight *= weight;
		freq *= 2.0f;
	}

	float fbm = Min(sum / weightSum, 1.0f);
	fbm = Max(fbm, 0.0f);
	return fbm;
}

float Noise::WorleyNoise(float3 pos, float cellCount)
{
	const float3 pCell = pos * cellCount;
	float cellNoise = 1.0e10;

	for(int i = -1; i <=1; i++)
		for(int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
			{
				float3 tp = Floor(pCell) + float3(i, j, k);
				float3 tpMod = Mod(tp, cellCount);
				float3 inte = Floor(tpMod);
				float3 frac = Fract(tpMod);

				frac = frac * frac * (3.0f - 2.0f * frac);
				float n = inte.x + inte.y * 57.0f + inte.z * 113.0f;
				float noise = Lerp( 
					Lerp( 
						Lerp(Random::Hash(n + 0.0f), Random::Hash(n + 1.0f), frac.x),
						Lerp(Random::Hash(n + 57.0f), Random::Hash(n + 58.0f), frac.x),
						frac.y),
					Lerp(
						Lerp(Random::Hash(n + 113.0f), Random::Hash(n + 114.0f), frac.x),
						Lerp(Random::Hash(n + 170.0f), Random::Hash(n + 171.0f), frac.x),
						frac.y),
					frac.z);

				tp = pCell - tp - noise;
				cellNoise = Min(cellNoise, Dot(tp, tp));
			}

	cellNoise = Min(cellNoise, 1.0f);
	cellNoise = Max(cellNoise, 0.0f);

	return 1.0f - cellNoise;
}

float Noise::WorleyFbm(float3 pos, float cellCount, float freqs[3])
{
	float fbm = 0.0f;
	fbm += Noise::WorleyNoise(pos, cellCount * freqs[0]) * 0.625f;
	fbm += Noise::WorleyNoise(pos, cellCount * freqs[1]) * 0.25f;
	fbm += Noise::WorleyNoise(pos, cellCount * freqs[2]) * 0.125f;

	return fbm;
}

float Noise::CurlNoise(float3 pos)
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

float Exp(float a)
{
	return exp(a);
}

float3 Exp(float3 v)
{
	return float3(exp(v.x), exp(v.y), exp(v.z));
}

float4 Exp(float4 v)
{
	return float4(exp(v.x), exp(v.y), exp(v.z), exp(v.w));
}

float Sin(float a)
{
	return sinf(a);
}

float3 Sin(float3 v)
{
	return float3(sinf(v.x), sinf(v.y), sinf(v.z));
}

float4 Sin(float4 v)
{
	return float4(sinf(v.x), sinf(v.y), sinf(v.z), sinf(v.w));
}

float FastInvSqrt(float a)
{
	float half = a * 0.5f;
	int i = *(int*)&a;
	i = 0x5f3759df - (i >> 1);
	a = *(float*)&i;
	a = a * (1.5f - half * a * a);
	return a;
}

float3 FastInvSqrt(float3 a)
{
	float3 half = a * 0.5f;

	int i = *(int*)&a.x;
	i = 0x5f3759df - (i >> 1);
	a.x = *(float*)&i;
	a.x = a.x * (1.5f - half.x * a.x * a.x);

	i = *(int*)&a.y;
	i = 0x5f3759df - (i >> 1);
	a.y = *(float*)&i;
	a.y = a.y * (1.5f - half.y * a.y * a.y);

	i = *(int*)&a.z;
	i = 0x5f3759df - (i >> 1);
	a.z = *(float*)&i;
	a.z = a.z * (1.5f - half.z * a.z * a.z);

	return a;
}

float4 FastInvSqrt(float4 a)
{
	float4 half = a * 0.5f;

	int i = *(int*)&a.x;
	i = 0x5f3759df - (i >> 1);
	a.x = *(float*)&i;
	a.x = a.x * (1.5f - half.x * a.x * a.x);

	i = *(int*)&a.y;
	i = 0x5f3759df - (i >> 1);
	a.y = *(float*)&i;
	a.y = a.y * (1.5f - half.y * a.y * a.y);

	i = *(int*)&a.z;
	i = 0x5f3759df - (i >> 1);
	a.z = *(float*)&i;
	a.z = a.z * (1.5f - half.z * a.z * a.z);

	i = *(int*)&a.w;
	i = 0x5f3759df - (i >> 1);
	a.w = *(float*)&i;
	a.w = a.w * (1.5f - half.w * a.w * a.w);

	return a;
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

float Floor(float a)
{
	return floorf(a);
}

float3 Floor(float3 a)
{
	return float3(floorf(a.x), floorf(a.y), floorf(a.z));
}

float4 Floor(float4 a)
{
	return float4(floorf(a.x), floorf(a.y), floorf(a.z), floorf(a.w));
}

float Mod(float a, float b)
{
	return fmodf(a, b);
}

float3 Mod(float3 a, float b)
{
	return float3(fmodf(a.x, b), fmodf(a.y, b), fmodf(a.z, b));
}

float4 Mod(float4 a, float b)
{
	return float4(fmodf(a.x, b), fmodf(a.y, b), fmodf(a.z, b), fmodf(a.w, b));
}

float Fract(float a)
{
	return a - Floor(a);
}

float3 Fract(float3 a)
{
	return a - Floor(a);
}

float4 Fract(float4 a)
{
	return a - Floor(a);
}

float Max(float a, float b)
{
	return fmaxf(a, b);
}

float3 Max(float3 a, float3 b)
{
	return float3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
}

float4 Max(float4 a, float4 b)
{
	return float4(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z), fmaxf(a.w, b.w));
}

float Min(float a, float b)
{
	return fminf(a, b);
}

float3 Min(float3 a, float3 b)
{
	return float3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
}

float4 Min(float4 a, float4 b)
{
	return float4(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z), fminf(a.w, b.w));
}

float Abs(float a)
{
	return fabsf(a);
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

float Step(float a, float b)
{
	return a >= b ? 1.0f : 0.0f;
}

float3 Step(float3 a, float b)
{
	return float3(a.x >= b ? 1.0f : 0.0f,
		a.y >= b ? 1.0f : 0.0f,
		a.z >= b ? 1.0f : 0.0f);
}

float4 Step(float4 a, float b)
{
	return float4(a.x >= b ? 1.0f : 0.0f,
		a.y >= b ? 1.0f : 0.0f,
		a.z >= b ? 1.0f : 0.0f,
		a.w >= b ? 1.0f : 0.0f);
}

float3 Step(float3 a, float3 b)
{
	return float3(a.x >= b.x ? 1.0f : 0.0f,
		a.y >= b.y ? 1.0f : 0.0f,
		a.z >= b.z ? 1.0f : 0.0f);
}

float4 Step(float4 a, float4 b)
{
	return float4(a.x >= b.x ? 1.0f : 0.0f,
		a.y >= b.y ? 1.0f : 0.0f,
		a.z >= b.z ? 1.0f : 0.0f,
		a.w >= b.w ? 1.0f : 0.0f);
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

float Random::Hash(float a)
{
	return Fract(Sin(a + 1.951f) * 43758.5453123f);
}

float3 Random::Hash(float3 a)
{
	return Fract(Sin(a + 1.951f) * 43758.5453123f);
}

float Random::Lcg(float a)
{
	uint32_t uintA = (uint32_t)a;

	if (uintA <= 1)
		return 1;

	static uint32_t M = 4294967296;
	static uint32_t A = 214013;
	static uint32_t C = 2531011;

	return (A * (uint32_t)Random::Lcg(uintA - 1) + C) % A;
}

float3 Random::Lcg(float3 a)
{
	return float3();
}
