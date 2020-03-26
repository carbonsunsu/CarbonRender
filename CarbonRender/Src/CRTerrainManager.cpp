#include "..\Inc\CRTerrainManager.h"

TerrainManager* TerrainManager::ins = nullptr;

TerrainManager * TerrainManager::Instance()
{
	if (ins == nullptr)
		ins = new TerrainManager();

	return ins;
}

void TerrainManager::CreateTerrainObject()
{
	if (terrainObj == nullptr)
	{
		terrainObj = new TerrainObject();
		useTerrain = true;
	}
}

void TerrainManager::DeleteTerrainObject()
{
	delete terrainObj;
	useTerrain = false;
}

TerrainObject * TerrainManager::GetTerrainObject()
{
	return terrainObj;
}

bool TerrainManager::UseTerrain()
{
	return useTerrain;
}

TerrainManager::TerrainManager()
{
	CreateTerrainObject();
}

TerrainManager::~TerrainManager()
{
	delete terrainObj;
	terrainObj = nullptr;
	useTerrain = false;
	ins = nullptr;
}

void TerrainObject::Render(GLuint shader)
{
	testBoxLT.Render(shader);
	testBoxLB.Render(shader);
	testBoxRB.Render(shader);
	testBoxRT.Render(shader);
}

void TerrainObject::Update(Camera * cam)
{
	testBoxLT.SetPosition(cam->GetIntersectionPtLT());
	testBoxLB.SetPosition(cam->GetIntersectionPtLB());
	testBoxRT.SetPosition(cam->GetIntersectionPtRT());
	testBoxRB.SetPosition(cam->GetIntersectionPtRB());
}

TerrainObject::TerrainObject()
{
	testBoxLB.SetMeshData(MeshManager::Instance()->GetBuildinBox());
	testBoxLB.SetMaterial(MaterialManager::Instance()->GetDefaultMaterial());

	testBoxLT.SetMeshData(MeshManager::Instance()->GetBuildinBox());
	testBoxLT.SetMaterial(MaterialManager::Instance()->GetDefaultMaterial());

	testBoxRT.SetMeshData(MeshManager::Instance()->GetBuildinBox());
	testBoxRT.SetMaterial(MaterialManager::Instance()->GetDefaultMaterial());

	testBoxRB.SetMeshData(MeshManager::Instance()->GetBuildinBox());
	testBoxRB.SetMaterial(MaterialManager::Instance()->GetDefaultMaterial());
}

TerrainObject::~TerrainObject()
{
}