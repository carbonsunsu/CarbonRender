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
	/**
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
	/**/

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::Instance()->GetWindow(), true);
	ImGui_ImplOpenGL3_Init();
}

void MenuManager::OnMainMenu(int value)
{
	/**
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
	/**/
}

void MenuManager::OnImportModelMenu(int value)
{
	/**
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
	/**/
}

void MenuManager::UpdateModelList()
{
	/**
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
	/**/
}

void MenuManager::RenderMenu()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuManager::UpdateMenu()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("CarbonRender");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	bool tmp0 = true;
	bool tmp1 = false;
	ImGui::Checkbox("Demo Window", &tmp0);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &tmp1);

	float f = 0.0f;
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImVec4 tmp2 = ImVec4(0.5, 0.2, 0.4, 0.1);
	ImGui::ColorEdit3("clear color", (float*)&tmp2); // Edit 3 floats representing a color

	int counter = 0;
	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
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
