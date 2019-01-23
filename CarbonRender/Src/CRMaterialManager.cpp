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

Material * MaterialManager::CreateMaterial()
{
	Material* newMat = new Material();
	
	return newMat;
}

Material::Material()
{
	roughness = 0.5f;
	metallic = 0.5f;
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

string Material::GetDiffuse()
{
	return texDirs[0];
}

string Material::GetNormal()
{
	return texDirs[1];
}

string Material::GetSpecular()
{
	return texDirs[2];
}

void Material::SetMetallic(float m)
{
	metallic = Math::Clamp(m, 0.0f, 1.0f);
}

void Material::SetDiffuse(string dir)
{
	texDirs[0] = dir;
}

void Material::SetNormal(string dir)
{
	texDirs[1] = dir;
}

void Material::SetSpecular(string dir)
{
	texDirs[2] = dir;
}

float Material::GetMetallic()
{
	return metallic;
}
