#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRWeatherSystem.h"
#include "..\Inc\CRControllerManager.h"
#include "..\Inc\CRTextureManager.h"
#include "..\Inc\CRSceneManager.h"
#include "..\Inc\CRRenderPassManager.h"
#include "..\Inc\CRConfigManager.h"
#include "..\Inc\CRMaterialManager.h"
#include "..\Inc\CRMeshManager.h"
#include "..\Inc\CRMenuManager.h"
#include "..\Inc\CRTerrainManager.h"
#include "..\Inc\CRTimer.h"

void MainDisplay()
{
	CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();
	
	if (TerrainManager::Instance()->UseTerrain())
		TerrainManager::Instance()->GetTerrainObject()->Update(CameraManager::Instance()->GetCurrentCamera());

	if (TerrainManager::Instance()->UseOcean())
		TerrainManager::Instance()->GetOceanObject()->Update(CameraManager::Instance()->GetCurrentCamera());

	RenderPassManager::Instance()->Draw();

	MenuManager::Instance()->UpdateMenu();
	MenuManager::Instance()->RenderMenu();

	glfwSwapBuffers(WindowManager::Instance()->GetWindow());
}

void ReSizeCallback(int w, int h)
{
	WindowManager::Instance()->ReSize((unsigned int)w, (unsigned int)h);
	Camera* cam = CameraManager::Instance()->GetCurrentCamera();
	if (cam != nullptr)
		cam->UpdateProjectionMatrix();
}

void Init(int argc, char** argv)
{
	ConfigManager::Instance()->LoadConfig();

	if (!glfwInit())
		cout << "GLFW init fail" << endl;

	WindowSize size = ConfigManager::Instance()->GetScreenSize();
	WindowManager::Instance()->CreateMainWindow(size.w, size.h, "CarbonRender", ConfigManager::Instance()->IsFullScreen());
	glfwSwapInterval(1);

	const GLenum glewError = glewInit();
	if (glewError)
		cout << glewGetErrorString(glewError) << endl;

	MenuManager::Instance()->InitMenu();

	string str = "@carbonsunsu";
	cout << str << endl;
	str = string("OpenGL Version: ") + string((GLchar *)glGetString(GL_VERSION));
	cout << str << endl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	TextureManager::Instance();
	MeshManager::Instance();
	WeatherSystem::Instance();

	SceneManager::Instance()->LoadDefaultScene();

	//Create controller
	FreeController *ctrl = new FreeController();
	ctrl->Init();
	ControllerManager::Instance()->Push(ctrl);

	RenderPassManager::Instance()->Init();
	TerrainManager::Instance();
}

void FixedUpdate()
{
	WeatherSystem::Instance()->Update();
	ControllerManager::Instance()->GetCurrentController()->Update();
}

void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	ControllerManager::Instance()->GetCurrentController()->KeyInputCallback(window, key, scanCode, action, mods);
	ImGui_ImplGlfw_KeyCallback(window, key, scanCode, action, mods);
}

void MouseKeyCallback(GLFWwindow* window, int button, int state, int mods)
{
	ControllerManager::Instance()->GetCurrentController()->MouseKeyCallback(window, button, state);
	ImGui_ImplGlfw_MouseButtonCallback(window, button, state, mods);
}

void MouseMotionCallback(GLFWwindow* window, double x, double y) 
{
	ControllerManager::Instance()->GetCurrentController()->MouseMotionCallback(window, x, y);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	ControllerManager::Instance()->GetCurrentController()->ScrollCallback(window, xOffset, yOffset);
	ImGui_ImplGlfw_ScrollCallback(window, xOffset, yOffset);
}

void main(int argc, char** argv)
{
	Init(argc, argv);

	unsigned int timerID = Timer::Instance()->NewTimer();

	glfwSetMouseButtonCallback(WindowManager::Instance()->GetWindow(), MouseKeyCallback);
	glfwSetCursorPosCallback(WindowManager::Instance()->GetWindow(), MouseMotionCallback);
	glfwSetKeyCallback(WindowManager::Instance()->GetWindow(), KeyInputCallback);
	//glfwSetScrollCallback(WindowManager::Instance()->GetWindow(), ScrollCallback);

	while (!glfwWindowShouldClose(WindowManager::Instance()->GetWindow()))
	{
		if (Timer::Instance()->GetTime() - Timer::Instance()->LastUpdateTime(timerID) >= FIXEDUPDATE_TIME)
		{
			Timer::Instance()->UpdateTimer(timerID);
			FixedUpdate();
		}

		MainDisplay();
		glfwPollEvents();
	}

	glfwDestroyWindow(WindowManager::Instance()->GetWindow());
	glfwTerminate();
}