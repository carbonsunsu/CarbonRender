#ifndef CR_MATMANAGER
#define CR_MATMANAGER

#include "..\Inc\CRMath.h"
#include "..\Inc\CRTextureManager.h"

class Material
{
private:
	Material();
	~Material();
	unsigned int id;
	string name;
	string texDirs[3];
	GLuint texIns[3];
	bool hasTex[3];
	float texTilling[6];
	float roughness;
	float metallic;
	float4 color;
	float3 emissionColor;
	float emissionIntensity;
	void SetTexture(string dir, int index);
	void SetID(unsigned int newID);

public:
	friend class MaterialManager;
	
	void SetName(string n);
	void SetRoughness(float r);
	void SetMetallic(float m);
	void SetDiffuse(string dir);
	void SetNormal(string dir);
	void SetSpecular(string dir);
	void SetColor(float4 c);
	void SetDiffuseTilling(float x, float y);
	void SetNormalTilling(float x, float y);
	void SetSpecularTilling(float x, float y);
	void SetEmissionColor(float3 c);
	void SetEmissionIntensity(float i);

	void RemoveDiffuse();
	void RemoveNormal();
	void RemoveSpecular();

	unsigned int GetID();
	string GetName();
	float GetMetallic();
	float GetRoughness();
	string GetDiffusePath();
	GLuint GetDiffuse();
	string GetNormalPath();
	GLuint GetNormal();
	string GetSpecularPath();
	GLuint GetSpecular();
	float4 GetColor();
	float3 GetDiffuseTilling();
	float3 GetNormalTilling();
	float3 GetSpecularTilling();
	float3 GetEmissionColor();
	float GetEmissionIntensity();
	bool HasDiffuseTexture();
	bool HasNormalTexture();
	bool HasSpecularTexture();
};

class MaterialManager
{
private:
	MaterialManager();
	static MaterialManager* ins;
	
	unordered_map<unsigned int, Material*> materials;
	Material defaultMaterial;

	unsigned int nextMatID;
public:
	friend class SceneManager;
	friend class MenuManager;
	~MaterialManager();
	static MaterialManager* Instance();

	Material* CreateNewMaterial(unsigned int id, string name);
	Material* CreateNewMaterial();
	Material* CreateNewMaterial(string name, bool findSame = true);
	Material* GetMaterial(unsigned int id);
	Material* GetDefaultMaterial();
	void DeleteMaterial(unsigned int id);
	void RemoveAllMaterials();
};

#endif