#ifndef CR_CONFIG_MANAGER
#define CR_CONFIG_MANAGER

#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRWindowManager.h"

class ConfigManager
{
private:
	ConfigManager();
	static ConfigManager* ins;

	bool bFullScreen;
	WindowSize screenSize;

public:
	~ConfigManager();
	static ConfigManager* Instance();

	void LoadConfig();
	bool IsFullScreen();
	WindowSize GetScreenSize();
};

#endif