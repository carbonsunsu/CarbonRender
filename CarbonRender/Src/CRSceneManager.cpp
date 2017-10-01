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
	cam.SetPerspectiveCamera(60.0f, 0.01f, 3000.0f);
	cam.SetPosition(float3(0.0f, 2.0f, 0.0f));
	CameraManager::Instance()->Push(cam);

	Controller ctrl;
	ctrl.Init();
	ControllerManager::Instance()->Push(ctrl);
	//return;
	FbxImportManager::Instance()->ImportFbxModel("terrain", &terrain);
	terrain.GetReady4Rending();
	return;
	FbxImportManager::Instance()->ImportFbxModel("Sponza", &sponza);
	sponza.GetReady4Rending();
	sponza.SetPosition(float3(-1.0f, -0.2f, 0.0f));
	sponza.SetRotation(float3(0.0f, 180.0f, 0.0f));

	FbxImportManager::Instance()->ImportFbxModel("Type59", &type59);
	type59.GetReady4Rending();
	type59.SetPosition(float3(-1.0f, -0.2f, 0.0f));
	type59.SetRotation(float3(0.0f, 0.0f, 0.0f));

	FbxImportManager::Instance()->ImportFbxModel("dragon", &dragon);
	dragon.GetReady4Rending();
	dragon.SetPosition(float3(-2.0f, 0.1f, 14.0f));
	dragon.SetRotation(float3(0.0f, 180.0f, 0.0f));
}

void SceneManager::DrawScene(GLuint shaderProgram)
{
	//return;
	terrain.Render(shaderProgram);
	return;
	sponza.Render(shaderProgram);
	type59.Render(shaderProgram);
	dragon.Render(shaderProgram);
}
