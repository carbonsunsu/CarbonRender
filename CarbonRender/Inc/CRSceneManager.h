#ifndef CR_SCENEMANAGER
#define CR_SCENEMANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRControllerManager.h"
#include "..\Inc\CRFileReader.h"

class SceneManager
{
private:
	SceneManager();
	static SceneManager* ins;

	MeshObject terrain;
	MeshObject sponza;
	MeshObject tank;
	MeshObject dragon;
	MeshObject recon;

	MeshObject bunny[10];
public:
	~SceneManager();
	static SceneManager* Instance();
	void Init();
	void LoadScene();
	void DrawScene(GLuint shaderProgram);
};

#endif