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

	FbxImportManager::Instance()->ImportFbxModel("Plane", &terrain);
	terrain.GetReady4Rending();

	FbxImportManager::Instance()->ImportFbxModel("HappyRecon", &recon);
	recon.GetReady4Rending();
	recon.SetPosition(float3(-6.0f, 0.0f, 6.0f));

	FbxImportManager::Instance()->ImportFbxModel("dragon", &dragon);
	dragon.GetReady4Rending();
	dragon.SetPosition(float3(-6.0f, 0.0f, -4.0f));
	dragon.SetRotation(float3(0.0f, 90.0f, 0.0f));

	FbxImportManager::Instance()->ImportFbxModel("Sponza", &sponza);
	sponza.GetReady4Rending();
	sponza.SetRotation(float3(0.0f, 180.0f, 0.0f));

	FbxImportManager::Instance()->ImportFbxModel("Type59", &type59);
	type59.GetReady4Rending();
}

void SceneManager::DrawScene(GLuint shaderProgram)
{
	terrain.Render(shaderProgram);
	sponza.Render(shaderProgram);
	dragon.Render(shaderProgram);
	type59.Render(shaderProgram);
	recon.Render(shaderProgram);
}
