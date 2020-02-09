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
	void KeyDownCallback(unsigned char key, int x, int y);
	void KeyUpCallback(unsigned char key, int x, int y);

	void SpecialKeyDownCallback(int key, int x, int y);
	void SpecialKeyUpCallback(int key, int x, int y);

	void MouseMotionCallback(int x, int y);
};

#endif
