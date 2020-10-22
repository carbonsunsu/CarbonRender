#include "..\Inc\CRController.h"

void Controller::Init()
{
}

void Controller::Update()
{
}

void Controller::KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
}

void Controller::MouseKeyCallback(GLFWwindow* window, int button, int state)
{
	switch (state)
	{
	case GLFW_PRESS:
	{
		curMouseButton = button;
	}
	break;
	case GLFW_RELEASE:
	{	
		curMouseButton = -1;
	}
	break;
	}

	glfwGetCursorPos(window, &lastMousePos[0], &lastMousePos[1]);
}

void Controller::MouseMotionCallback(GLFWwindow* window, double x, double y)
{
}

void Controller::ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
}
