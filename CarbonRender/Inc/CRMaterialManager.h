#ifndef CR_MATMANAGER
#define CR_MATMANAGER

#include "..\Inc\CRMath.h"

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
};

class MaterialManager
{
private:
	MaterialManager();
	static MaterialManager* ins;

public:
	~MaterialManager();
	static MaterialManager* Instance();
};

#endif