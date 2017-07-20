#ifndef CR_CAMERA
#define CR_CAMERA

#include "..\Inc\CRObject.h"
#include "..\Inc\CRMath.h"

enum CameraProjectMode
{
	Perspective,
	Ortho,
};

class Camera : Object
{
private:
	Matrix4x4 projectionMatrix;

	CameraProjectMode curCameraMode;
public:
	void SetPerspectiveCamera(float fov, float nearClip, float farClip);
	void SetOrthoCamera();
	void LookAt(float3 p);

	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetPerspectiveMatrix();
};

#endif
