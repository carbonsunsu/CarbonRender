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

Material::Material()
{
	roughness = 1.0f;
	metallic = 1.0f;
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
