#include "..\Inc\CRMaterialManager.h"

MaterialManager* MaterialManager::ins = nullptr;

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

MaterialManager * MaterialManager::Instance()
{
	if (ins == nullptr)
		ins = new MaterialManager();

	return ins;
}

Material * MaterialManager::CreateNewMaterial()
{
	Material* newMat = new Material();
	materials.emplace_back(*newMat);
	
	return newMat;
}

Material::Material()
{
	roughness = 1.0f;
	metallic = 1.0f;
	texIns[0] = 0;
	texIns[1] = 0;
	texIns[2] = 0;
}

Material::~Material()
{
}

void Material::SetRoughness(float r)
{
	roughness = Math::Clamp(r, 0.0f, 1.0f);
}

float Material::GetRoughness()
{
	return roughness;
}

void Material::SetMetallic(float m)
{
	metallic = Math::Clamp(m, 0.0f, 1.0f);
}

float Material::GetMetallic()
{
	return metallic;
}

string Material::GetDiffusePath()
{
	return texDirs[0];
}

GLuint Material::GetDiffuse()
{
	return texIns[0];
}

string Material::GetNormalPath()
{
	return texDirs[1];
}

GLuint Material::GetNormal()
{
	return texIns[1];
}

string Material::GetSpecularPath()
{
	return texDirs[2];
}

GLuint Material::GetSpecular()
{
	return texIns[2];
}

void Material::SetDiffuse(string dir)
{
	texDirs[0] = dir;
	if (dir.empty())
		texIns[0] = TextureManager::Instance()->LoadDefaultD();
	else
		texIns[0] = TextureManager::Instance()->LoadTexture(dir);
}

void Material::SetNormal(string dir)
{
	texDirs[1] = dir;
	if (dir.empty())
		texIns[1] = TextureManager::Instance()->LoadDefaultN();
	else
		texIns[1] = TextureManager::Instance()->LoadTexture(dir);
}

void Material::SetSpecular(string dir)
{
	texDirs[2] = dir;
	if (dir.empty())
		texIns[2] = TextureManager::Instance()->LoadDefaultS();
	else
		texIns[2] = TextureManager::Instance()->LoadTexture(dir);
}