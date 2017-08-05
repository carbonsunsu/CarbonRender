#ifndef CR_MESHOBJ
#define CR_MESHOBJ

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRObject.h"

struct Mesh
{
public:
	string name;
	float3 translation;
	float3 rotation;
	float3 scaling;
	int vertexCount = 0;
	int polygonCount = 0;
	int* index;
	float* vertex;
	float* color;
	float* normal;
	float* tangent;
	float* uv;
};

class MeshObject : Object
{
public:
	int childCount = 0;
	Mesh* child;
};

#endif
