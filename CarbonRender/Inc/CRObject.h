#ifndef CR_OBJECT
#define CR_OBJECT

#include "..\Inc\CRMath.h"

class Object
{
protected:
	float transform[9] = { 0.0f, 0.0f, 0.0f,	//position
						   1.0f, 1.0f, 1.0f,	//scale
						   0.0f, 0.0f, 0.0f};	//rotation

	float FRU[9] = { 0.0f, 0.0f, -1.0f,
					1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f };

	Matrix4x4 modelMatrix;
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