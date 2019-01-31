#ifndef CR_MENU_MANAGER
#define CR_MENU_MANAGER

#include "..\Inc\CRSceneManager.h"
#include "..\Inc\CRConfigManager.h"

class MenuManager
{
private:
	MenuManager();
	static MenuManager* ins;

	static int mainMenu, importModelMenu;
	static vector<string> modelList;
	
	static void OnMainMenu(int value);
	static void OnImportModelMenu(int value);
	static void UpdateModelList();
public:
	~MenuManager();
	static MenuManager* Instance();
	static void InitMenu();
};

#endif