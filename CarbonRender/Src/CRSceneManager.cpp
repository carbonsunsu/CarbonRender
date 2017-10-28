#include "..\Inc\CRSceneManager.h"

SceneManager* SceneManager::ins = nullptr;

SceneManager::SceneManager()
{
	Init();
}

SceneManager::~SceneManager()
{
}

SceneManager * SceneManager::Instance()
{
	if (ins == nullptr)
		ins = new SceneManager();

	return ins;
}

void SceneManager::Init()
{
}

void SceneManager::LoadScene(char* sceneName)
{
	char* realDir = "Resources\\Scene\\";
	realDir = FileReader::BindString(realDir, sceneName);
	realDir = FileReader::BindString(realDir, ".xml");

	TextFile sceneFile = FileReader::ReadTextFile(realDir);
	xml_document<> sceneData;
	sceneData.parse<0>(sceneFile.data);

	xml_node<>* root = sceneData.first_node();
	xml_node<>* l1Node = root->first_node();
	xml_node<>* l2Node = l1Node->first_node("Latitude");
	xml_attribute<>* attri;
	WeatherSystem::Instance()->SetLatitude(atof(l2Node->value()));
	l2Node = l1Node->first_node("Day");
	WeatherSystem::Instance()->SetDay(atoi(l2Node->value()));
	l2Node = l1Node->first_node("Time");
	WeatherSystem::Instance()->SetHour(atof(l2Node->value()));
	l2Node = l1Node->first_node("Turbidity");
	WeatherSystem::Instance()->SetTurbidity(atof(l2Node->value()));
	l2Node = l1Node->first_node("Exposure");
	WeatherSystem::Instance()->SetExposure(atof(l2Node->value()));
	l2Node = l1Node->first_node("TimeSpeed");
	WeatherSystem::Instance()->SetTimeSpeed(atof(l2Node->value()));
	l2Node = l1Node->first_node("WindDirection");
	attri = l2Node->first_attribute();
	float a[3];
	for (int i = 0; i < 3; i++)
	{
		a[i] = atof(attri->value());
		attri = attri->next_attribute();
	}
	WeatherSystem::Instance()->SetWindDirection(float3(a[0], a[1], a[2]));
	WeatherSystem::Instance()->SetWindStrength(atof(attri->value()));

	Camera cam;
	l1Node = root->first_node("Camera");
	l2Node = l1Node->first_node("Position");
	attri = l2Node->first_attribute();
	for (int i = 0; i < 3; i++)
	{
		a[i] = atof(attri->value());
		attri = attri->next_attribute();
	}
	cam.SetPosition(float3(a[0], a[1], a[2]));
	l2Node = l1Node->first_node("Rotation");
	attri = l2Node->first_attribute();
	for (int i = 0; i < 3; i++)
	{
		a[i] = atof(attri->value());
		attri = attri->next_attribute();
	}
	cam.SetRotation(float3(a[0], a[1], a[2]));
	l2Node = l1Node->first_node("FOV");
	a[0] = atof(l2Node->value());
	l2Node = l1Node->first_node("NearClip");
	a[1] = atof(l2Node->value());
	l2Node = l1Node->first_node("FarClip");
	a[2] = atof(l2Node->value());
	cam.SetPerspectiveCamera(a[0], a[1], a[2]);
	CameraManager::Instance()->Push(cam);

	Controller ctrl;
	ctrl.Init();
	ControllerManager::Instance()->Push(ctrl);

	l1Node = root->first_node("Objects");
	l2Node = l1Node->first_node("StaticMeshObjects");
	int count = atoi(l2Node->first_attribute()->value());
	xml_node<>* l3Node = l2Node->first_node();
	xml_node<>* l4Node;
	for (int i = 0; i < count; i++)
	{
		MeshObject staticMeshObject;
		
		FbxImportManager::Instance()->ImportFbxModel(l3Node->first_attribute()->value(), &staticMeshObject);
		staticMeshObject.GetReady4Rending();
		l4Node = l3Node->first_node("Position");
		attri = l4Node->first_attribute();
		for (int i = 0; i < 3; i++)
		{
			a[i] = atof(attri->value());
			attri = attri->next_attribute();
		}
		staticMeshObject.SetPosition(float3(a[0], a[1], a[2]));
		l4Node = l3Node->first_node("Rotation");
		attri = l4Node->first_attribute();
		for (int i = 0; i < 3; i++)
		{
			a[i] = atof(attri->value());
			attri = attri->next_attribute();
		}
		staticMeshObject.SetRotation(float3(a[0], a[1], a[2]));
		l4Node = l3Node->first_node("Scale");
		attri = l4Node->first_attribute();
		for (int i = 0; i < 3; i++)
		{
			a[i] = atof(attri->value());
			attri = attri->next_attribute();
		}
		staticMeshObject.SetScale(float3(a[0], a[1], a[2]));
		staticMeshObjects.emplace_back(staticMeshObject);
		l3Node = l3Node->next_sibling();
	}
}

void SceneManager::DrawScene(GLuint shaderProgram)
{
	for (int i = 0; i < staticMeshObjects.size(); i++)
	{
		staticMeshObjects[i].Render(shaderProgram);
	}
}
