#ifndef CR_MESHOBJ
#define CR_MESHOBJ

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRObject.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRCameraManager.h"

struct Mesh
{
public:
	string name;
	float3 translation;
	float3 rotation;
	float3 scaling;
	Matrix4x4 modelMatrix;
	int vertexCount = 0;
	int polygonCount = 0;
	unsigned int* index;
	float* vertex;
	float* color;
	float* normal;
	float* tangent;
	float* uv;
};

class MeshObject : public Object
{
private:
	uint16_t childCount = 0;
	Mesh* child;
	GLuint* vaos;
	GLuint* ebos;
	GLuint* vbs;
	GLuint* cbs;
	GLuint* ubs;
	GLuint* nbs;
	GLuint* tbs;
	GLuint shaderProgram;
	bool bReady4Render = false;
public:
	MeshObject();
	~MeshObject();
	void SetChildCount(uint16_t count);
	void SetChild(Mesh newChild, unsigned int index);
	uint16_t GetChildCount();
	Mesh* GetChild(unsigned int index);
	Mesh* GetAllChild();
	void GetReady4Rending();
	void Render();
	void AttachShader(GLuint shader);
};

#endif
