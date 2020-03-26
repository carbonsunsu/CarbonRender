#ifndef CR_CAMERA
#define CR_CAMERA

#include "..\Inc\CRObject.h"
#include "..\Inc\CRWindowManager.h"

enum CameraProjectMode
{
	ePerspective = 0,
	eOrtho,
};

class Camera : public Object
{
private:
	float fov = 60.0f;//vertical direction
	float orthoSize = 5.0f;//half of height
	float nearClip = 0.01f;
	float farClip = 1000.0f;
	float wInPixel = 1920.0f;
	float hInPixel = 1080.0f;
	Matrix4x4 projectionMatrix;

	CameraProjectMode curCameraMode;

	float3 nearClipPtLT;
	float3 nearClipPtLB;
	float3 nearClipPtRT;
	float3 nearClipPtRB;

	float3 groundPtLT;
	float3 groundPtLB;
	float3 groundPtRT;
	float3 groundPtRB;

	float3 GetIntersectionPoint(float3 v, float3 o, float3 terrainPlane);
public:
	Camera();
	~Camera();
	void SetPerspectiveCamera(float iFov, float iNearClip, float iFarClip);
	void SetPerspectiveCamera(float iFov, float iNearClip, float iFarClip, int size);
	void SetNearClip(float iNearClip);
	float GetNearClip();
	void SetFarClip(float iFarClip);
	float GetFarClip();
	void SetFov(float iFov);
	float GetFov();
	void SetOrthoSize(float size);
	float3 GetCameraPara();
	void SetOrthoCamera(float size, float iNearClip, float iFarClip);
	void SetOrthoCamera(float size, float iNearClip, float iFarClip, float sideSize);

	void UpdateProjectionMatrix();
	void UpdateViewMatrix();
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetProjectionMatrix();

	float3 GetIntersectionPtLT();
	float3 GetIntersectionPtLB();
	float3 GetIntersectionPtRT();
	float3 GetIntersectionPtRB();

	void UpdateIntersectionWithTerrain(float3 terrainPlane);
};

#endif
