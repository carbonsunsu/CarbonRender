#include "..\Inc\CRCamera.h"

void Camera::SetPerspectiveCamera(float fov, float nearClip, float farClip)
{
	float halfFOV = fov * 0.5f;

	//read from window class later
	float wWidth = 1920.0f;
	float wHeight = 1080.0f;

	float w = nearClip * tan(halfFOV * A2R);

	float m[16] = {};

	projectionMatrix = m;
}

void Camera::UpdateViewMatrix()
{
	modelMatrix = Translate(transform[0], transform[1], transform[2]) *
				  Rotate(float3(1.0f, 0.0f, 0.0f), transform[6]).Normailze().ToMatrix() *
				  Rotate(float3(0.0f, 1.0f, 0.0f), transform[7]).Normailze().ToMatrix() *
				  Rotate(float3(0.0f, 0.0f, 1.0f), transform[8]).Normailze().ToMatrix();
}

Matrix4x4 Camera::GetViewMatrix()
{
	return modelMatrix;
}

Matrix4x4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
