#ifndef CR_OBJECT
#define CR_OBJECT

#include "..\Inc\CRMath.h"

class Object
{
private:
	float transform[9] = { 0.0f, 0.0f, 0.0f,
						   1.0f, 1.0f, 1.0f,
						   0.0f, 0.0f, 0.0f};

	float FRU[9] = { 0.0f, 0.0f, -1.0f,
					1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f };

	float modelMatrix[16];
public:
	Object();
	void SetPosition(float3 p);
	void SetScale(float3 s);
	void SetRotation(float3 r);
	void UpdateModelMatrix();

	float3 GetPosition();
	float3 GetScale();
	float3 GetRotation();
	Matrix4x4 GetModelMatrix();
};

#endif