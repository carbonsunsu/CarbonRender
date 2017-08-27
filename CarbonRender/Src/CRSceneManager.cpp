#include "..\Inc\CRSceneManager.h"

SceneManager* SceneManager::ins = nullptr;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

SceneManager * SceneManager::Instance()
{
	if (ins == nullptr)
		ins = new SceneManager();

	return ins;
}

void SceneManager::Init()
{
}

void SceneManager::LoadScene()
{
	Camera cam;
	cam.SetPerspectiveCamera(60.0f, 0.01f, 10000.0f);
	cam.SetPosition(float3(0.0f, 2.0f, 0.0f));
	CameraManager::Instance()->Push(cam);

	Controller ctrl;
	ctrl.Init();
	ControllerManager::Instance()->Push(ctrl);

	FbxImportManager::Instance()->ImportFbxModel("Terrain", &scene);
	FbxImportManager::Instance()->ImportFbxModel("Type59", &type59);
	scene.GetReady4Rending();
	type59.GetReady4Rending();

	type59.SetPosition(float3(-1.0f, -0.6f, -17.5f));
}

void SceneManager::DrawScene(GLuint shaderProgram)
{
	scene.Render(shaderProgram);
	type59.Render(shaderProgram);
}
