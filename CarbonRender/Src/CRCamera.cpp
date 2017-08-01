#include "..\Inc\CRCamera.h"

void Camera::SetPerspectiveCamera(float fov, float nearClip, float farClip)
{
	curCameraMode = Perspective;

	float halfFOV = fov * 0.5f;

	//read from window class later
	float wWidth = 1920.0f;
	float wHeight = 1080.0f;

	float hT = 1.0f / (nearClip * tan(halfFOV * A2R) * 2.0f); 
	float wT = wHeight*hT / wWidth;

	float m[16] = {2.0f*nearClip*wT, 0.0f, 0.0f, 0.0f,
				   0.0f, 2.0f*nearClip*hT, 0.0f, 0.0f,
				   0.0f, 0.0f, (farClip+nearClip)/(nearClip-farClip), -1.0f,
				   0.0f, 0.0f, 2*nearClip*farClip/(nearClip-farClip), 0.0f};

	projectionMatrix = m;
}

void Camera::SetOrthoCamera(float size)
{
	curCameraMode = Ortho;
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
