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

#include "..\Inc\CRSkyRenderPass.h"
#include "..\Inc\CRFinalPass.h"
#include "..\Inc\CRGPass.h"
#include "..\Inc\CRLightPass.h"

SkyRenderPass skyPass;
FinalPass finalPass;
GPass gPass;
LightPass lightPass;

void MainDisplay()
{
	PassOutput* sky = skyPass.Draw(NULL);
	PassOutput* g = gPass.Draw(NULL);
	PassOutput* light = lightPass.Draw(g);
	PassOutput newOutput;
	newOutput.cout = sky->cout + light->cout;
	newOutput.RTS = new GLuint[newOutput.cout];
	newOutput.RTS[0] = sky->RTS[0];
	newOutput.RTS[1] = light->RTS[0];
	finalPass.Draw(&newOutput);

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//test code
	Camera cam;
	cam.SetPerspectiveCamera(60.0f, 0.01f, 10000.0f);
	cam.SetPosition(float3(0.0f, 1.0f, 5.0f));
	CameraManager::Instance()->Push(cam);

	Controller ctrl;
	ctrl.Init();
	ControllerManager::Instance()->Push(ctrl);

	skyPass.Init();
	finalPass.Init();
	gPass.Init();
	lightPass.Init();
	//test code
}

void FixedUpdate(int value)
{
	WeatherSystem::Instance()->Update();
	ControllerManager::Instance()->GetCurrentController()->Update();

	glutPostRedisplay();

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
	glutMouseFunc(MouseKeyCallback);
	glutMotionFunc(MouseMotionCallback);
	glutMainLoop();
}