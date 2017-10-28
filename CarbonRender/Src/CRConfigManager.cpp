#include "..\Inc\CRConfigManager.h"

ConfigManager* ConfigManager::ins = nullptr;

ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{
}

ConfigManager * ConfigManager::Instance()
{
	if (ins == nullptr)
		ins = new ConfigManager();

	return ins;
}

void ConfigManager::LoadConfig()
{
	TextFile configFile = FileReader::ReadTextFile("Config.xml");
	xml_document<> configData;
	configData.parse<0>(configFile.data);

	xml_node<>* root = configData.first_node();
	xml_node<>* l1Node = root->first_node("Scene");
	scenePath = l1Node->value();
}

char * ConfigManager::GetScenePath()
{
	return scenePath;
}
