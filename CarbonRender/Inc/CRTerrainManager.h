#ifndef CR_TERRAIN_MANAGER
#define CR_TERRAIN_MANAGER

#include "..\Inc\CRObject.h"
#include "..\Inc\CRCamera.h"
#include "..\Inc\CRWindowManager.h"

#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRMeshManager.h"

class TerrainObject : public Object
{
public:
	friend class TerrainManager;

	void Render(GLuint shader);
	void Update(Camera* cam);
private:
	TerrainObject();
	~TerrainObject();

	MeshObject testBoxLT;
	MeshObject testBoxLB;
	MeshObject testBoxRT;
	MeshObject testBoxRB;

	MeshObject terrainMeshObj;
	MeshData terrainMeshData;
};

class TerrainManager
{
public:
	static TerrainManager* Instance();

	void CreateTerrainObject();
	void DeleteTerrainObject();
	TerrainObject* GetTerrainObject();
	bool UseTerrain();
private:
	TerrainManager();
	~TerrainManager();

	static TerrainManager* ins;
	bool useTerrain;

	TerrainObject* terrainObj;
};

#endif
