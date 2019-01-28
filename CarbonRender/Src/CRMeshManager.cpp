#include "..\Inc\CRMeshManager.h"

MeshData::MeshData()
{
	bReady4Rending = false;
	vao = -1;
	ebo = -1;
	vBuffer = -1;
	cBuffer = -1;
	uvBuffer = -1;
	nBuffer = -1;
	tBuffer = -1;
	bBuffer = -1;
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
	delete indexArray;
	delete vertexArray;
	delete colorArray;
	delete normalArray;
	delete tangentArray;
	delete binormalArray;
	delete uvArray;

	vao = -1;
	ebo = -1;
	vBuffer = -1;
	cBuffer = -1;
	uvBuffer = -1;
	nBuffer = -1;
	tBuffer = -1;
	bBuffer = -1;
	vertexCount = 0;
	polygonCount = 0;
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
	return vao;
}

GLuint MeshData::GetElementBufferObject()
{
	return ebo;
}

unsigned int MeshData::GetVertexCount()
{
	return vertexCount;
}

unsigned int MeshData::GetPolygonCount()
{
	return polygonCount;
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

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &vBuffer);
	glGenBuffers(1, &cBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &nBuffer);
	glGenBuffers(1, &tBuffer);
	glGenBuffers(1, &bBuffer);

	glBindVertexArray(vao);

	GLHelper::SetGLArrayBuffer(vBuffer, sizeof(float)*vertexCount * 3, vertexArray, 3, GL_FLOAT, CR_VERTATTRIPOS_POS);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_POS);

	GLHelper::SetGLArrayBuffer(cBuffer, sizeof(float)*vertexCount * 4, colorArray, 4, GL_FLOAT, CR_VERTATTRIPOS_COL);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_COL);

	GLHelper::SetGLArrayBuffer(uvBuffer, sizeof(float)*vertexCount * 4, uvArray, 4, GL_FLOAT, CR_VERTATTRIPOS_UVS);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_UVS);

	GLHelper::SetGLArrayBuffer(nBuffer, sizeof(float)*vertexCount * 3, normalArray, 3, GL_FLOAT, CR_VERTATTRIPOS_NOR);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_NOR);

	GLHelper::SetGLArrayBuffer(tBuffer, sizeof(float)*vertexCount * 3, tangentArray, 3, GL_FLOAT, CR_VERTATTRIPOS_TAG);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_TAG);

	GLHelper::SetGLArrayBuffer(bBuffer, sizeof(float)*vertexCount * 3, binormalArray, 3, GL_FLOAT, CR_VERTATTRIPOS_BNL);
	glEnableVertexAttribArray(CR_VERTATTRIPOS_BNL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*polygonCount * 3, indexArray, GL_STATIC_DRAW);

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	bReady4Rending = true;
}

MeshManager* MeshManager::ins = nullptr;

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
	meshDatas.clear();
}

MeshManager * MeshManager::Instance()
{
	if (ins == nullptr)
		ins = new MeshManager();

	return ins;
}

void MeshManager::AddMeshData(MeshData* data)
{
	char* index = FileReader::BindString((char*)data->GetPath().c_str(), ":");
	index = FileReader::BindString(index, (char*)data->GetSubMeshName().c_str());

	if (meshDatas.find(index) == meshDatas.end())
	{
		data->GetReady4Rending();
		meshDatas[index] = data;
	}
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
