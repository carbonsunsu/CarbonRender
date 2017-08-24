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
	{
		float4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
		float4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
		float4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = Translate(-transform[0], -transform[1], -transform[2]);
		modelMatrix = modelMatrix * Rotate(xAxis, -transform[6]).Normailze().ToMatrix();

		yAxis = yAxis * Rotate(xAxis, -transform[6]).Normailze().ToMatrix();
		zAxis = zAxis * Rotate(xAxis, -transform[6]).Normailze().ToMatrix();

		modelMatrix = modelMatrix * Rotate(float3(yAxis), -transform[7]).Normailze().ToMatrix();

		xAxis = xAxis * Rotate(float3(yAxis), -transform[7]).Normailze().ToMatrix();
		zAxis = zAxis * Rotate(float3(yAxis), -transform[7]).Normailze().ToMatrix();

		modelMatrix = modelMatrix * Rotate(float3(zAxis), -transform[8]).Normailze().ToMatrix();

		xAxis = xAxis * Rotate(float3(zAxis), -transform[8]).Normailze().ToMatrix();
		yAxis = zAxis * Rotate(float3(zAxis), -transform[8]).Normailze().ToMatrix();
	}
	
	//update localCoord
	{
		float4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
		float4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
		float4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

		//yAxis = yAxis * Rotate(xAxis, transform[6]).Normailze().ToMatrix();
		zAxis = zAxis * Rotate(xAxis, transform[6]).Normailze().ToMatrix();

		xAxis = xAxis * Rotate(yAxis, transform[7]).Normailze().ToMatrix();
		zAxis = zAxis * Rotate(yAxis, transform[7]).Normailze().ToMatrix();

		xAxis = xAxis * Rotate(zAxis, transform[8]).Normailze().ToMatrix();
		//yAxis = zAxis * Rotate(zAxis, transform[8]).Normailze().ToMatrix();

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
}

Matrix4x4 Camera::GetViewMatrix()
{
	return modelMatrix;
}

Matrix4x4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
