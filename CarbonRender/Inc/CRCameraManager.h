#ifndef CR_CAMERAMANAGER
#define CR_CAMERAMANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRCamera.h"

class CameraManager
{
private:
	CameraManager();
	static CameraManager* ins;
	Camera cam;
public:
	~CameraManager();
	static CameraManager* Instance();
	void Push(Camera newCam);
	Camera* GetCurrentCamera();
};

#endif