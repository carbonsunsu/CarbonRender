#ifndef CR_MESH_MANAGER
#define CR_MESH_MANAGER

#include "..\Inc\CRMath.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRGLHelper.h"

class MeshData
{
private:
	GLuint vao;
	GLuint ebo;
	GLuint vBuffer;
	GLuint cBuffer;
	GLuint uvBuffer;
	GLuint nBuffer;
	GLuint tBuffer;
	GLuint bBuffer;

	unsigned int vertexCount;
	unsigned int polygonCount;
	unsigned int* indexArray;
	float* vertexArray;
	float* colorArray;
	float* normalArray;
	float* tangentArray;
	float* binormalArray;
	float* uvArray;

	string meshPath;
	string subMeshName;

	bool bReady4Rending;
public:
	MeshData();
	~MeshData();

	void CreateArraies();
	void CreateIndexArray(unsigned int size);
	void CreateVertexArray(unsigned int size);
	void CreateVertexColorArray(unsigned int size);
	void CreateNormalArray(unsigned int size);
	void CreateTangentArray(unsigned int size);
	void CreateBinormalArray(unsigned int size);
	void CreateUVArray(unsigned int size);

	void SetVertexCount(unsigned int count);
	void SetPolygonCount(unsigned int count);
	void SetIndexAt(unsigned int i, unsigned index);
	void SetPath(string pathStr, string subMeshStr);

	void CopyToVertexArray(float* data);
	void CopyToVertexColorArray(float* data);
	void CopyToNormalrray(float* data);
	void CopyToTangentArray(float* data);
	void CopyToBinormalArray(float* data);
	void CopyToUVArray(float* data);

	GLuint GetVertexArrayObject();
	GLuint GetElementBufferObject();
	unsigned int GetVertexCount();
	unsigned int GetPolygonCount();
	string GetPath();
	string GetSubMeshName();

	void GetReady4Rending();
};

class MeshManager
{
private:
	MeshManager();
	static MeshManager* ins;

	unordered_map<string, MeshData*> meshDatas;

public:
	~MeshManager();
	static MeshManager* Instance();

	void AddMeshData(MeshData* data);

	MeshData* GetMeshData(string meshPath, string subMeshName);
};

#endif
