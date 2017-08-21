#include "..\Inc\CRCameraManager.h"

CameraManager* CameraManager::ins = nullptr;

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
	while (!cams.empty())
		cams.pop();

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
	cams.push(newCam);
}

void CameraManager::Pop()
{
	cams.pop();
}

Camera* CameraManager::GetCurrentCamera()
{
	if (cams.empty())
		return nullptr;

	return &cams.top();
}
