#ifndef CR_CONTROLLER
#define CR_CONTROLLER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRCameraManager.h"

class Controller
{
protected:
	int lastMousePos[2];
	float sensitivity[2] = { 0.02f, 0.01f };
	int curMouseButton;
	float moveSpeed = 2.0f;
	float3 v;

public:
	void Init();
	void Update();
	void KeyDownCallback(unsigned char key, int x, int y);
	void KeyUpCallback(unsigned char key, int x, int y);
	void MouseKeyCallback(int button, int state, int x, int y);
	void MouseMotionCallback(int x, int y);
};

#endif