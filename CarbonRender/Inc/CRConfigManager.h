#ifndef CR_CONFIG_MANAGER
#define CR_CONFIG_MANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRWindowManager.h"

class ConfigManager
{
private:
	ConfigManager();
	static ConfigManager* ins;

	char* scenePath;
	bool bFullScreen;
	WindowSize screenSize;

public:
	~ConfigManager();
	static ConfigManager* Instance();

	void LoadConfig();
	char* GetScenePath();
	bool IsFullScreen();
	WindowSize GetScreenSize();
};

#endif