#include "..\Inc\CRMeshManager.h"

MeshData::MeshData()
{
	bReady4Rending = false;

	for (int i = 0; i < 8; i++)
		vertexArrayBuffers[i] = -1;

	vertexCount = 0;
	polygonCount = 0;

	indexArray = nullptr;
	vertexArray = nullptr;
	colorArray = nullptr;
	normalArray = nullptr;
	tangentArray = nullptr;
	binormalArray = nullptr;
	uvArray = nullptr;
}

MeshData::~MeshData()
{
	if (bReady4Rending)
	{
		glDeleteVertexArrays(1, vertexArrayBuffers);
		glDeleteBuffers(7, vertexArrayBuffers + 1);
	}

	for (int i = 0; i < 8; i++)
		vertexArrayBuffers[i] = -1;

	vertexCount = 0;
	polygonCount = 0;

	delete[] indexArray;
	delete[] vertexArray;
	delete[] colorArray;
	delete[] normalArray;
	delete[] tangentArray;
	delete[] binormalArray;
	delete[] uvArray;
}

void MeshData::CreateArraies()
{
	CreateIndexArray(polygonCount * 3);
	CreateVertexArray(vertexCount * 3);
	CreateVertexColorArray(vertexCount * 4);
	CreateNormalArray(vertexCount * 3);
	CreateTangentArray(vertexCount * 3);
	CreateBinormalArray(vertexCount * 3);
	CreateUVArray(vertexCount * 4);
}

void MeshData::CreateIndexArray(unsigned int size)
{
	indexArray = new unsigned int[size];
}

void MeshData::CreateVertexArray(unsigned int size)
{
	vertexArray = new float[size];
}

void MeshData::CreateVertexColorArray(unsigned int size)
{
	colorArray = new float[size];
}

void MeshData::CreateNormalArray(unsigned int size)
{
	normalArray = new float[size];
}

void MeshData::CreateTangentArray(unsigned int size)
{
	tangentArray = new float[size];
}

void MeshData::CreateBinormalArray(unsigned int size)
{
	binormalArray = new float[size];
}

void MeshData::CreateUVArray(unsigned int size)
{
	uvArray = new float[size];
}

void MeshData::SetVertexCount(unsigned int count)
{
	vertexCount = Math::Max(count, 0);
}

void MeshData::SetPolygonCount(unsigned int count)
{
	polygonCount = Math::Max(count, 0);
}

void MeshData::SetIndexAt(unsigned int i, unsigned index)
{
	indexArray[i] = index;
}

void MeshData::SetPath(string pathStr, string subMeshStr)
{
	meshPath = pathStr;
	subMeshName = subMeshStr;
}

void MeshData::CopyToIndexArray(unsigned int * data)
{
	memcpy(indexArray, data, sizeof(unsigned int) * GetPolygonCount() * 3);
}

void MeshData::CopyToVertexArray(float * data)
{
	memcpy(vertexArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshData::CopyToVertexColorArray(float * data)
{
	memcpy(colorArray, data, sizeof(float) * GetVertexCount() * 4);
}

void MeshData::CopyToNormalrray(float * data)
{
	memcpy(normalArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshData::CopyToTangentArray(float * data)
{
	memcpy(tangentArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshData::CopyToBinormalArray(float * data)
{
	memcpy(binormalArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshData::CopyToUVArray(float * data)
{
	memcpy(uvArray, data, sizeof(float) * GetVertexCount() * 4);
}

GLuint MeshData::GetVertexArrayObject()
{
	return vertexArrayBuffers[0];
}

GLuint MeshData::GetElementBufferObject()
{
	return vertexArrayBuffers[1];
}

unsigned int MeshData::GetVertexCount()
{
	return vertexCount;
}

unsigned int MeshData::GetPolygonCount()
{
	return polygonCount;
}

unsigned int * MeshData::GetIndexArray()
{
	return indexArray;
}

float * MeshData::GetVertexArray()
{
	return vertexArray;
}

float * MeshData::GetColorArray()
{
	return colorArray;
}

float * MeshData::GetNormalArray()
{
	return normalArray;
}

float * MeshData::GetTangentArray()
{
	return tangentArray;
}

float * MeshData::GetBinormalArray()
{
	return binormalArray;
}

float * MeshData::GetUVArray()
{
	return uvArray;
}

string MeshData::GetPath()
{
	return meshPath;
}

string MeshData::GetSubMeshName()
{
	return subMeshName;
}

void MeshData::GetReady4Rending()
{
	if (bReady4Rending)
		return;

	glGenVertexArrays(1, vertexArrayBuffers);
	glGenBuffers(7, vertexArrayBuffers + 1);

	glBindVertexArray(vertexArrayBuffers[0]);

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[2], sizeof(float)*vertexCount * 3, vertexArray, 3, GL_FLOAT, CR_VERTATTRIPOS_POS);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_POS);

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[3], sizeof(float)*vertexCount * 4, colorArray, 4, GL_FLOAT, CR_VERTATTRIPOS_COL);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_COL);

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[4], sizeof(float)*vertexCount * 4, uvArray, 4, GL_FLOAT, CR_VERTATTRIPOS_UVS);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_UVS);

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[5], sizeof(float)*vertexCount * 3, normalArray, 3, GL_FLOAT, CR_VERTATTRIPOS_NOR);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_NOR);

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[6], sizeof(float)*vertexCount * 3, tangentArray, 3, GL_FLOAT, CR_VERTATTRIPOS_TAG);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_TAG);

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[7], sizeof(float)*vertexCount * 3, binormalArray, 3, GL_FLOAT, CR_VERTATTRIPOS_BNL);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_BNL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*polygonCount * 3, indexArray, GL_STATIC_DRAW);

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	bReady4Rending = true;
}

MeshManager* MeshManager::ins = nullptr;

MeshManager::MeshManager()
{
	InitBuildinBoxMesh();
}

void MeshManager::InitBuildinBoxMesh()
{
	float vertexPos[72] = {
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f
	};

	float vertexUV[96] = {
		1.0f, 1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, -1.0f,
		1.0f, 0.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, -1.0f,
		1.0f, 0.0f, -1.0f, -1.0f,
		0.0f, 0.0f, -1.0f, -1.0f,
		1.0f, 0.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, -1.0f,
		1.0f, 0.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, -1.0f,
		0.0f, 0.0f, -1.0f, -1.0f,
		1.0f, 0.0f, -1.0f, -1.0f,
		0.0f, 0.0f, -1.0f, -1.0f,
		0.0f, 0.0f, -1.0f, -1.0f,
		0.0f, 0.0f, -1.0f, -1.0f,
		0.0f, 0.0f, -1.0f, -1.0f
	};

	float vertexColor[96] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	float vertexNormal[72] = {
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,		
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f
	};

	float vertexTangent[72] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	float vertexBinormal[72] = {
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		-0.0f, -0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	unsigned int index[36] = {
		1, 2, 0,
		3, 6, 8,
		7, 4, 9,
		5, 10, 11,
		12, 13, 14,
		15, 16, 17,
		1, 18, 2,
		3, 19, 6,
		7, 20, 4,
		5, 21, 10,
		12, 22, 13,
		15, 23, 16
	};

	buildinBox.SetVertexCount(24);
	buildinBox.SetPolygonCount(12);

	buildinBox.CreateIndexArray(36);
	buildinBox.CreateVertexArray(72);
	buildinBox.CreateVertexColorArray(96);
	buildinBox.CreateUVArray(96);
	buildinBox.CreateNormalArray(72);
	buildinBox.CreateTangentArray(72);
	buildinBox.CreateBinormalArray(72);

	buildinBox.CopyToIndexArray(index);
	buildinBox.CopyToVertexArray(vertexPos);
	buildinBox.CopyToVertexColorArray(vertexColor);
	buildinBox.CopyToUVArray(vertexUV);
	buildinBox.CopyToNormalrray(vertexNormal);
	buildinBox.CopyToTangentArray(vertexTangent);
	buildinBox.CopyToBinormalArray(vertexBinormal);

	buildinBox.GetReady4Rending();
}

void MeshManager::InitBuildinSphereMesh()
{
	float radius = 0.5f;
	int slices = 360;
	int stacks = 360;

	int vertexCount = slices * (2 * stacks - 1) + 2;
	int polygonCount = slices * (stacks * 4 - 2);

	unsigned int* index = new unsigned int[polygonCount * 3];
	float* vertexPos = new float[vertexCount * 3];
	float* vertexUV = new float[vertexCount * 4];
	float* vertexColor = new float[vertexCount * 4];
	float* vertexNormal = new float[vertexCount * 3];
	float* vertexTangent = new float[vertexCount * 3];
	float* vertexBinormal = new float[vertexCount * 3];

	for (int i = 0; i < slices; i++)
	{
		for (int j = 0; j < stacks; j++)
		{
			 
		}
	}

	buildinSphere.SetVertexCount(vertexCount);
	buildinSphere.SetPolygonCount(polygonCount);

	buildinSphere.CreateIndexArray(polygonCount * 3);
	buildinSphere.CreateVertexArray(vertexCount * 3);
	buildinSphere.CreateVertexColorArray(vertexCount * 4);
	buildinSphere.CreateUVArray(vertexCount * 4);
	buildinSphere.CreateNormalArray(vertexCount * 3);
	buildinSphere.CreateTangentArray(vertexCount * 3);
	buildinSphere.CreateBinormalArray(vertexCount * 3);

	
}

MeshManager::~MeshManager()
{
	for (unordered_map<string, MeshData*>::iterator i = meshDatas.begin(); i != meshDatas.end(); i++)
		delete i->second;

	meshDatas.clear();
	ins = nullptr;
}

MeshManager * MeshManager::Instance()
{
	if (ins == nullptr)
		ins = new MeshManager();

	return ins;
}

MeshData * MeshManager::GetMeshData(string meshPath, string subMeshName)
{
	char* index = FileReader::BindString((char*)meshPath.c_str(), ":");
	index = FileReader::BindString(index, (char*)subMeshName.c_str());

	if (meshDatas.find(index) != meshDatas.end())
		return meshDatas[index];
	else
		return nullptr;
}

MeshData * MeshManager::CreateNewMeshData(string meshPath, string subMeshName)
{
	char* index = FileReader::BindString((char*)meshPath.c_str(), ":");
	index = FileReader::BindString(index, (char*)subMeshName.c_str());

	MeshData* newMeshData = new MeshData();
	newMeshData->SetPath(meshPath, subMeshName);
	meshDatas[index] = newMeshData;

	return newMeshData;
}

bool MeshManager::Find(string meshPath, string subMeshName)
{
	char* index = FileReader::BindString((char*)meshPath.c_str(), ":");
	index = FileReader::BindString(index, (char*)subMeshName.c_str());

	return meshDatas.find(index) != meshDatas.end();
}

MeshData * MeshManager::GetBuildinBox()
{
	return &buildinBox;
}

MeshData * MeshManager::GetBuildinSphere()
{
	return &buildinSphere;
}
