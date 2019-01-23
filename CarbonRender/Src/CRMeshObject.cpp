#include "..\Inc\CRMeshObject.h"

MeshObject::MeshObject()
{
	Object();

	objType = ObjectType::eMesh;
	indexArray = nullptr;
	vertexArray = nullptr;
	colorArray = nullptr;
	normalArray = nullptr;
	tangentArray = nullptr;
	binormalArray = nullptr;
	uvArray = nullptr;
	material = nullptr;
}

MeshObject::~MeshObject()
{
}

void MeshObject::Delete()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vBuffer);
	glDeleteBuffers(1, &cBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &nBuffer);
	glDeleteBuffers(1, &tBuffer);

	delete this;
}

void MeshObject::CreateArraies()
{
	CreateIndexArray(polygonCount * 3);
	CreateVertexArray(vertexCount * 3);
	CreateVertexColorArray(vertexCount * 4);
	CreateNormalArray(vertexCount * 3);
	CreateTangentArray(vertexCount * 3);
	CreateBinormalArray(vertexCount * 3);
	CreateUVArray(vertexCount * 4);
}

void MeshObject::CreateIndexArray(unsigned int size)
{
	indexArray = new unsigned int[size];
}

void MeshObject::CreateVertexArray(unsigned int size)
{
	vertexArray = new float[size];
}

void MeshObject::CreateVertexColorArray(unsigned int size)
{
	colorArray = new float[size];
}

void MeshObject::CreateNormalArray(unsigned int size)
{
	normalArray = new float[size];
}

void MeshObject::CreateTangentArray(unsigned int size)
{
	tangentArray = new float[size];
}

void MeshObject::CreateBinormalArray(unsigned int size)
{
	binormalArray = new float[size];
}

void MeshObject::CreateUVArray(unsigned int size)
{
	uvArray = new float[size];
}

void MeshObject::SetVertexCount(unsigned int count)
{
	vertexCount = Math::Max(count, 0);
}

void MeshObject::SetPolygonCount(unsigned int count)
{
	polygonCount = Math::Max(count, 0);
}

void MeshObject::SetTexture(unsigned int i, GLuint tex)
{
	if (i >= 0 && i < 3)
		texs[i] = tex;
}

void MeshObject::SetIndexAt(unsigned int i, unsigned index)
{
	indexArray[i] = index;
}

void MeshObject::SetPath(string pathStr, string subMeshStr)
{
	path = pathStr;
	subMeshName = subMeshStr;
}

void MeshObject::CopyToVertexArray(float * data)
{
	memcpy(vertexArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshObject::CopyToVertexColorArray(float * data)
{
	memcpy(colorArray, data, sizeof(float) * GetVertexCount() * 4);
}

void MeshObject::CopyToNormalrray(float * data)
{
	memcpy(normalArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshObject::CopyToTangentArray(float * data)
{
	memcpy(tangentArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshObject::CopyToBinormalArray(float * data)
{
	memcpy(binormalArray, data, sizeof(float) * GetVertexCount() * 3);
}

void MeshObject::CopyToUVArray(float * data)
{
	memcpy(uvArray, data, sizeof(float) * GetVertexCount() * 4);
}

unsigned int MeshObject::GetVertexCount()
{
	return vertexCount;
}

unsigned int MeshObject::GetPolygonCount()
{
	return polygonCount;
}

GLuint MeshObject::GetTexture(unsigned int i)
{
	if (i >= 0 && i < 3)
		return texs[i];
	else
		return 0;
}

string MeshObject::GetPath()
{
	return path;
}

string MeshObject::GetSubMeshName()
{
	return subMeshName;
}

void MeshObject::GetReady4Rending()
{
	if (bReady4Render)
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

	bReady4Render = true;
}

void MeshObject::Render(GLuint shaderProgram, bool useTex)
{
	UpdateModelMatrix();

	Matrix4x4 finalMat;
	if (parent != nullptr)
	{
		parent->UpdateModelMatrix();
		finalMat = modelMatrix * parent->GetModelMatrix();
	}
	else
		finalMat = modelMatrix;
	Matrix3x3 normalMat = finalMat;

	if (useTex)
	{
		for (int i = 0; i < 3; i++)
		{
			glActiveTexture(GL_TEXTURE1 + i);
			glBindTexture(GL_TEXTURE_2D, texs[i]);
		}
	}

	GLint location = glGetUniformLocation(shaderProgram, "modelMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, finalMat.matrix);
	location = glGetUniformLocation(shaderProgram, "normalMat");
	glUniformMatrix3fv(location, 1, GL_FALSE, normalMat.matrix);
	location = glGetUniformLocation(shaderProgram, "viewMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix().matrix);
	location = glGetUniformLocation(shaderProgram, "proMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix().matrix);
	location = glGetUniformLocation(shaderProgram, "albedoMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "normalMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "msMap");
	glUniform1i(location, 3);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, polygonCount * 3, GL_UNSIGNED_INT, NULL);

	if (useTex)
	{
		for (int i = 0; i < 3; i++)
		{
			glActiveTexture(GL_TEXTURE1 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}
