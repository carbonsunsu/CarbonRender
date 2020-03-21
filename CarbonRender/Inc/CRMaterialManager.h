#ifndef CR_MATMANAGER
#define CR_MATMANAGER

#include "..\Inc\CRMath.h"
#include "..\Inc\CRTextureManager.h"

class Material
{
private:
	string texDirs[3];
	GLuint texIns[3];
	float roughness;
	float metallic;

public:
	Material();
	~Material();
	void SetRoughness(float r);
	void SetMetallic(float m);
	void SetDiffuse(string dir);
	void SetNormal(string dir);
	void SetSpecular(string dir);

	float GetMetallic();
	float GetRoughness();
	string GetDiffusePath();
	GLuint GetDiffuse();
	string GetNormalPath();
	GLuint GetNormal();
	string GetSpecularPath();
	GLuint GetSpecular();
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