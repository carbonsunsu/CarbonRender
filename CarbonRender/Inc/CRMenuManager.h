#ifndef CR_MENU_MANAGER
#define CR_MENU_MANAGER

#include "..\Inc\CRSceneManager.h"
#include "..\Inc\CRConfigManager.h"

class MenuManager;
typedef void (MenuManager::*FileImportCallback)(string);
typedef void (MenuManager::*InputPopupCallback)(string);

class MenuManager
{
private:
	MenuManager();
	static MenuManager* ins;

	bool showMenu;
	ImGuiContext* guiContext;

	Object* selectedObj;
	string selectedFileName;
	unsigned int selectedMatID;
	string startPath;
	string curPath;
	string fileSuffix;
	char curSceneNameChar[32];
	FileImportCallback fileImportCallback;
	InputPopupCallback inputPopupCallback;
	int sceneTreeNodeIndex;
	string inputPopupTitle;
	char newInputInitChar[64];

	bool showWorldEditorDialog;
	bool showSceneEditorDialog;
	bool showObjectEditorDialog;
	bool showFileBroser;
	bool showSaveSceneDialog;
	bool showMaterialManager;
	bool showInputPopup;

	void DrawMainMenuBar();
	void DrawMainMenuBar_Scene();
	void DrawMainMenuBar_Editor();
	void DrawWorldEditorDialog();
	void DrawSceneNode(Object * node, ImGuiTreeNodeFlags flags);
	void DrawSceneEditorDialog();
	void DrawObjectEditorDialog();
	void DrawSaveSceneDialog();
	void DrawMaterialManagerDialog();

	void InitFileBrowser(string path, string suffix, FileImportCallback callback);
	void DrawFileBrowser();

	void InitInputPopup(string title, string defaultChar, InputPopupCallback callback);
	void DrawInputPopup();

	void ImportModel(string path);
	void ImportDiffuse(string path);
	void ImportNormal(string path);
	void ImportSpecular(string path);
	void OpenNewScene(string path);
	void ImportWeatherMap(string path);
	void CreateNewMaterial(string name);

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