#ifndef CR_CONFIG_MANAGER
#define CR_CONFIG_MANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"

class ConfigManager
{
private:
	ConfigManager();
	static ConfigManager* ins;

	char* scenePath;

public:
	~ConfigManager();
	static ConfigManager* Instance();

	void LoadConfig();
	char* GetScenePath();
};

#endif