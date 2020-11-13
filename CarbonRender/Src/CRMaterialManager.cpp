#include "..\Inc\CRMaterialManager.h"

MaterialManager* MaterialManager::ins = nullptr;

MaterialManager::MaterialManager()
{
	nextMatID = 1;
	defaultMaterial.SetDiffuse("");
	defaultMaterial.SetNormal("");
	defaultMaterial.SetSpecular("");
}

MaterialManager::~MaterialManager()
{
	RemoveAllMaterials();
	ins = nullptr;
}

MaterialManager * MaterialManager::Instance()
{
	if (ins == nullptr)
		ins = new MaterialManager();

	return ins;
}

Material * MaterialManager::CreateNewMaterial(unsigned int id, string name)
{
	if (materials.find(id) != materials.end())
		return materials[id];

	Material* newMat = new Material();
	newMat->SetID(id);
	newMat->SetName(name);
	nextMatID = Math::Max(id, nextMatID);
	materials[id] = newMat;

	return newMat;
}

Material * MaterialManager::CreateNewMaterial()
{
	return CreateNewMaterial("New Material " + to_string(nextMatID), false);
}

Material * MaterialManager::CreateNewMaterial(string name, bool findSame)
{
	if (findSame)
	{
		for (unordered_map<unsigned int, Material*>::iterator i = materials.begin(); i != materials.end(); i++)
		{
			if (i->second->GetName() == name)
				return i->second;
		}
	}

	Material* newMat = new Material();
	newMat->SetID(nextMatID);
	nextMatID++;
	newMat->SetName(name);
	materials[newMat->GetID()] = newMat;

	return newMat;
}

Material * MaterialManager::GetMaterial(unsigned int id)
{
	if (materials.find(id) != materials.end())
		return materials[id];
	else
		return GetDefaultMaterial();
}

Material * MaterialManager::GetDefaultMaterial()
{
	return &defaultMaterial;
}

void MaterialManager::DeleteMaterial(unsigned int id)
{
	if (materials.find(id) != materials.end())
	{
		delete materials[id];
		materials.erase(id);
	}
}

void MaterialManager::RemoveAllMaterials()
{
	for (unordered_map<unsigned int, Material*>::iterator i = materials.begin(); i != materials.end(); i++)
	{
		delete i->second;
		materials.erase(i->first);
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

void Material::SetID(unsigned int newID)
{
	id = newID;
}

Material::Material()
{
	id = 0;
	name = "New Material";
	roughness = 0.5f;
	metallic = 0.5f;
	texIns[0] = 0;
	texIns[1] = 0;
	texIns[2] = 0;
	hasTex[0] = false;
	hasTex[1] = false;
	hasTex[2] = false;
	texTilling[0] = 1.0f;
	texTilling[1] = 1.0f;
	texTilling[2] = 1.0f;
	texTilling[3] = 1.0f;
	texTilling[4] = 1.0f;
	texTilling[5] = 1.0f;
	color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	emissionColor = float3(0.0f, 0.0f, 0.0f);
	emissionIntensity = 1.0f;
	alphaTestThreshold = 0.5f;
	SetDiffuse("");
	SetNormal("");
	SetSpecular("");
}

Material::~Material()
{
	RemoveDiffuse();
	RemoveNormal();
	RemoveSpecular();
}

void Material::SetName(string n)
{
	name = n;
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

float3 Material::GetDiffuseTilling()
{
	return float3(texTilling[0], texTilling[1], 0.0f);
}

float3 Material::GetNormalTilling()
{
	return float3(texTilling[2], texTilling[3], 0.0f);
}

float3 Material::GetSpecularTilling()
{
	return float3(texTilling[4], texTilling[5], 0.0f);
}

float3 Material::GetEmissionColor()
{
	return emissionColor;
}

float Material::GetEmissionIntensity()
{
	return emissionIntensity;
}

float Material::GetAlphaTestThreshold()
{
	return alphaTestThreshold;
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

void Material::SetDiffuseTilling(float x, float y)
{
	texTilling[0] = x;
	texTilling[1] = y;
}

void Material::SetNormalTilling(float x, float y)
{
	texTilling[2] = x;
	texTilling[3] = y;
}

void Material::SetSpecularTilling(float x, float y)
{
	texTilling[4] = x;
	texTilling[5] = y;
}

void Material::SetEmissionColor(float3 c)
{
	emissionColor = c;
}

void Material::SetEmissionIntensity(float i)
{
	emissionIntensity = i;
}

void Material::SetAlphaTestThreshold(float t)
{
	alphaTestThreshold = t;
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

unsigned int Material::GetID()
{
	return id;
}

string Material::GetName()
{
	return name;
}
