#ifndef CR_SCENEMANAGER
#define CR_SCENEMANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRControllerManager.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRWeatherSystem.h"

class SceneManager
{
private:
	SceneManager();
	static SceneManager* ins;

	vector<MeshObject> staticMeshObjects;

	void Init();
public:
	~SceneManager();
	static SceneManager* Instance();
	
	void LoadScene(char* sceneName);
	void DrawScene(GLuint shaderProgram);
};

#endif