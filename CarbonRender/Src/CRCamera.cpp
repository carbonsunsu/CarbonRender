#include "..\Inc\CRCamera.h"

void Camera::SetPerspectiveCamera(float iFov, float iNearClip, float iFarClip)
{
	fov = iFov;
	nearClip = iNearClip;
	farClip = iFarClip;
	curCameraMode = CameraProjectMode::Perspective;

	UpdateProjectionMatrix();
}

void Camera::SetNearFar(float iNearClip, float iFarClip)
{
	nearClip = iNearClip;
	farClip = iFarClip;

	UpdateProjectionMatrix();
}

void Camera::SetFov(float iFov)
{
	fov = iFov;

	UpdateProjectionMatrix();
}

float3 Camera::GetCameraPara()
{
	return float3(fov, nearClip, farClip);
}

void Camera::SetOrthoCamera(float size)
{
	curCameraMode = CameraProjectMode::Ortho;
}

void Camera::UpdateProjectionMatrix()
{
	switch (curCameraMode)
	{
	default:
		break;
	case CameraProjectMode::Perspective:
	{
		float halfFOV = fov * 0.5f;
		WindowSize wSize = WindowManager::Instance()->GetWindowSize();
		wInPixel = wSize.w;
		hInPixel = wSize.h;

		float hT = 1.0f / (nearClip * tan(halfFOV * A2R) * 2.0f);
		float wT = hInPixel*hT / wInPixel;

		float m[16] = { 2.0f*nearClip*wT, 0.0f, 0.0f, 0.0f,
						0.0f, 2.0f*nearClip*hT, 0.0f, 0.0f,
						0.0f, 0.0f, (farClip + nearClip) / (nearClip - farClip), -1.0f,
						0.0f, 0.0f, 2 * nearClip*farClip / (nearClip - farClip), 0.0f };

		projectionMatrix = m;
	}
	break;
	case CameraProjectMode::Ortho:
	{

	}
	break;
	}
}

void Camera::UpdateViewMatrix()
{
	modelMatrix = Translate(-transform[0], -transform[1], -transform[2]) * 
				  Rotate(float3(1.0f, 0.0f, 0.0f), -transform[6]).Normailze().ToMatrix() *
				  Rotate(float3(0.0f, 1.0f, 0.0f), -transform[7]).Normailze().ToMatrix() *
				  Rotate(float3(0.0f, 0.0f, 1.0f), -transform[8]).Normailze().ToMatrix();
}

Matrix4x4 Camera::GetViewMatrix()
{
	return modelMatrix;
}

Matrix4x4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
