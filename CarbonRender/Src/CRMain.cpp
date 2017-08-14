#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRObject.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRCameraManager.h"

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
	glViewport(0, 0, w, h);
}


void Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);

	FbxImportManager::Instance();
	ShaderManager::Instance();
	CameraManager::Instance();

	//test code
	Camera cam;
	cam.SetPerspectiveCamera(90.0f, 0.01f, 1000.0f);
	cam.SetPosition(float3(0.0f, 0.0f, 10.0f));
	CameraManager::Instance()->Push(cam);

	FbxImportManager::Instance()->ImportFbxModel("Resources\\Models\\box.fbx", &type59);
	type59.GetReady4Rending();
	type59.AttachShader(ShaderManager::Instance()->LoadShader("Test.vert", "Test.frag"));
	type59.SetRotation(float3(0.0f, 0.0f, 0.0f));
	delete ShaderManager::Instance();
	//test code
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("CarbonRender");

	if (glewInit())
	{
		std::cout << "GLEW init fail" << std::endl;
	}

	Init();
	
	glutDisplayFunc(MainDisplay);
	glutIdleFunc(MainDisplay);
	glutReshapeFunc(ReSizeCallback);
	glutMainLoop();
}