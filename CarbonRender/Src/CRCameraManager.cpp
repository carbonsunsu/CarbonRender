#include "..\Inc\CRCameraManager.h"

CameraManager* CameraManager::ins = nullptr;

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
	while (!cams.empty())
	{
		delete cams.top();
		cams.pop();
	}

	ins = nullptr;
}

CameraManager * CameraManager::Instance()
{
	if (ins == nullptr)
		ins = new CameraManager();

	return ins;
}

Camera* CameraManager::Push()
{
	Camera* newCam = new Camera();
	cams.push(newCam);
	return newCam;
}

void CameraManager::Pop()
{
	cams.pop();
}

Camera* CameraManager::GetCurrentCamera()
{
	if (cams.empty())
		return nullptr;

	return cams.top();
}
