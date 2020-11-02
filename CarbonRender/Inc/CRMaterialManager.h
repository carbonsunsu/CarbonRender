#ifndef CR_MATMANAGER
#define CR_MATMANAGER

#include "..\Inc\CRMath.h"
#include "..\Inc\CRTextureManager.h"

class Material
{
private:
	string texDirs[3];
	GLuint texIns[3];
	bool hasTex[3];
	float roughness;
	float metallic;
	float4 color;
	void SetTexture(string dir, int index);

public:
	Material();
	~Material();
	void SetRoughness(float r);
	void SetMetallic(float m);
	void SetDiffuse(string dir);
	void SetNormal(string dir);
	void SetSpecular(string dir);
	void SetColor(float4 c);
	void RemoveDiffuse();
	void RemoveNormal();
	void RemoveSpecular();

	float GetMetallic();
	float GetRoughness();
	string GetDiffusePath();
	GLuint GetDiffuse();
	string GetNormalPath();
	GLuint GetNormal();
	string GetSpecularPath();
	GLuint GetSpecular();
	float4 GetColor();
	bool HasDiffuseTexture();
	bool HasNormalTexture();
	bool HasSpecularTexture();
};

class MaterialManager
{
private:
	MaterialManager();
	static MaterialManager* ins;
	
	vector<Material*> materials;
	Material defaultMaterial;
public:
	~MaterialManager();
	static MaterialManager* Instance();

	Material* CreateNewMaterial();
	Material* GetDefaultMaterial();
};

#endif