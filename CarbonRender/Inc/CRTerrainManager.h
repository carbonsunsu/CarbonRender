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

	void Render(GLuint shaderProgram);
	void Update(Camera* cam);
private:
	TerrainObject();
	~TerrainObject();

	GLuint vertexArrayBuffers[8];
};

class OceanObject : public Object
{
public:
	friend class TerrainManager;

	void Render(GLuint shaderProgram);
	void Update(Camera* cam);

private:
	OceanObject();
	~OceanObject();

	GLuint vertexArrayBuffers[8];

	float3 GetIntersectionWithOcean(float3 v, float3 o, float maxRange, Camera* cam);
};

class TerrainManager
{
public:
	~TerrainManager();
	static TerrainManager* Instance();

	void CreateTerrainObject();
	void DeleteTerrainObject();
	TerrainObject* GetTerrainObject();
	bool UseTerrain();

	void CreateOceanObject();
	void DeleteOceanObject();
	OceanObject* GetOceanObject();
	bool UseOcean();
private:
	TerrainManager();

	static TerrainManager* ins;
	bool useTerrain;
	bool useOcean;

	TerrainObject* terrainObj;
	OceanObject* oceanObj;
};

#endif
