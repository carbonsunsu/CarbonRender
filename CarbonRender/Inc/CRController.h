#ifndef CR_CONTROLLER
#define CR_CONTROLLER

#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRWeatherSystem.h"

class Controller
{
protected:
	double lastMousePos[2];
	int curMouseButton;

public:
	virtual void Init();
	virtual void Update();
	virtual void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

	virtual void MouseKeyCallback(GLFWwindow* window, int button, int state);
	virtual void MouseMotionCallback(GLFWwindow* window, double x, double y);
	virtual void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif