#ifndef CR_FBXMANAGER
#define CR_FBXMANAGER

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
	FbxVector4 ReadBinormal(FbxMesh * mesh, int index, int vertexID);
	void ReadTexture(FbxMesh* mesh, MeshObject* meshObj, char* meshFileName);

public:
	~FbxImportManager();
	static FbxImportManager* Instance();
	int ImportFbxModel(char* filePath, Object* root, bool newAMeshObj = true);
	MeshData* ImportFbxModel(string pathStr, string subMeshStr);
};

#endif