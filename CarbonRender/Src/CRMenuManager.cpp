#include "..\Inc\CRMenuManager.h"

MenuManager* MenuManager::ins = nullptr;

MenuManager::MenuManager()
{
}

MenuManager::~MenuManager()
{
}

MenuManager * MenuManager::Instance()
{
	if (ins == nullptr)
		ins = new MenuManager();

	return ins;
}
