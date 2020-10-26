#ifndef CR_FREE_CONTROLLER
#define CR_FREE_CONTROLLER

#include "..\Inc\CRController.h"
#include "..\Inc\CRMenuManager.h"

class FreeController : public Controller
{
protected:
	float rotSensitivity[2] = { 0.02f, 0.01f };
	float mouseMovSensitivity[3] = { 0.2f, 0.1f, 0.1f };
	float moveSpeedLow = 2.0f;
	float moveSpeedHigh;
	float moveSpeed;
	float3 v;
	bool ctrlPressed;


public:
	void Init();
	void Update();
	void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

	void MouseMotionCallback(GLFWwindow* window, double x, double y);
	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif
