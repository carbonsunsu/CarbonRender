#include "..\Inc\CRController.h"

void Controller::Init()
{
}

void Controller::Update()
{
}

void Controller::KeyDownCallback(unsigned char key, int x, int y)
{
}

void Controller::KeyUpCallback(unsigned char key, int x, int y)
{
}

void Controller::SpecialKeyDownCallback(int key, int x, int y)
{
}

void Controller::SpecialKeyUpCallback(int key, int x, int y)
{
}

void Controller::MouseKeyCallback(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
	{
		if (state == GLUT_DOWN)
		{
			curMouseButton = GLUT_LEFT_BUTTON;
		}
	}
	break;
	case GLUT_RIGHT_BUTTON:
	{	
		if (state == GLUT_DOWN)
		{
			curMouseButton = GLUT_RIGHT_BUTTON;
		}
	}
	break;
	case GLUT_MIDDLE_BUTTON:
	{
		if (state == GLUT_DOWN)
		{
			curMouseButton = GLUT_MIDDLE_BUTTON;
		}
	}
	break;
	}
	lastMousePos[0] = x;
	lastMousePos[1] = y;
}

void Controller::MouseMotionCallback(int x, int y)
{
}
