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

void WindowManager::CreateWindow(unsigned int w, unsigned int h, char * title)
{
	size.w = w;
	size.h = h;
	glutInitWindowSize((int)size.w, (int)size.h);
	glutCreateWindow(title);
	glViewport(0, 0, (GLsizei)size.w, (GLsizei)size.h);
}

WindowSize WindowManager::GetWindowSize()
{
	return size;
}
