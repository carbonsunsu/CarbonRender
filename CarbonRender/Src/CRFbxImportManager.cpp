#include "..\Inc\CRFbxImportManager.h"

FbxImportManager* FbxImportManager::ins = nullptr;

FbxImportManager::FbxImportManager()
{
	Init();
}

FbxImportManager::~FbxImportManager()
{
	ins = nullptr;
	fbxManager->Destroy();

	return;
}

FbxImportManager * FbxImportManager::instance()
{
	if (ins == nullptr)
		ins = new FbxImportManager();

	return ins;
}

void FbxImportManager::Init()
{
	fbxManager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ioSettings);

	FbxString ex = "dll";
	FbxString path = FbxGetApplicationDirectory();
	fbxManager->LoadPluginsDirectory(path.Buffer(), ex.Buffer());



	return;
}
