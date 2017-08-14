#include "..\Inc\CRMeshObject.h"

MeshObject::MeshObject()
{
}

MeshObject::~MeshObject()
{
	glDeleteVertexArrays(childCount, vaos);
	glDeleteBuffers(childCount, ebos);
	glDeleteBuffers(childCount, buffers);
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

	glGenVertexArrays(childCount, vaos);
	glGenBuffers(childCount, ebos);
	glGenBuffers(childCount, vbs);

	for (int i = 0; i < childCount; i++)
	{
		Mesh curMesh = child[i];
		glBindVertexArray(vaos[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*curMesh.vertexCount * 3, curMesh.vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(CR_VERTARRTIPOS_POS);

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
		Matrix4x4 finalMat = modelMatrix * curMesh.modelMatrix;

		GLint location = glGetUniformLocation(shaderProgram, "modelMat");
		glUniformMatrix4fv(location, 1, GL_FALSE, finalMat.matrix);
		location = glGetUniformLocation(shaderProgram, "viewMat");
		glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix().matrix);
		location = glGetUniformLocation(shaderProgram, "proMat");
		glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix().matrix);

		ShaderManager::Instance()->UseShader(shaderProgram);
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);
		glPointSize(5.0f);
		glDrawElements(GL_POINTS, curMesh.vertexCount, GL_UNSIGNED_INT, NULL);
	}

	return;
}

void MeshObject::AttachShader(GLuint shader)
{
	shaderProgram = shader;

	return;
}
