#ifndef CR_MENU_MANAGER
#define CR_MENU_MANAGER

#include "..\Inc\CRFileReader.h"

class MenuManager
{
private:
	MenuManager();
	static MenuManager* ins;

public:
	~MenuManager();
	static MenuManager* Instance();
};

#endif