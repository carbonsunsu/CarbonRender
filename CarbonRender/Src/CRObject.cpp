#include "..\Inc\CRObject.h"

Object::Object()
{
	firstChild = nullptr;
	parent = nullptr;
	previous = nullptr;
	next = nullptr;
	childCount = 0;
	name = "Object";
	objType = ObjectType::eNull;
	visible = true;
}

Object::~Object()
{
}

void Object::SetPosition(float3 p)
{
	transform[0] = p.x;
	transform[1] = p.y;
	transform[2] = p.z;
}

void Object::SetScale(float3 s)
{
	transform[3] = s.x;
	transform[4] = s.y;
	transform[5] = s.z;
}

void Object::SetRotation(float3 r)
{
	transform[6] = r.x;
	transform[7] = r.y;
	transform[8] = r.z;
}

void Object::UpdateModelMatrix()
{
	modelMatrix = Math::CalculateModelMatrix(localCoord,
										float3(transform[0], transform[1], transform[2]),
										float3(transform[6], transform[7], transform[8]),
										float3(transform[3], transform[4], transform[5]), false);
}

void Object::SetName(string n)
{
	name = n;
}

void Object::SetChildCount(unsigned int count)
{
	childCount = count;
}

void Object::SetChild(Object * child, unsigned int)
{
}

void Object::AddChild(Object * child)
{
	child->parent = this;
	if (childCount == 0)
		firstChild = child;
	else
	{
		Object* last = GetLastChild();
		last->next = child;
		child->previous = last;
	}

	childCount++;
}

void Object::SetFirstChild(Object * child)
{
	firstChild = child;
}

void Object::SetParent(Object * prnt)
{
	parent = prnt;
	prnt->AddChild(this);
}

void Object::SetPrevious(Object * pre)
{
	previous = pre;
}

void Object::SetNext(Object * nxt)
{
	next = nxt;
}

void Object::SetVisible(bool v)
{
	visible = v;
}

void Object::SetVisible(bool v, bool setAllChild, bool setAllParent)
{
	if (setAllParent)
	{
		Object* curParent = parent;
		while (curParent != nullptr)
		{
			curParent->SetVisible(v, false, true);
			curParent = curParent->GetParent();
		}
	}

	visible = v;

	if (setAllChild)
	{
		Object* curChild = firstChild;
		while (curChild != nullptr)
		{
			curChild->SetVisible(v, true, false);
			curChild = curChild->GetNext();
		}
	}
}

float3 Object::GetPosition()
{
	float3 result;
	result.x = transform[0];
	result.y = transform[1];
	result.z = transform[2];

	return result;
}

float3 Object::GetScale()
{
	float3 result;
	result.x = transform[3];
	result.y = transform[4];
	result.z = transform[5];

	return result;
}

float3 Object::GetRotation()
{
	float3 result;
	result.x = transform[6];
	result.y = transform[7];
	result.z = transform[8];

	return result;
}

float3 Object::GetForward()
{
	return float3(-localCoord[6], -localCoord[7], -localCoord[8]);
}

float3 Object::GetRight()
{
	return float3(localCoord[0], localCoord[1], localCoord[2]);
}

float3 Object::GetUp()
{
	return float3(localCoord[3], localCoord[4], localCoord[5]);
}

Matrix4x4 Object::GetModelMatrix()
{
	return modelMatrix;
}

string Object::GetName()
{
	return name;
}

unsigned int Object::GetChildCount()
{
	return childCount;
}

Object * Object::GetParent()
{
	return parent;
}

Object * Object::GetFirstChild()
{
	return firstChild;
}

Object * Object::GetPrevious()
{
	return previous;
}

Object * Object::GetNext()
{
	return next;
}

Object * Object::GetLastChild()
{
	if (childCount == 0)
		return nullptr;

	Object* current = firstChild;
	while (true)
	{
		if (current->next == nullptr)
			return current;
		else
			current = current->next;
	}
}

ObjectType Object::GetType()
{
	return objType;
}

bool Object::IsVisible()
{
	return visible;
}

void Object::LookAt(float3 p)
{
	float3 v = p - float3(transform[0], transform[1], transform[2]);
	float3 vXZ = float3(v.x, 0.0f, v.z).normalize();
	v = v.normalize();

	float thetaY = Math::Dot(float3(0.0f, 0.0f, -1.0f), vXZ);
	thetaY = acos(thetaY) * R2A;
	if (v.x < 0.0f) thetaY = -thetaY;

	float thetaX = Math::Dot(v, vXZ);
	thetaX = acos(thetaX) * R2A;
	if (v.y > 0.0f) thetaX = -thetaX;

	SetRotation(float3(thetaX, thetaY, transform[8]));
}
