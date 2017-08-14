#include "..\Inc\CRCameraManager.h"

CameraManager* CameraManager::ins = nullptr;

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{

	ins = nullptr;
}

CameraManager * CameraManager::Instance()
{
	if (ins == nullptr)
		ins = new CameraManager();

	return ins;
}

void CameraManager::Push(Camera newCam)
{
	this->cam = newCam;
}

Camera* CameraManager::GetCurrentCamera()
{
	return &cam;
}
