#ifndef CR_FBXMANAGER
#define CR_FBXMANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMeshObject.h"

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

public:
	~FbxImportManager();
	static FbxImportManager* instance();
	bool importFbxModel(char* fileName, MeshObject* out);
};

#endif