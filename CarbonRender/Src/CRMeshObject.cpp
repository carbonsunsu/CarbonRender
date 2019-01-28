#include "..\Inc\CRMeshObject.h"

MeshObject::MeshObject()
{
	Object();

	objType = ObjectType::eMesh;
	material = nullptr;
	meshData = nullptr;
}

MeshObject::~MeshObject()
{
}

void MeshObject::SetMaterial(Material * mat)
{
	material = mat;
}

void MeshObject::SetMeshData(MeshData * data)
{
	meshData = data;
}

Material * MeshObject::GetMaterial()
{
	return material;
}

MeshData * MeshObject::GetMeshData()
{
	return meshData;
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

	if (material != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->GetDiffuse());

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material->GetNormal());

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, material->GetSpecular());
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

	glBindVertexArray(meshData->GetVertexArrayObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GetElementBufferObject());
	glDrawElements(GL_TRIANGLES, meshData->GetPolygonCount() * 3, GL_UNSIGNED_INT, NULL);

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
