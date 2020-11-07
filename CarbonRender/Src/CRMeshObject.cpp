#include "..\Inc\CRMeshObject.h"

MeshObject::MeshObject()
{
	Object();

	objType = ObjectType::eMesh;
	materialID = 0;
	meshData = nullptr;
}

MeshObject::~MeshObject()
{
	MaterialManager::Instance()->DeleteMaterial(materialID);
}

void MeshObject::SetMaterial(unsigned int id)
{
	materialID = id;
}

void MeshObject::SetMeshData(MeshData * data)
{
	meshData = data;
}

Material * MeshObject::GetMaterial()
{
	return MaterialManager::Instance()->GetMaterial(materialID);
}

MeshData * MeshObject::GetMeshData()
{
	return meshData;
}

void MeshObject::Render(GLuint shaderProgram, bool useTex)
{
	if (meshData == nullptr || !visible)
		return;

	UpdateModelMatrix();

	Matrix4x4 finalMat = modelMatrix;
	Object* curP = parent;
	while (curP != nullptr)
	{
		curP->UpdateModelMatrix();
		finalMat = finalMat * curP->GetModelMatrix();
		curP = curP->GetParent();
	}
	Matrix3x3 normalMat = finalMat;

	Material* material = GetMaterial();

	if (useTex)
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
	location = glGetUniformLocation(shaderProgram, "rmMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "roughnessScale");
	glUniform1f(location, material->HasSpecularTexture() ? 1.0f : material->GetRoughness());
	location = glGetUniformLocation(shaderProgram, "metallicScale");
	glUniform1f(location, material->HasSpecularTexture() ? 1.0f : material->GetMetallic());
	float4 colorScaler = material->GetColor();
	location = glGetUniformLocation(shaderProgram, "albedoScaler");
	glUniform4f(location, colorScaler.x, colorScaler.y, colorScaler.z, colorScaler.w);
	float3 diffTilling = material->GetDiffuseTilling();
	float3 norlTilling = material->GetNormalTilling();
	float3 specTilling = material->GetSpecularTilling();
	location = glGetUniformLocation(shaderProgram, "uvTilling0");
	glUniform2f(location, diffTilling.x, diffTilling.y);
	location = glGetUniformLocation(shaderProgram, "uvTilling1");
	glUniform4f(location, norlTilling.x, norlTilling.y, specTilling.x, specTilling.y);
	float3 emisColor = material->GetEmissionColor() * material->GetEmissionIntensity();
	location = glGetUniformLocation(shaderProgram, "emissionColor");
	glUniform3f(location, emisColor.x, emisColor.y, emisColor.z);

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
