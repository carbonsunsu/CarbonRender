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

void MainDisplay()
{
	RenderPassManager::Instance()->Draw();

	glutSwapBuffers();
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

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	WindowManager::Instance()->CreateWindow(1600, 900, "CarbonRender");

	if (glewInit())
		cout << "GLEW init fail" << endl;

	string str = "@carbonsunsu";
	cout << str << endl;
	str = string("OpenGL Version: ") + string((GLchar *)glGetString(GL_VERSION));
	cout << str << endl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);

	WeatherSystem::Instance();

	SceneManager::Instance()->LoadScene(ConfigManager::Instance()->GetScenePath());

	RenderPassManager::Instance()->Init();
}

void FixedUpdate(int value)
{
	WeatherSystem::Instance()->Update();
	ControllerManager::Instance()->GetCurrentController()->Update();

	glutTimerFunc((unsigned int)(FIXEDUPDATE_TIME * 1000.0f), FixedUpdate, 0);
}

void KeyDownCallback(unsigned char key, int x, int y)
{
	ControllerManager::Instance()->GetCurrentController()->KeyDownCallback(key, x, y);
}

void KeyUpCallback(unsigned char key, int x, int y)
{
	ControllerManager::Instance()->GetCurrentController()->KeyUpCallback(key, x, y);
}

void SpecialKeyDownCallback(int key, int x, int y)
{
	ControllerManager::Instance()->GetCurrentController()->SpecialKeyDownCallback(key, x, y);
}

void SpecialKeyUpCallback(int key, int x, int y)
{
	ControllerManager::Instance()->GetCurrentController()->SpecialKeyUpCallback(key, x, y);
}

void MouseKeyCallback(int button, int state, int x, int y)
{
	ControllerManager::Instance()->GetCurrentController()->MouseKeyCallback(button, state, x, y);
}

void MouseMotionCallback(int x, int y) 
{
	ControllerManager::Instance()->GetCurrentController()->MouseMotionCallback(x, y);
}

void main(int argc, char** argv)
{
	Init(argc, argv);
	
	glutDisplayFunc(MainDisplay);
	glutIdleFunc(MainDisplay);
	glutReshapeFunc(ReSizeCallback);

	glutTimerFunc((unsigned int)(FIXEDUPDATE_TIME * 1000.0f), FixedUpdate, 0);

	glutKeyboardFunc(KeyDownCallback);
	glutKeyboardUpFunc(KeyUpCallback);

	glutSpecialFunc(SpecialKeyDownCallback);
	glutSpecialUpFunc(SpecialKeyUpCallback);

	glutMouseFunc(MouseKeyCallback);
	glutMotionFunc(MouseMotionCallback);

	glutMainLoop();
}