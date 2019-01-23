#ifndef CR_MESHOBJ
#define CR_MESHOBJ

#include "..\Inc\CRObject.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRGLHelper.h"
#include "..\Inc\CRMaterialManager.h"

class MeshObject : public Object
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

	unsigned int vertexCount = 0;
	unsigned int polygonCount = 0;
	unsigned int* indexArray;
	float* vertexArray;
	float* colorArray;
	float* normalArray;
	float* tangentArray;
	float* binormalArray;
	float* uvArray;
	GLuint texs[3];
	Material* material;
	string path;
	string subMeshName;

	bool bReady4Render = false;
public:
	MeshObject();
	~MeshObject();
	void Delete();

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
	void SetTexture(unsigned int i, GLuint tex);
	void SetIndexAt(unsigned int i, unsigned index);
	void SetPath(string pathStr, string subMeshStr);

	void CopyToVertexArray(float* data);
	void CopyToVertexColorArray(float* data);
	void CopyToNormalrray(float* data);
	void CopyToTangentArray(float* data);
	void CopyToBinormalArray(float* data);
	void CopyToUVArray(float* data);

	unsigned int GetVertexCount();
	unsigned int GetPolygonCount();
	GLuint GetTexture(unsigned int i);
	string GetPath();
	string GetSubMeshName();

	void GetReady4Rending();

	void Render(GLuint shaderProgram, bool useTex = true);
};

#endif
