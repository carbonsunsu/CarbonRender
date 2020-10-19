#include "..\Inc\CRWindowManager.h"

WindowManager* WindowManager::ins = nullptr;

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
	ins = nullptr;
}

WindowManager * WindowManager::Instance()
{
	if (ins == nullptr)
		ins = new WindowManager();

	return ins;
}

void WindowManager::ReSize(unsigned int newW, unsigned newH)
{
	size.w = newW;
	size.h = newH;

	glViewport(0, 0, (GLsizei)size.w, (GLsizei)size.h);
}

void WindowManager::CreateMainWindow(unsigned int w, unsigned int h, char * title, bool fullScreen)
{
	size.w = w;
	size.h = h;
	if (fullScreen)
		window = glfwCreateWindow((int)size.w, (int)size.h, title, glfwGetPrimaryMonitor(), NULL);
	else
		window = glfwCreateWindow((int)size.w, (int)size.h, title, NULL, NULL);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, (GLsizei)size.w, (GLsizei)size.h);
}

WindowSize WindowManager::GetWindowSize()
{
	return size;
}

GLFWwindow * WindowManager::GetWindow()
{
	return window;
}
