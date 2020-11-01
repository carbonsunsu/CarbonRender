#ifndef CR_MENU_MANAGER
#define CR_MENU_MANAGER

#include "..\Inc\CRSceneManager.h"
#include "..\Inc\CRConfigManager.h"

class MenuManager
{
private:
	MenuManager();
	static MenuManager* ins;

	bool showMenu;
	ImGuiContext* guiContext;
	vector<string> modelList;

	Object* selectedObj;

	bool showModelImportDialog;
	bool showWorldEditorDialog;
	bool showSceneEditorDialog;
	bool showObjectEditorDialog;

	void DrawMainMenuBar();
	void DrawMainMenuBar_Scene();
	void DrawMainMenuBar_Editor();
	void DrawImportModelDialog();
	void DrawWorldEditorDialog();
	void DrawSceneNode(Object * node, ImGuiTreeNodeFlags flags);
	void DrawSceneEditorDialog();
	void DrawObjectEditorDialog();

public:
	~MenuManager();
	static MenuManager* Instance();
	void InitMenu();
	void RenderMenu();
	void UpdateMenu();
	void ToogleMenu();
	bool MenuStatus();
	bool MouseOnMenu();
};

#endif