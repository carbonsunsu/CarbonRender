#ifndef CR_CAMERA
#define CR_CAMERA

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRObject.h"
#include "..\Inc\CRMath.h"

enum CameraProjectMode
{
	Perspective = 0,
	Ortho,
};

class Camera : public Object
{
private:
	float fov = 60.0f;
	float nearClip = 0.01f;
	float farClip = 1000.0f;
	Matrix4x4 projectionMatrix;

	CameraProjectMode curCameraMode;
public:
	void SetPerspectiveCamera(float fov, float nearClip, float farClip);
	void SetOrthoCamera(float size);
	void LookAt(float3 p);

	void UpdateViewMatrix();
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetProjectionMatrix();
};

#endif
