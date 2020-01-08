#ifndef CR_CONTROLLER
#define CR_CONTROLLER

#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRWeatherSystem.h"

class Controller
{
protected:
	int lastMousePos[2];
	int curMouseButton;

public:
	virtual void Init();
	virtual void Update();
	virtual void KeyDownCallback(unsigned char key, int x, int y);
	virtual void KeyUpCallback(unsigned char key, int x, int y);

	virtual void SpecialKeyDownCallback(int key, int x, int y);
	virtual void SpecialKeyUpCallback(int key, int x, int y);

	virtual void MouseKeyCallback(int button, int state, int x, int y);
	virtual void MouseMotionCallback(int x, int y);
};

#endif