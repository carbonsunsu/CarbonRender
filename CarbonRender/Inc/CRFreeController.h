#ifndef CR_FREE_CONTROLLER
#define CR_FREE_CONTROLLER

#include "..\Inc\CRController.h"

class FreeController : public Controller
{
protected:
	float sensitivity[2] = { 0.02f, 0.01f };
	float moveSpeedLow = 2.0f;
	float moveSpeedHigh;
	float moveSpeed;
	float3 v;
	bool highSpeed;

public:
	void Init();
	void Update();
	void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

	void MouseMotionCallback(GLFWwindow* window, double x, double y);
};

#endif
