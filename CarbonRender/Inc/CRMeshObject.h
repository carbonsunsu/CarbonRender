#ifndef CR_MESHOBJ
#define CR_MESHOBJ

#include "..\Inc\CRObject.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRGLHelper.h"
#include "..\Inc\CRMaterialManager.h"
#include "..\Inc\CRMeshManager.h"

class MeshObject : public Object
{
private:
	unsigned int materialID;
	MeshData* meshData;
public:
	MeshObject();
	~MeshObject();

	void SetMaterial(unsigned int id);
	void SetMeshData(MeshData* data);

	Material* GetMaterial();
	MeshData* GetMeshData();

	void Render(GLuint shaderProgram, bool useTex = true);
};

#endif
