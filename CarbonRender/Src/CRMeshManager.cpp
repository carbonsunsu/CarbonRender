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
	InitBuildinSphereMesh();
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
	int slices = 32;
	int stacks = 8;

	int vertexCount = 2 * slices * stacks + 2 + stacks * 2;
	int polygonCount = slices * stacks * 4 - 2 * slices;

	unsigned int* index = new unsigned int[polygonCount * 3];
	float* vertexPos = new float[vertexCount * 3];
	float* vertexUV = new float[vertexCount * 4];
	float* vertexColor = new float[vertexCount * 4];
	float* vertexNormal = new float[vertexCount * 3];
	float* vertexTangent = new float[vertexCount * 3];
	float* vertexBinormal = new float[vertexCount * 3];

	//top and bottom vertex
	vertexPos[0] = 0.0f; vertexPos[1] = radius; vertexPos[2] = 0.0f;
	vertexUV[0] = 0.0f; vertexUV[1] = 1.0f; vertexUV[2] = 0.0f; vertexUV[3] = 0.0f;
	vertexColor[0] = 1.0f; vertexColor[1] = 1.0f; vertexColor[2] = 1.0f; vertexColor[3] = 1.0f;
	vertexNormal[0] = 0.0f; vertexNormal[1] = 1.0f; vertexNormal[2] = 0.0f;
	vertexTangent[0] = 1.0f; vertexTangent[1] = 0.0f; vertexTangent[2] = 0.0f;
	vertexBinormal[0] = 0.0f; vertexBinormal[1] = 0.0f; vertexBinormal[2] = -1.0f;

	vertexPos[3] = 0.0f; vertexPos[4] = -radius; vertexPos[5] = 0.0f;
	vertexUV[4] = 0.0f; vertexUV[5] = 0.0f; vertexUV[6] = 0.0f; vertexUV[7] = 0.0f;
	vertexColor[4] = 1.0f; vertexColor[5] = 1.0f; vertexColor[6] = 1.0f; vertexColor[7] = 1.0f;
	vertexNormal[3] = 0.0f; vertexNormal[4] = -1.0f; vertexNormal[5] = 0.0f;
	vertexTangent[3] = 1.0f; vertexTangent[4] = 0.0f; vertexTangent[5] = 0.0f;
	vertexBinormal[3] = 0.0f; vertexBinormal[4] = 0.0f; vertexBinormal[5] = -1.0f;

	float anglePerSlice = 2.0f * PI / slices;
	float anglePerStacks = PI * 0.5f / stacks;
	int vertexIndex = 2;
	int polyIndex = 0;

	//top sphere
	for (int i = 0; i <= slices; i++)
	{
		float hAngle = anglePerSlice * i;

		for (int j = 0; j < stacks; j++)
		{
			float vAngle = anglePerStacks * j;
			
			vertexPos[vertexIndex * 3] = sinf(hAngle) * cosf(vAngle) * radius;
			vertexPos[vertexIndex * 3 + 1] = sinf(vAngle) * radius;
			vertexPos[vertexIndex * 3 + 2] = -cosf(hAngle) * cosf(vAngle) * radius;

			vertexUV[vertexIndex * 4] = (float)i / slices;
			vertexUV[vertexIndex * 4 + 1] = 0.5f + j * 0.5f / stacks;
			vertexUV[vertexIndex * 4 + 2] = 0.0f; vertexUV[vertexIndex * 4 + 3] = 0.0f;

			vertexColor[vertexIndex * 4] = 1.0f; vertexColor[vertexIndex * 4 + 1] = 1.0f; vertexColor[vertexIndex * 4 + 2] = 1.0f; vertexColor[vertexIndex * 4 + 3] = 1.0f;

			vertexNormal[vertexIndex * 3] = vertexPos[vertexIndex * 3] * 2.0f;
			vertexNormal[vertexIndex * 3 + 1] = vertexPos[vertexIndex * 3 + 1] * 2.0f;
			vertexNormal[vertexIndex * 3 + 2] = vertexPos[vertexIndex * 3 + 2] * 2.0f;

			if (i != 0)
			{
				int neighboutIndex = vertexIndex - stacks;
				float3 neighbourVertex = float3(vertexPos[neighboutIndex * 3], vertexPos[neighboutIndex * 3 + 1], vertexPos[neighboutIndex * 3 + 2]);
				float3 neighbourT = float3(vertexPos[vertexIndex * 3] - neighbourVertex.x,
											vertexPos[vertexIndex * 3 + 1] - neighbourVertex.y,
											vertexPos[vertexIndex * 3 + 2] - neighbourVertex.z).normalize();
				float3 neighbourN = float3(vertexNormal[neighboutIndex * 3], vertexNormal[neighboutIndex * 3 + 1], vertexNormal[neighboutIndex * 3 + 2]);
				float3 neighboutB = Math::Cross(neighbourN, neighbourT).normalize();

				vertexTangent[neighboutIndex * 3] = neighbourT.x;
				vertexTangent[neighboutIndex * 3 + 1] = neighbourT.y;
				vertexTangent[neighboutIndex * 3 + 2] = neighbourT.z;

				vertexBinormal[neighboutIndex * 3] = neighboutB.x;
				vertexBinormal[neighboutIndex * 3 + 1] = neighboutB.y;
				vertexBinormal[neighboutIndex * 3 + 2] = neighboutB.z;
				/**/
				if (j == stacks - 1)
				{
					index[polyIndex * 3] = vertexIndex;
					index[polyIndex * 3 + 1] = neighboutIndex;
					index[polyIndex * 3 + 2] = 0;
				}
				else
				{
					index[polyIndex * 3] = vertexIndex;
					index[polyIndex * 3 + 1] = neighboutIndex;
					index[polyIndex * 3 + 2] = neighboutIndex + 1;
				}
				polyIndex++;

				if (j != 0)
				{
					index[polyIndex * 3] = vertexIndex;
					index[polyIndex * 3 + 1] = vertexIndex - 1;
					index[polyIndex * 3 + 2] = neighboutIndex;
					polyIndex++;
				}
				
				if (0)
				{
					int neighboutIndex = 2 + j;
					float3 neighbourVertex = float3(vertexPos[neighboutIndex * 3], vertexPos[neighboutIndex * 3 + 1], vertexPos[neighboutIndex * 3 + 2]);
					float3 T = float3(-vertexPos[vertexIndex * 3] + neighbourVertex.x,
										-vertexPos[vertexIndex * 3 + 1] + neighbourVertex.y,
										-vertexPos[vertexIndex * 3 + 2] + neighbourVertex.z).normalize();
					float3 N = float3(vertexNormal[vertexIndex * 3], vertexNormal[vertexIndex * 3 + 1], vertexNormal[vertexIndex * 3 + 2]);
					float3 B = Math::Cross(N, T).normalize();

					vertexTangent[vertexIndex * 3] = T.x;
					vertexTangent[vertexIndex * 3 + 1] = T.y;
					vertexTangent[vertexIndex * 3 + 2] = T.z;

					vertexBinormal[vertexIndex * 3] = B.x;
					vertexBinormal[vertexIndex * 3 + 1] = B.y;
					vertexBinormal[vertexIndex * 3 + 2] = B.z;

					if (j == stacks - 1)
					{
						index[polyIndex * 3] = vertexIndex;
						index[polyIndex * 3 + 1] = 0;
						index[polyIndex * 3 + 2] = neighboutIndex;
					}
					else
					{
						index[polyIndex * 3] = vertexIndex;
						index[polyIndex * 3 + 1] = neighboutIndex + 1;
						index[polyIndex * 3 + 2] = neighboutIndex;
					}
					polyIndex++;

					if (j != 0)
					{
						index[polyIndex * 3] = vertexIndex;
						index[polyIndex * 3 + 1] = neighboutIndex;
						index[polyIndex * 3 + 2] = vertexIndex - 1;
						polyIndex++;
					}
				}
				
			}
			vertexIndex++;
		}
	}

	//bottom sphere
	int bottomVertexStartIndex = vertexIndex;
	int topVertexIndex = 2;
	for (int i = 0; i <= slices; i++)
	{
		for (int j = 0; j < stacks; j++)
		{
			vertexPos[vertexIndex * 3] = vertexPos[topVertexIndex * 3];
			vertexPos[vertexIndex * 3 + 1] = -vertexPos[topVertexIndex * 3 + 1];
			vertexPos[vertexIndex * 3 + 2] = vertexPos[topVertexIndex * 3 + 2];

			vertexUV[vertexIndex * 4] = vertexUV[topVertexIndex * 4];
			vertexUV[vertexIndex * 4 + 1] = 1.0f - vertexUV[topVertexIndex * 4 + 1];
			vertexUV[vertexIndex * 4 + 2] = 0.0f; vertexUV[vertexIndex * 4 + 3] = 0.0f;

			vertexColor[vertexIndex * 4] = 1.0f; vertexColor[vertexIndex * 4 + 1] = 1.0f; vertexColor[vertexIndex * 4 + 2] = 1.0f; vertexColor[vertexIndex * 4 + 3] = 1.0f;

			vertexNormal[vertexIndex * 3] = vertexNormal[topVertexIndex * 3];
			vertexNormal[vertexIndex * 3 + 1] = -vertexNormal[topVertexIndex * 3 + 1];
			vertexNormal[vertexIndex * 3 + 2] = vertexNormal[topVertexIndex * 3 + 2];

			vertexTangent[vertexIndex * 3] = vertexTangent[topVertexIndex * 3];
			vertexTangent[vertexIndex * 3 + 1] = -vertexTangent[topVertexIndex * 3 + 1];
			vertexTangent[vertexIndex * 3 + 2] = vertexTangent[topVertexIndex * 3 + 2];

			vertexBinormal[vertexIndex * 3] = vertexBinormal[topVertexIndex * 3];
			vertexBinormal[vertexIndex * 3 + 1] = -vertexBinormal[topVertexIndex * 3 + 1];
			vertexBinormal[vertexIndex * 3 + 2] = vertexBinormal[topVertexIndex * 3 + 2];

			if (i != 0)
			{
				int neighboutIndex = vertexIndex - stacks;

				if (j == stacks - 1)
				{
					index[polyIndex * 3] = vertexIndex;
					index[polyIndex * 3 + 1] = 1;
					index[polyIndex * 3 + 2] = neighboutIndex;
				}
				else
				{
					index[polyIndex * 3] = vertexIndex;
					index[polyIndex * 3 + 1] = neighboutIndex + 1;
					index[polyIndex * 3 + 2] = neighboutIndex;
				}
				polyIndex++;

				if (j != 0)
				{
					index[polyIndex * 3] = vertexIndex;
					index[polyIndex * 3 + 1] = neighboutIndex;
					index[polyIndex * 3 + 2] = vertexIndex - 1;
					polyIndex++;
				}			
				
				if (0)
				{
					int neighboutIndex = bottomVertexStartIndex + j;

					if (j == stacks - 1)
					{
						index[polyIndex * 3] = vertexIndex;
						index[polyIndex * 3 + 1] = neighboutIndex;
						index[polyIndex * 3 + 2] = 1;
					}
					else
					{
						index[polyIndex * 3] = vertexIndex;
						index[polyIndex * 3 + 1] = neighboutIndex;
						index[polyIndex * 3 + 2] = neighboutIndex + 1;
					}
					polyIndex++;

					if (j != 0)
					{
						index[polyIndex * 3] = vertexIndex;
						index[polyIndex * 3 + 1] = vertexIndex - 1;
						index[polyIndex * 3 + 2] = neighboutIndex;
						polyIndex++;
					}
				}
			}
			vertexIndex++;
			topVertexIndex++;
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

	buildinSphere.CopyToIndexArray(index);
	buildinSphere.CopyToVertexArray(vertexPos);
	buildinSphere.CopyToVertexColorArray(vertexColor);
	buildinSphere.CopyToUVArray(vertexUV);
	buildinSphere.CopyToNormalrray(vertexNormal);
	buildinSphere.CopyToTangentArray(vertexTangent);
	buildinSphere.CopyToBinormalArray(vertexBinormal);

	buildinSphere.GetReady4Rending();
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
