#ifndef CR_CINEMATIC_CONTROLLER
#define CR_CINEMATIC_CONTROLLER

#include "..\Inc\CRControllerManager.h"
#include "..\Inc\CRMenuManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRWeatherSystem.h"

class CinematicController : public Controller
{
protected:
	bool end;
	float filmStartTime;

	float GetCurrentTime();

public:
	void Init();
	void Update();
	void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

	void MouseMotionCallback(GLFWwindow* window, double x, double y);
	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif