#ifndef CR_OBJECT
#define CR_OBJECT

#include "..\Inc\CRMath.h"

enum ObjectType
{
	eNull = 0,
	eMesh,
	eLight,
	eCamera,
};

class Object
{
protected:
	ObjectType objType;

	float transform[9] = { 0.0f, 0.0f, 0.0f,	//position
						   1.0f, 1.0f, 1.0f,	//scale
						   0.0f, 0.0f, 0.0f};	//rotation

	float localCoord[9] = { 1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f };

	Matrix4x4 modelMatrix;

	string name;

	unsigned int childCount;
	Object* parent;
	Object* firstChild;
	Object* next;
public:
	Object();
	~Object();
	void SetPosition(float3 p);
	void SetScale(float3 s);
	void SetRotation(float3 r);
	void UpdateModelMatrix();
	void SetName(string n);
	void SetChildCount(unsigned int count);
	void SetChild(Object* child, unsigned int);
	void AddChild(Object* child);
	void SetParent(Object* prnt);

	float3 GetPosition();
	float3 GetScale();
	float3 GetRotation();
	float3 GetForward();
	float3 GetRight();
	float3 GetUp();
	Matrix4x4 GetModelMatrix();
	string GetName();
	unsigned int GetChildCount();
	Object* GetLastChild();
	void LookAt(float3 p);
};

#endif