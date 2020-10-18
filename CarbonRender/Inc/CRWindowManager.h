#ifndef CR_WINMANAGER
#define CR_WINMANAGER

#include "..\Inc\CRGloble.h"

struct  WindowSize
{
	unsigned int w;
	unsigned int h;
};

class WindowManager
{
private:
	static WindowManager* ins;
	WindowManager();
	WindowSize size;
	GLFWwindow* window;

public :
	~WindowManager();
	static WindowManager* Instance();
	void ReSize(unsigned int w, unsigned h);
	void CreateWindow(unsigned int w, unsigned int h, char* tittle, bool fullScreen);
	WindowSize GetWindowSize();
	GLFWwindow* GetWindow();

};

#endif 
