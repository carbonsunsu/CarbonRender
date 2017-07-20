#include "..\Inc\CRCamera.h"

void Camera::SetPerspectiveCamera(float fov, float nearClip, float farClip)
{
	float halfFOV = fov * 0.5f;

	//read from window class later
	float wWidth = 1920.0f;
	float wHeight = 1080.0f;

	float w = nearClip * tan(halfFOV * a2r);

	float m[16] = {};

	projectionMatrix.set(m);
}
