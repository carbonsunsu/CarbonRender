#include "..\Inc\CRConfigManager.h"

ConfigManager* ConfigManager::ins = nullptr;

ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{
	ins = nullptr;
}

ConfigManager * ConfigManager::Instance()
{
	if (ins == nullptr)
		ins = new ConfigManager();

	return ins;
}

void ConfigManager::LoadConfig()
{
	TextFile configFile = FileReader::ReadTextFile("Config.cfg");
	xml_document<> configData;
	configData.parse<0>(configFile.data);

	xml_node<>* root = configData.first_node();
	xml_node<>* l1Node = root->first_node("LoadScene");
	scenePath = l1Node->value();

	l1Node = root->first_node("FullScreen");
	bFullScreen = (bool)atoi(l1Node->value());

	l1Node = root->first_node("ScreenWidth");
	screenSize.w = atoi(l1Node->value());

	l1Node = root->first_node("ScreenHeight");
	screenSize.h = atoi(l1Node->value());
}

string ConfigManager::GetScenePath()
{
	return scenePath;
}

bool ConfigManager::IsFullScreen()
{
	return bFullScreen;
}

WindowSize ConfigManager::GetScreenSize()
{
	return screenSize;
}
