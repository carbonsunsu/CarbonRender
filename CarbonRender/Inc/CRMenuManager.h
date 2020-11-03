#ifndef CR_MENU_MANAGER
#define CR_MENU_MANAGER

#include "..\Inc\CRSceneManager.h"
#include "..\Inc\CRConfigManager.h"

class MenuManager;
typedef void (MenuManager::*FileImportCallback)(string);

class MenuManager
{
private:
	MenuManager();
	static MenuManager* ins;

	bool showMenu;
	ImGuiContext* guiContext;

	Object* selectedObj;
	string selectedFileName;
	string startPath;
	string curPath;
	string fileSuffix;
	char curSceneNameChar[32];
	FileImportCallback fileImportCallback;
	int sceneTreeNodeIndex;

	bool showWorldEditorDialog;
	bool showSceneEditorDialog;
	bool showObjectEditorDialog;
	bool showFileBroser;
	bool showSaveSceneDialog;

	void DrawMainMenuBar();
	void DrawMainMenuBar_Scene();
	void DrawMainMenuBar_Editor();
	void DrawWorldEditorDialog();
	void DrawSceneNode(Object * node, ImGuiTreeNodeFlags flags);
	void DrawSceneEditorDialog();
	void DrawObjectEditorDialog();
	void DrawSaveSceneDialog();

	void InitFileBrowser(string path, string suffix, FileImportCallback callback);
	void DrawFileBrowser();

	void ImportModel(string path);
	void ImportDiffuse(string path);
	void ImportNormal(string path);
	void ImportSpecular(string path);
	void OpenNewScene(string path);

public:
	~MenuManager();
	static MenuManager* Instance();
	void InitMenu();
	void RenderMenu();
	void UpdateMenu();
	void ToogleMenu();
	bool MenuStatus();
	bool MouseOnMenu();
	Object* GetSelectedObj();
	void ResetSelectedObj();
};

#endif