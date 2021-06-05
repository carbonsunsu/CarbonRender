#include "..\Inc\CRTerrainManager.h"

TerrainManager* TerrainManager::ins = nullptr;

TerrainManager * TerrainManager::Instance()
{
	if (ins == nullptr)
		ins = new TerrainManager();

	return ins;
}

void TerrainManager::CreateTerrainObject()
{
	if (terrainObj == nullptr)
	{
		terrainObj = new TerrainObject();
		useTerrain = true;
	}
}

void TerrainManager::DeleteTerrainObject()
{
	delete terrainObj;
	useTerrain = false;
}

TerrainObject * TerrainManager::GetTerrainObject()
{
	return terrainObj;
}

bool TerrainManager::UseTerrain()
{
	return useTerrain;
}

void TerrainManager::CreateOceanObject()
{
	if (oceanObj == nullptr)
	{
		oceanObj = new OceanObject();
		useOcean = true;
	}
}

void TerrainManager::DeleteOceanObject()
{
	delete oceanObj;
	useOcean = false;
}

OceanObject * TerrainManager::GetOceanObject()
{
	return oceanObj;
}

bool TerrainManager::UseOcean()
{
	return useOcean;
}

TerrainManager::TerrainManager()
{
	CreateTerrainObject();
	//CreateOceanObject();
}

TerrainManager::~TerrainManager()
{
	if (useTerrain)
	{
		delete terrainObj;
		terrainObj = nullptr;
		useTerrain = false;
	}

	if (useOcean)
	{
		delete oceanObj;
		oceanObj = nullptr;
		useOcean = false;
	}

	ins = nullptr;
}

void TerrainObject::Render(GLuint shaderProgram)
{
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

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultD());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultN());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultS());

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
	glUniform1f(location, 0.5f);
	location = glGetUniformLocation(shaderProgram, "metallicScale");
	glUniform1f(location, 0.0f);
	location = glGetUniformLocation(shaderProgram, "albedoScaler");
	glUniform4f(location, 0.5f, 0.5f, 0.5f, 1.0f);

	glBindVertexArray(vertexArrayBuffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, NULL);

	for (int i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void TerrainObject::Update(Camera * cam)
{
	float3 camPos = cam->GetPosition();
	camPos.y = 0.0f;

	float farClip = cam->GetFarClip();
	float3 intersections[4];
	intersections[0] = float3(-farClip, 0.0f, -farClip) + camPos;
	intersections[1] = float3(-farClip, 0.0f, farClip) + camPos;
	intersections[2] = float3(farClip, 0.0f, -farClip) + camPos;
	intersections[3] = float3(farClip, 0.0f, farClip) + camPos;
	
	//update vertex pos
	float* ptrPos = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[2], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrPos[i * 3] = intersections[i].x;
		ptrPos[i * 3 + 1] = intersections[i].y;
		ptrPos[i * 3 + 2] = intersections[i].z;
	}

	//update vertex color
	float* ptrColor = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[3], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrColor[i * 4] = 1.0f;
		ptrColor[i * 4 + 1] = 1.0f;
		ptrColor[i * 4 + 2] = 1.0f;
		ptrColor[i * 4 + 3] = 1.0f;
	}

	//update vertex uv
	float* ptrUV = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[4], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrUV[i * 4] = ptrPos[i] * 0.001f;
		ptrUV[i * 4 + 1] = ptrPos[i * 3 + 2] * 0.001f;
		ptrUV[i * 4 + 2] = 1.0f;
		ptrUV[i * 4 + 3] = 1.0f;
	}

	//update vertex normal
	float* ptrNormal = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[5], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrNormal[i * 3] = 0.0f;
		ptrNormal[i * 3 + 1] = 1.0f;
		ptrNormal[i * 3 + 2] = 0.0f;
	}

	//update vertex tangent
	float* ptrTangent = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[6], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrTangent[i * 3] = 1.0f;
		ptrTangent[i * 3 + 1] = 0.0f;
		ptrTangent[i * 3 + 2] = 0.0f;
	}

	//update vertex binormal
	float* ptrBinormal = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[7], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrBinormal[i * 3] = 0.0f;
		ptrBinormal[i * 3 + 1] = 0.0f;
		ptrBinormal[i * 3 + 2] = -1.0f;
	}

	for (int i = 2; i < 8; i++)
		GLHelper::UnmapGLArrayBuffer(vertexArrayBuffers[i]);
}

TerrainObject::TerrainObject()
{
	glGenVertexArrays(1, vertexArrayBuffers);
	glGenBuffers(7, vertexArrayBuffers + 1);

	glBindVertexArray(vertexArrayBuffers[0]);

	unsigned int indexes[6] = {
		0, 1, 2,
		1, 3, 2};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * 3, indexes, GL_STATIC_DRAW);

	float* vertexBuffer = new float[4 * 3];
	float* colorBuffer = new float[4 * 4];
	float* uvBuffer = new float[4 * 4];
	float* normalBuffer = new float[4 * 3];
	float* tangentBuffer = new float[4 * 3];
	float* binormalBuffer = new float[4 * 3];

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[2], sizeof(float) * 4 * 3, vertexBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_POS);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[3], sizeof(float) * 4 * 4, colorBuffer, 4, GL_FLOAT, CR_VERTATTRIPOS_COL);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[4], sizeof(float) * 4 * 4, uvBuffer, 4, GL_FLOAT, CR_VERTATTRIPOS_UVS);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[5], sizeof(float) * 4 * 3, normalBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_NOR);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[6], sizeof(float) * 4 * 3, tangentBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_TAG);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[7], sizeof(float) * 4 * 3, binormalBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_BNL);

	delete[] vertexBuffer;
	delete[] colorBuffer;
	delete[] uvBuffer;
	delete[] normalBuffer;
	delete[] tangentBuffer;
	delete[] binormalBuffer;
}

TerrainObject::~TerrainObject()
{
	glDeleteVertexArrays(1, vertexArrayBuffers);
	glDeleteBuffers(7, vertexArrayBuffers + 1);

	for (int i = 0; i < 8; i++)
		vertexArrayBuffers[i] = -1;
}

void OceanObject::Render(GLuint shaderProgram)
{
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

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultD());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultN());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultS());

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
	glUniform1f(location, 0.5f);
	location = glGetUniformLocation(shaderProgram, "metallicScale");
	glUniform1f(location, 0.0f);
	location = glGetUniformLocation(shaderProgram, "albedoScaler");
	glUniform4f(location, 0.5f, 0.5f, 0.5f, 1.0f);

	glBindVertexArray(vertexArrayBuffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, NULL);

	for (int i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void OceanObject::Update(Camera * cam)
{
	float halfFov = cam->GetFov() * 0.5f;
	float nearClip = cam->GetNearClip();
	float farClip = cam->GetFarClip();
	float3 camF = cam->GetForward().normalize();
	float3 camR = cam->GetRight().normalize();
	float3 camU = Math::Cross(camR, camF).normalize();
	float3 camPos = cam->GetPosition();

	float tanAlpha = tan(Math::Angle2Radian(halfFov));
	float halfH = nearClip * tanAlpha;
	WindowSize wSize = WindowManager::Instance()->GetWindowSize();
	float halfW = wSize.w * halfH / wSize.h;
	float3 vLT = (-camR * halfW + camU * halfH + camF * nearClip).normalize();
	float3 vLB = (-camR * halfW - camU * halfH + camF * nearClip).normalize();
	float3 vRT = (camR * halfW + camU * halfH + camF * nearClip).normalize();
	float3 vRB = (camR * halfW - camU * halfH + camF * nearClip).normalize();

	float mappedMaxRange = farClip / Math::Dot(camF, vLT);

	float3 intersections[4];
	intersections[0] = GetIntersectionWithOcean(vLB, camPos, mappedMaxRange, cam);
	intersections[1] = GetIntersectionWithOcean(vRB, camPos, mappedMaxRange, cam);
	intersections[2] = GetIntersectionWithOcean(vLT, camPos, mappedMaxRange, cam);
	intersections[3] = GetIntersectionWithOcean(vRT, camPos, mappedMaxRange, cam);

	//update vertex pos
	float* ptrPos = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[2], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrPos[i * 3] = intersections[i].x;
		ptrPos[i * 3 + 1] = intersections[i].y;
		ptrPos[i * 3 + 2] = intersections[i].z;
	}

	//update vertex color
	float* ptrColor = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[3], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrColor[i * 4] = 1.0f;
		ptrColor[i * 4 + 1] = 1.0f;
		ptrColor[i * 4 + 2] = 1.0f;
		ptrColor[i * 4 + 3] = 1.0f;
	}

	//update vertex uv
	float* ptrUV = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[4], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrUV[i * 4] = ptrPos[i] * 0.001f;
		ptrUV[i * 4 + 1] = ptrPos[i * 3 + 2] * 0.001f;
		ptrUV[i * 4 + 2] = 1.0f;
		ptrUV[i * 4 + 3] = 1.0f;
	}

	//update vertex normal
	float* ptrNormal = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[5], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrNormal[i * 3] = 0.0f;
		ptrNormal[i * 3 + 1] = 1.0f;
		ptrNormal[i * 3 + 2] = 0.0f;
	}

	//update vertex tangent
	float* ptrTangent = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[6], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrTangent[i * 3] = 1.0f;
		ptrTangent[i * 3 + 1] = 0.0f;
		ptrTangent[i * 3 + 2] = 0.0f;
	}

	//update vertex binormal
	float* ptrBinormal = (float*)GLHelper::GetGLArrayBufferPointer(vertexArrayBuffers[7], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrBinormal[i * 3] = 0.0f;
		ptrBinormal[i * 3 + 1] = 0.0f;
		ptrBinormal[i * 3 + 2] = -1.0f;
	}

	for (int i = 2; i < 8; i++)
		GLHelper::UnmapGLArrayBuffer(vertexArrayBuffers[i]);
}

OceanObject::OceanObject()
{
	glGenVertexArrays(1, vertexArrayBuffers);
	glGenBuffers(7, vertexArrayBuffers + 1);

	glBindVertexArray(vertexArrayBuffers[0]);

	unsigned int indexes[6] = {
		0, 1, 2,
		1, 3, 2 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * 3, indexes, GL_STATIC_DRAW);

	float* vertexBuffer = new float[4 * 3];
	float* colorBuffer = new float[4 * 4];
	float* uvBuffer = new float[4 * 4];
	float* normalBuffer = new float[4 * 3];
	float* tangentBuffer = new float[4 * 3];
	float* binormalBuffer = new float[4 * 3];

	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[2], sizeof(float) * 4 * 3, vertexBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_POS);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[3], sizeof(float) * 4 * 4, colorBuffer, 4, GL_FLOAT, CR_VERTATTRIPOS_COL);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[4], sizeof(float) * 4 * 4, uvBuffer, 4, GL_FLOAT, CR_VERTATTRIPOS_UVS);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[5], sizeof(float) * 4 * 3, normalBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_NOR);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[6], sizeof(float) * 4 * 3, tangentBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_TAG);
	GLHelper::SetGLArrayBuffer(vertexArrayBuffers[7], sizeof(float) * 4 * 3, binormalBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_BNL);

	delete[] vertexBuffer;
	delete[] colorBuffer;
	delete[] uvBuffer;
	delete[] normalBuffer;
	delete[] tangentBuffer;
	delete[] binormalBuffer;
}

OceanObject::~OceanObject()
{
	glDeleteVertexArrays(1, vertexArrayBuffers);
	glDeleteBuffers(7, vertexArrayBuffers + 1);

	for (int i = 0; i < 8; i++)
		vertexArrayBuffers[i] = -1;
}

float3 OceanObject::GetIntersectionWithOcean(float3 v, float3 o, float maxRange, Camera * cam)
{
	float3 downV = float3(0.0f, -1.0f, 0.0f);
	float3 intersectioinPoint;

	float VoD = Math::Dot(v, downV);
	float l;

	if (VoD > 0)
	{
		l = Math::Abs(o.y - this->transform[1]) / VoD;
		l = Math::Min(l, maxRange);
		intersectioinPoint = o + v * l;
		intersectioinPoint.y = this->transform[1];
		return intersectioinPoint;
	}
	else
	{
		float3 camF = cam->GetForward().normalize();
		float3 camR = cam->GetRight().normalize();
		float3 camU = Math::Cross(camR, camF).normalize();
		return GetIntersectionWithOcean(-camU, o + v * maxRange, INT_MAX, cam);
	}
}