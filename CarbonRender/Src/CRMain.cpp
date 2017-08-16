#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRObject.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRWindowManager.h"

MeshObject type59;

void MainDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();

	type59.Render();

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
	FbxImportManager::Instance();
	ShaderManager::Instance();
	CameraManager::Instance();
	WindowManager::Instance();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	WindowManager::Instance()->CreateWindow(1920, 1080, "CarbonRender");

	if (glewInit())
		std::cout << "GLEW init fail" << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//test code
	Camera cam;
	cam.SetPerspectiveCamera(90.0f, 0.01f, 1000.0f);
	cam.SetPosition(float3(0.0f, 2.0f, 7.0f));
	CameraManager::Instance()->Push(cam);

	FbxImportManager::Instance()->ImportFbxModel("Resources\\Models\\Type59.fbx", &type59);
	type59.GetReady4Rending();
	type59.AttachShader(ShaderManager::Instance()->LoadShader("Test.vert", "Test.frag"));
	type59.SetRotation(float3(0.0f, 45.0f, 0.0f));
	delete ShaderManager::Instance();
	//test code
}

void main(int argc, char** argv)
{
	Init(argc, argv);
	
	glutDisplayFunc(MainDisplay);
	glutIdleFunc(MainDisplay);
	glutReshapeFunc(ReSizeCallback);
	glutMainLoop();
}