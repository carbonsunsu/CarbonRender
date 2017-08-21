#include "..\Inc\CRObject.h"

Object::Object(){}

void Object::SetPosition(float3 p)
{
	transform[0] = p.x;
	transform[1] = p.y;
	transform[2] = p.z;
}

void Object::SetScale(float3 s)
{
	transform[3] = s.x;
	transform[4] = s.y;
	transform[5] = s.z;
}

void Object::SetRotation(float3 r)
{
	transform[6] = r.x;
	transform[7] = r.y;
	transform[8] = r.z;
}

void Object::UpdateModelMatrix()
{
	modelMatrix = CalculateModelMatrix(float3(transform[0], transform[1], transform[2]),
										float3(transform[6], transform[7], transform[8]),
										float3(transform[3], transform[4], transform[5]));
}

void Object::UpdateLocalCoord()
{
	float4 xAxis(localCoord[0], localCoord[1], localCoord[2], 0.0f);
	xAxis = xAxis * modelMatrix;
	float4 yAxis(localCoord[3], localCoord[4], localCoord[5], 0.0f);
	yAxis = yAxis * modelMatrix;
	float4 zAxis(localCoord[6], localCoord[7], localCoord[8], 0.0f);
	zAxis = zAxis * modelMatrix;

	localCoord[0] = xAxis.x;
	localCoord[1] = xAxis.y;
	localCoord[2] = xAxis.z;

	localCoord[3] = yAxis.x;
	localCoord[4] = yAxis.y;
	localCoord[5] = yAxis.z;

	localCoord[6] = zAxis.x;
	localCoord[7] = zAxis.y;
	localCoord[8] = zAxis.z;
}

float3 Object::GetPosition()
{
	float3 result;
	result.x = transform[0];
	result.y = transform[1];
	result.z = transform[2];

	return result;
}

float3 Object::GetScale()
{
	float3 result;
	result.x = transform[3];
	result.y = transform[4];
	result.z = transform[5];

	return result;
}

float3 Object::GetRotation()
{
	float3 result;
	result.x = transform[6];
	result.y = transform[7];
	result.z = transform[8];

	return result;
}

float3 Object::GetForward()
{
	return float3(-localCoord[6], -localCoord[7], -localCoord[8]);
}

float3 Object::GetRight()
{
	return float3(localCoord[0], localCoord[1], localCoord[2]);
}

float3 Object::GetUp()
{
	return float3(localCoord[3], localCoord[4], localCoord[5]);
}

Matrix4x4 Object::GetModelMatrix()
{
	return modelMatrix;
}
