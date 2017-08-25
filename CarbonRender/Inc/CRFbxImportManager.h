#ifndef CR_FBXMANAGER
#define CR_FBXMANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRTextureManager.h"

class FbxImportManager
{
private:
	static FbxImportManager* ins;
	FbxManager* fbxManager;
	FbxIOSettings* ioSettings;

	FbxImportManager();
	void Init();
	FbxColor ReadColor(FbxMesh* mesh, int index, int vertexID);
	float4 ReadUV(FbxMesh* mesh, int index, int uvIndex);
	FbxVector4 ReadNormal(FbxMesh* mesh, int index, int vertexID);
	FbxVector4 ReadTangent(FbxMesh* mesh, int index, int vertexID);
	FbxVector4 ReadBinormal(FbxMesh * mesh, int index, int vertexID);
	void ReadTexture(FbxMesh* mesh, Mesh* crMesh, char* meshFileName);

public:
	~FbxImportManager();
	static FbxImportManager* Instance();
	bool ImportFbxModel(char* fileName, MeshObject* out);
};

#endif