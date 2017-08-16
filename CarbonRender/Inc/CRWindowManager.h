#ifndef CR_WINMANAGER
#define CR_WINMANAGER

#include "..\Inc\CRGloble.h"

class Camera;

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

public :
	~WindowManager();
	static WindowManager* Instance();
	void ReSize(unsigned int w, unsigned h);
	void CreateWindow(unsigned int w, unsigned int h, char* tittle);
	WindowSize GetWindowSize();

};

#endif 
