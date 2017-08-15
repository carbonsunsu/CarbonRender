#include "..\Inc\CRMeshObject.h"

MeshObject::MeshObject()
{
}

MeshObject::~MeshObject()
{
	glDeleteVertexArrays(childCount, vaos);
	glDeleteBuffers(childCount, ebos);
	glDeleteBuffers(childCount, vbs);
	glDeleteBuffers(childCount, cbs);
	glDeleteBuffers(childCount, ubs);
	glDeleteBuffers(childCount, nbs);
	glDeleteBuffers(childCount, tbs);
}

void MeshObject::SetChildCount(uint16_t count)
{
	childCount = count;
	child = new Mesh[childCount];
}

void MeshObject::SetChild(Mesh newChild, unsigned int index)
{
	child[index] = newChild;
}

uint16_t MeshObject::GetChildCount()
{
	return childCount;
}

Mesh * MeshObject::GetChild(unsigned int index)
{
	return &child[index];
}

Mesh * MeshObject::GetAllChild()
{
	return child;
}

void MeshObject::GetReady4Rending()
{
	if (bReady4Render)
		return;

	vaos = new GLuint[childCount];
	ebos = new GLuint[childCount];
	vbs = new GLuint[childCount];
	cbs = new GLuint[childCount];
	ubs = new GLuint[childCount];
	nbs = new GLuint[childCount];
	tbs = new GLuint[childCount];

	glGenVertexArrays(childCount, vaos);
	glGenBuffers(childCount, ebos);
	glGenBuffers(childCount, vbs);
	glGenBuffers(childCount, cbs);
	glGenBuffers(childCount, ubs);
	glGenBuffers(childCount, nbs);
	glGenBuffers(childCount, tbs);

	for (int i = 0; i < childCount; i++)
	{
		Mesh curMesh = child[i];
		glBindVertexArray(vaos[i]);

		SetGLArrayBuffer(vbs[i], sizeof(float)*curMesh.vertexCount * 3, curMesh.vertex, 3, GL_FLOAT, CR_VERTATTRIPOS_POS);
		glEnableVertexAttribArray(CR_VERTATTRIPOS_POS);

		SetGLArrayBuffer(cbs[i], sizeof(float)*curMesh.vertexCount * 4, curMesh.color, 4, GL_FLOAT, CR_VERTATTRIPOS_COL);
		glEnableVertexAttribArray(CR_VERTATTRIPOS_COL);

		SetGLArrayBuffer(ubs[i], sizeof(float)*curMesh.vertexCount * 4, curMesh.uv, 4, GL_FLOAT, CR_VERTATTRIPOS_UVS);
		glEnableVertexAttribArray(CR_VERTATTRIPOS_UVS);

		SetGLArrayBuffer(nbs[i], sizeof(float)*curMesh.vertexCount * 3, curMesh.normal, 3, GL_FLOAT, CR_VERTATTRIPOS_NOR);
		glEnableVertexAttribArray(CR_VERTATTRIPOS_NOR);

		SetGLArrayBuffer(tbs[i], sizeof(float)*curMesh.vertexCount * 3, curMesh.tangent, 3, GL_FLOAT, CR_VERTATTRIPOS_TAG);
		glEnableVertexAttribArray(CR_VERTATTRIPOS_TAG);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*curMesh.polygonCount * 3, curMesh.index, GL_STATIC_DRAW);

		glBindVertexArray(NULL);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}

	bReady4Render = true;
}

void MeshObject::Render()
{
	UpdateModelMatrix();
	for (int i = 0; i < childCount; i++)
	{
		Mesh curMesh = child[i];
		Matrix4x4 finalMat = curMesh.modelMatrix * modelMatrix;// *curMesh.modelMatrix;
		Matrix3x3 normalMat = finalMat;

		GLint location = glGetUniformLocation(shaderProgram, "modelMat");
		glUniformMatrix4fv(location, 1, GL_FALSE, finalMat.matrix);
		location = glGetUniformLocation(shaderProgram, "normalMat");
		glUniformMatrix3fv(location, 1, GL_FALSE, normalMat.matrix);
		location = glGetUniformLocation(shaderProgram, "viewMat");
		glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix().matrix);
		location = glGetUniformLocation(shaderProgram, "proMat");
		glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix().matrix);

		ShaderManager::Instance()->UseShader(shaderProgram);
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);
		glDrawElements(GL_TRIANGLES, curMesh.polygonCount * 3, GL_UNSIGNED_INT, NULL);
	}

	return;
}

void MeshObject::AttachShader(GLuint shader)
{
	shaderProgram = shader;

	return;
}
