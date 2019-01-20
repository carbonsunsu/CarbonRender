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
	sceneRoot = new Object();
	sceneRoot->SetName("Root");
}

void SceneManager::LoadScene(char* sceneName)
{
	char* realDir = "Resources\\Scene\\";
	realDir = FileReader::BindString(realDir, sceneName);
	realDir = FileReader::BindString(realDir, ".scene");

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
		Object* meshObj = new Object();
		
		FbxImportManager::Instance()->ImportFbxModel(l3Node->first_attribute()->value(), meshObj);
		l4Node = l3Node->first_node("Position");
		attri = l4Node->first_attribute();
		for (int i = 0; i < 3; i++)
		{
			a[i] = atof(attri->value());
			attri = attri->next_attribute();
		}
		meshObj->SetPosition(float3(a[0], a[1], a[2]));
		l4Node = l3Node->first_node("Rotation");
		attri = l4Node->first_attribute();
		for (int i = 0; i < 3; i++)
		{
			a[i] = atof(attri->value());
			attri = attri->next_attribute();
		}
		meshObj->SetRotation(float3(a[0], a[1], a[2]));
		l4Node = l3Node->first_node("Scale");
		attri = l4Node->first_attribute();
		for (int i = 0; i < 3; i++)
		{
			a[i] = atof(attri->value());
			attri = attri->next_attribute();
		}
		meshObj->SetScale(float3(a[0], a[1], a[2]));
		sceneRoot->AddChild(meshObj);
		l3Node = l3Node->next_sibling();
	}

	SaveScene("Test");
}

void SceneManager::AddObj2XMLNode(xml_document<>* sceneDoc, xml_node<>* parent, Object * obj)
{
	if (obj == nullptr)
		return;

	while (true)
	{
		xml_node<>* l1Node;
		xml_attribute<>* attrib;
		switch (obj->GetType())
		{
		case ObjectType::eMesh:
		{
			MeshObject* meshObj = (MeshObject*)obj;
			l1Node = sceneDoc->allocate_node(node_element, sceneDoc->allocate_string("StaticMeshObject"));
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Name"), sceneDoc->allocate_string(meshObj->GetName().c_str()));
			l1Node->append_attribute(attrib);
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Path"), sceneDoc->allocate_string(meshObj->GetPath().c_str()));
			l1Node->append_attribute(attrib);
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("SubMesh"), sceneDoc->allocate_string(meshObj->GetSubMeshName().c_str()));
			l1Node->append_attribute(attrib);
			parent->append_node(l1Node);

			AddObj2XMLNode(sceneDoc, l1Node, obj->GetFirstChild()); 
		}
			break;
		case ObjectType::eNull:
		{
			l1Node = sceneDoc->allocate_node(node_element, sceneDoc->allocate_string("Object"));
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Name"), sceneDoc->allocate_string(obj->GetName().c_str()));
			l1Node->append_attribute(attrib);
			parent->append_node(l1Node);

			AddObj2XMLNode(sceneDoc, l1Node, obj->GetFirstChild());
		}
			break;
		default:
			break;
		}

		obj = obj->GetNext();
		if (obj == nullptr)
			break;
	}
}

void SceneManager::SaveScene(char * sceneName)
{
	char* realDir = "Resources\\Scene\\";
	realDir = FileReader::BindString(realDir, sceneName);
	realDir = FileReader::BindString(realDir, ".scene");

	xml_document<> sceneDoc;

	xml_node<>* rootNode = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Scene"), nullptr);
	sceneDoc.append_node(rootNode);

	//World info
	xml_node<>* l0Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("World"), nullptr);
	rootNode->append_node(l0Node);

	xml_node<>* l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Latitude"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetLatitude()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Day"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetDay()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Time"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetHour()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Turbidity"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetTurbidity()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Exposure"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetExposure()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("TimeSpeed"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetTimeSpeed()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("WindDirection"));
	float3 windDir = WeatherSystem::Instance()->GetWindDirection();
	xml_attribute<>* attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("x"),
		sceneDoc.allocate_string(to_string(windDir.x).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("y"),
		sceneDoc.allocate_string(to_string(windDir.y).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("z"),
		sceneDoc.allocate_string(to_string(windDir.z).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("w"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetWindStrength()).c_str()));
	l1Node->append_attribute(attrib);
	l0Node->append_node(l1Node);

	//Camera info
	l0Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Camera"), nullptr);
	rootNode->append_node(l0Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Position"));
	float3 camPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("x"),
		sceneDoc.allocate_string(to_string(camPos.x).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("y"),
		sceneDoc.allocate_string(to_string(camPos.y).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("z"),
		sceneDoc.allocate_string(to_string(camPos.z).c_str()));
	l1Node->append_attribute(attrib);
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Rotation"));
	float3 camRot = CameraManager::Instance()->GetCurrentCamera()->GetRotation();
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("x"),
		sceneDoc.allocate_string(to_string(camRot.x).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("y"),
		sceneDoc.allocate_string(to_string(camRot.y).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("z"),
		sceneDoc.allocate_string(to_string(camRot.z).c_str()));
	l1Node->append_attribute(attrib);
	l0Node->append_node(l1Node);

	float3 camPara = CameraManager::Instance()->GetCurrentCamera()->GetCameraPara();
	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("FOV"),
		sceneDoc.allocate_string(to_string(camPara.x).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("NearClip"),
		sceneDoc.allocate_string(to_string(camPara.y).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("FarClip"),
		sceneDoc.allocate_string(to_string(camPara.z).c_str()));
	l0Node->append_node(l1Node);

	//Objs
	l0Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Objects"), nullptr);
	rootNode->append_node(l0Node);

	AddObj2XMLNode(&sceneDoc, l0Node, sceneRoot->GetFirstChild());

	TextFile sceneData = FileReader::XML2Text(rootNode);
	FileReader::WriteTextFile(realDir, sceneData);
}

void SceneManager::Draw(Object * node, GLuint shaderProgram)
{
	//draw node if the type is Mesh
	if (node->GetType() == ObjectType::eMesh)
	{
		MeshObject* meshObj = (MeshObject*)node;
		meshObj->Render(shaderProgram);
	}

	//draw children
	Object* child = node->GetFirstChild();
	if (child == nullptr)
		return;

	while (true)
	{
		Draw(child, shaderProgram);

		if (child->GetNext() == nullptr)
			break;

		child = child->GetNext();
	}
}

void SceneManager::DrawScene(GLuint shaderProgram)
{
	Draw(sceneRoot, shaderProgram);
}

Object * SceneManager::GetRootNode()
{
	return sceneRoot;
}
