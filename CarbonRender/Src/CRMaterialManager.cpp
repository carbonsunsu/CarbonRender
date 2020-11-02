#include "..\Inc\CRMaterialManager.h"

MaterialManager* MaterialManager::ins = nullptr;

MaterialManager::MaterialManager()
{
	defaultMaterial.SetDiffuse("");
	defaultMaterial.SetNormal("");
	defaultMaterial.SetSpecular("");
}

MaterialManager::~MaterialManager()
{
	for (vector<Material*>::iterator i = materials.begin(); i != materials.end(); i++)
		delete *i;

	ins = nullptr;
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
	materials.emplace_back(newMat);
	
	return newMat;
}

Material * MaterialManager::GetDefaultMaterial()
{
	return &defaultMaterial;
}

void MaterialManager::DeleteMaterial(Material * mat)
{
	if (mat == &defaultMaterial)
		return;

	for (vector<Material*>::iterator i = materials.begin(); i != materials.end(); i++)
		if (mat == *i)
		{
			materials.erase(i);
			delete mat;
			return;
		}
}

void Material::SetTexture(string dir, int index)
{
	texDirs[index] = dir;
	if (dir.empty())
	{
		texIns[index] = TextureManager::Instance()->LoadDefaultTexs()[index];
		hasTex[index] = false;
	}
	else
	{
		texIns[index] = TextureManager::Instance()->LoadTexture((char*)dir.c_str());
		if (texIns[index] == 0)
		{
			texIns[index] = TextureManager::Instance()->LoadDefaultTexs()[index];
			hasTex[index] = false;
		}
		else
			hasTex[index] = true;
	}
}

Material::Material()
{
	roughness = 0.5f;
	metallic = 0.5f;
	texIns[0] = 0;
	texIns[1] = 0;
	texIns[2] = 0;
	hasTex[0] = false;
	hasTex[1] = false;
	hasTex[2] = false;
	color = float4(1.0f, 1.0f, 1.0f, 1.0f);
}

Material::~Material()
{
	RemoveDiffuse();
	RemoveNormal();
	RemoveSpecular();
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

float4 Material::GetColor()
{
	return color;
}

bool Material::HasDiffuseTexture()
{
	return hasTex[0];
}

bool Material::HasNormalTexture()
{
	return hasTex[1];
}

bool Material::HasSpecularTexture()
{
	return hasTex[2];
}

void Material::SetDiffuse(string dir)
{
	SetTexture(dir, 0);
}

void Material::SetNormal(string dir)
{
	SetTexture(dir, 1);
}

void Material::SetSpecular(string dir)
{
	SetTexture(dir, 2);
}

void Material::SetColor(float4 c)
{
	color = c;
}

void Material::RemoveDiffuse()
{
	SetTexture("", 0);
}

void Material::RemoveNormal()
{
	SetTexture("", 1);
}

void Material::RemoveSpecular()
{
	SetTexture("", 2);
}
