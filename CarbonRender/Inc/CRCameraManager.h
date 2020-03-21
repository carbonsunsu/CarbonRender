#ifndef CR_CAMERAMANAGER
#define CR_CAMERAMANAGER

#include "..\Inc\CRCamera.h"

class CameraManager
{
private:
	CameraManager();
	static CameraManager* ins;
	stack<Camera*> cams;
public:
	~CameraManager();
	static CameraManager* Instance();
	Camera* Push();
	void Pop();
	Camera* GetCurrentCamera();
};

#endif