#include "..\Inc\CRMenuManager.h"

MenuManager* MenuManager::ins = nullptr;
int MenuManager::mainMenu = -1;
int MenuManager::importModelMenu = -1;
vector<string> MenuManager::modelList;

MenuManager::MenuManager()
{
}

void MenuManager::InitMenu()
{
	mainMenu = glutCreateMenu(OnMainMenu);
	importModelMenu = glutCreateMenu(OnImportModelMenu);

	glutSetMenu(mainMenu);
	glutAddSubMenu("Import new model", importModelMenu);
	glutAddMenuEntry("Save current scene", 0);

	glutSetMenu(importModelMenu);
	glutAddMenuEntry("Update model list", 0);

	UpdateModelList();

	glutSetMenu(mainMenu);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void MenuManager::OnMainMenu(int value)
{
	switch (value)
	{
	case 0:
	{
		SceneManager::Instance()->SaveScene(ConfigManager::Instance()->GetScenePath());
	}
	break;
	default:
		break;
	}
}

void MenuManager::OnImportModelMenu(int value)
{
	switch (value)
	{
	case 0:
	{
		UpdateModelList();
	}
	break;
	default:
	{
		Object* newObj = new Object();
		SceneManager::Instance()->GetRootNode()->AddChild(newObj);

		string fileName = modelList[value - 1];
		int pos = fileName.find(".fbx");
		if (pos >= 0)
			fileName.erase(pos);

		FbxImportManager::Instance()->ImportFbxModel((char *)fileName.c_str(), newObj);
	}
	break;
	}
}

void MenuManager::UpdateModelList()
{
	glutSetMenu(importModelMenu);

	modelList.clear();
	int itemCount = glutGet(GLUT_MENU_NUM_ITEMS);
	for (int i = 2; i <= itemCount; i++)
		glutRemoveMenuItem(2);

	itemCount = glutGet(GLUT_MENU_NUM_ITEMS);
	
	string path = "Resources\\Models";
	for (const auto & entry : directory_iterator(path))
	{
		if (strstr(entry.path().string().c_str(), ".fbx"))
		{
			modelList.emplace_back(entry.path().filename().string());
			string path = modelList[modelList.size() - 1];
			glutAddMenuEntry((const char*)path.c_str(), modelList.size());
		}
	}

	itemCount = glutGet(GLUT_MENU_NUM_ITEMS);
}

MenuManager::~MenuManager()
{
	modelList.clear();
	modelList.shrink_to_fit();
	ins = nullptr;
}

MenuManager * MenuManager::Instance()
{
	if (ins == nullptr)
		ins = new MenuManager();

	return ins;
}
