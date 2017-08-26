#ifndef CR_SCENEMANAGER
#define CR_SCENEMANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRControllerManager.h"

class SceneManager
{
private:
	SceneManager();
	static SceneManager* ins;

	MeshObject scene;
	MeshObject type59;

public:
	~SceneManager();
	static SceneManager* Instance();
	void Init();
	void LoadScene();
	void DrawScene(GLuint shaderProgram);
};

#endif