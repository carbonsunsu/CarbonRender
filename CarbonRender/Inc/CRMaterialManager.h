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
	float GetRoughness();
	void SetMetallic(float m);
	float GetMetallic();
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