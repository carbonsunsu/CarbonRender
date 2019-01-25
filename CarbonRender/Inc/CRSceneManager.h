#ifndef CR_SCENEMANAGER
#define CR_SCENEMANAGER

#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRControllerManager.h"

class SceneManager
{
private:
	SceneManager();
	static SceneManager* ins;
	
	Object* sceneRoot;

	void Init();
public:
	~SceneManager();
	static SceneManager* Instance();
	
	void LoadScene(char* sceneName);
	void WriteObj2XMLNode(xml_document<>* sceneDoc, xml_node<>* parent, Object* obj);
	void ReadObjFromXMLNode(xml_node<>* xmlNode, Object* sceneNodeParent);
	void SaveScene(char* sceneName);
	void Draw (Object* node, GLuint shaderProgram);
	void DrawScene(GLuint shaderProgram);

	Object* GetRootNode();
};

#endif