#include "..\Inc\CRSceneManager.h"

SceneManager* SceneManager::ins = nullptr;

SceneManager::SceneManager()
{
	Init();
}

SceneManager::~SceneManager()
{
	ins = nullptr;
}

SceneManager * SceneManager::Instance()
{
	if (ins == nullptr)
		ins = new SceneManager();

	return ins;
}

void SceneManager::Init()
{
	curSceneName = "New Scene";
	sceneRoot.SetName("Root");
}

void SceneManager::DeleteObject(Object * obj)
{
	if (obj->GetParent() != nullptr)
		obj->GetParent()->SetChildCount(obj->GetParent()->GetChildCount() - 1);

	if (obj->GetType() == ObjectType::eMesh)
	{
		MeshObject* meshObj = (MeshObject*)obj;
		delete meshObj;
	}
	else
	{
		delete obj;
	}
}

void SceneManager::LoadScene(string sceneName)
{
	char* realDir = "Resources\\Scene\\";
	realDir = FileReader::BindString(realDir, (char*)sceneName.c_str());
	realDir = FileReader::BindString(realDir, ".scene");

	TextFile sceneFile = FileReader::ReadTextFile(realDir);
	xml_document<> sceneData;
	sceneData.parse<0>(sceneFile.data);

	//World info
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

	l2Node = l1Node->first_node("WeatherMap");
	WeatherSystem::Instance()->SetWeatherMap(l2Node->value());

	l2Node = l1Node->first_node("CloudMaxAltitude");
	WeatherSystem::Instance()->SetCloudMaxAltitude(atof(l2Node->value()));

	l2Node = l1Node->first_node("CloudMinAltitude");
	WeatherSystem::Instance()->SetCloudMinAltitude(atof(l2Node->value()));

	l2Node = l1Node->first_node("CloudCoverage");
	WeatherSystem::Instance()->SetCloudCoverage(atof(l2Node->value()));

	l2Node = l1Node->first_node("CloudPrecipitation");
	WeatherSystem::Instance()->SetCloudPrecipitation(atof(l2Node->value()));

	l2Node = l1Node->first_node("FogDensity");
	WeatherSystem::Instance()->SetFogDensity(atof(l2Node->value()));

	l2Node = l1Node->first_node("FogColor");
	attri = l2Node->first_attribute();
	for (int i = 0; i < 3; i++)
	{
		a[i] = atof(attri->value());
		attri = attri->next_attribute();
	}
	WeatherSystem::Instance()->SetFogColor(float3(a[0], a[1], a[2]));

	l2Node = l1Node->first_node("FogMaxAltitude");
	WeatherSystem::Instance()->SetFogMaxAltitude(atof(l2Node->value()));

	l2Node = l1Node->first_node("FogPrecipitation");
	WeatherSystem::Instance()->SetFogPrecipitation(atof(l2Node->value()));

	//Camera info
	Camera* cam = CameraManager::Instance()->Push();
	l1Node = root->first_node("Camera");
	l2Node = l1Node->first_node("Position");
	attri = l2Node->first_attribute();
	for (int i = 0; i < 3; i++)
	{
		a[i] = atof(attri->value());
		attri = attri->next_attribute();
	}
	cam->SetPosition(float3(a[0], a[1], a[2]));

	l2Node = l1Node->first_node("Rotation");
	attri = l2Node->first_attribute();
	for (int i = 0; i < 3; i++)
	{
		a[i] = atof(attri->value());
		attri = attri->next_attribute();
	}
	cam->SetRotation(float3(a[0], a[1], a[2]));

	l2Node = l1Node->first_node("FOV");
	a[0] = atof(l2Node->value());

	l2Node = l1Node->first_node("NearClip");
	a[1] = atof(l2Node->value());

	l2Node = l1Node->first_node("FarClip");
	a[2] = atof(l2Node->value());
	cam->SetPerspectiveCamera(a[0], a[1], a[2]);

	//Materials
	l1Node = root->first_node("Materials");
	l2Node = l1Node->first_node("Material");
	while (l2Node != nullptr)
	{
		unsigned int matID = (unsigned int)atoi(l2Node->first_attribute("ID")->value());
		string matName = l2Node->first_attribute("Name")->value();
		Material* newMat = MaterialManager::Instance()->CreateNewMaterial(matID, matName);

		newMat->SetMetallic(atof(l2Node->first_node("Metallic")->value()));
		newMat->SetRoughness(atof(l2Node->first_node("Roughness")->value()));
		xml_node<>* colorNode = l2Node->first_node("Color");
		newMat->SetColor(float4(atof(colorNode->first_attribute("R")->value()),
			atof(colorNode->first_attribute("G")->value()),
			atof(colorNode->first_attribute("B")->value()),
			atof(colorNode->first_attribute("A")->value())
		));
		xml_node<>* l3Node = l2Node->first_node("Diffuse");
		newMat->SetDiffuse(l3Node->value());
		newMat->SetDiffuseTilling(atof(l3Node->first_attribute("tillingX")->value()), atof(l3Node->first_attribute("tillingY")->value()));

		l3Node = l2Node->first_node("Normal");
		newMat->SetNormal(l3Node->value());
		newMat->SetNormalTilling(atof(l3Node->first_attribute("tillingX")->value()), atof(l3Node->first_attribute("tillingY")->value()));

		l3Node = l2Node->first_node("Specular");
		newMat->SetSpecular(l3Node->value());
		newMat->SetSpecularTilling(atof(l3Node->first_attribute("tillingX")->value()), atof(l3Node->first_attribute("tillingY")->value()));

		l2Node = l2Node->next_sibling();
	}

	//Objs
	l1Node = root->first_node("Objects");
	l2Node = l1Node->first_node();

	if (l2Node != nullptr)
		ReadObjFromXMLNode(l2Node, &sceneRoot);
}

void SceneManager::LoadDefaultScene()
{
	//World info
	WeatherSystem::Instance()->SetLatitude(0.0f);
	WeatherSystem::Instance()->SetDay(180);
	WeatherSystem::Instance()->SetHour(15.0f);
	WeatherSystem::Instance()->SetTurbidity(4.0f);
	WeatherSystem::Instance()->SetExposure(30.0f);
	WeatherSystem::Instance()->SetTimeSpeed(1500.0f);
	WeatherSystem::Instance()->SetWindDirection(float3(0.5f, 0.0f, 1.0f));
	WeatherSystem::Instance()->SetWindStrength(10.0f);
	WeatherSystem::Instance()->SetWeatherMap("Weather0");
	WeatherSystem::Instance()->SetCloudMaxAltitude(6000.0f);
	WeatherSystem::Instance()->SetCloudMinAltitude(1500.0f);
	WeatherSystem::Instance()->SetCloudCoverage(0.675f);
	WeatherSystem::Instance()->SetCloudPrecipitation(1.0f);
	WeatherSystem::Instance()->SetFogDensity(0.01f);
	WeatherSystem::Instance()->SetFogColor(float3(1.0f, 1.0f, 1.0f));
	WeatherSystem::Instance()->SetFogMaxAltitude(20.0f);
	WeatherSystem::Instance()->SetFogPrecipitation(14.0f);

	//Camera info
	Camera* cam = CameraManager::Instance()->Push();
	cam->SetPosition(float3(0.0f, 1.0f, 3.0f));
	cam->SetRotation(float3(0.0f, 0.0f, 0.0f));
	cam->SetPerspectiveCamera(60.0f, 0.01f, 3000.0f);
}

void SceneManager::WriteObj2XMLNode(xml_document<>* sceneDoc, xml_node<>* parent, Object * obj)
{
	if (obj == nullptr)
		return;

	while (true)
	{
		xml_node<>* l1Node = sceneDoc->allocate_node(node_element);
		xml_attribute<>* attrib;
		switch (obj->GetType())
		{
		case ObjectType::eMesh:
		{
			MeshObject* meshObj = (MeshObject*)obj;
			l1Node->name(sceneDoc->allocate_string("StaticMeshObject"));
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Name"), sceneDoc->allocate_string(meshObj->GetName().c_str()));
			l1Node->append_attribute(attrib);
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Path"), sceneDoc->allocate_string(meshObj->GetMeshData()->GetPath().c_str()));
			l1Node->append_attribute(attrib);
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("SubMesh"), sceneDoc->allocate_string(meshObj->GetMeshData()->GetSubMeshName().c_str()));
			l1Node->append_attribute(attrib);
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Visible"), sceneDoc->allocate_string(meshObj->IsVisible() ? "1" : "0"));
			l1Node->append_attribute(attrib);
			parent->append_node(l1Node);

			//write material 
			xml_node<>* l2Node = sceneDoc->allocate_node(node_element, sceneDoc->allocate_string("MaterialID"), sceneDoc->allocate_string(to_string(meshObj->GetMaterial()->GetID()).c_str()));
			l1Node->append_node(l2Node);
		}
			break;
		case ObjectType::eNull:
		{
			l1Node->name(sceneDoc->allocate_string("Object"));
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Name"), sceneDoc->allocate_string(obj->GetName().c_str()));
			l1Node->append_attribute(attrib);
			attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("Visible"), sceneDoc->allocate_string(obj->IsVisible() ? "1" : "0"));
			l1Node->append_attribute(attrib);
			parent->append_node(l1Node);
		}
			break;
		default:
			break;
		}

		//write position
		float3 info = obj->GetPosition();
		xml_node<>* l2Node = sceneDoc->allocate_node(node_element, sceneDoc->allocate_string("Position"));
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("x"), sceneDoc->allocate_string(to_string(info.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("y"), sceneDoc->allocate_string(to_string(info.y).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("z"), sceneDoc->allocate_string(to_string(info.z).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		//write rotation
		info = obj->GetRotation();
		l2Node = sceneDoc->allocate_node(node_element, sceneDoc->allocate_string("Rotation"));
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("x"), sceneDoc->allocate_string(to_string(info.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("y"), sceneDoc->allocate_string(to_string(info.y).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("z"), sceneDoc->allocate_string(to_string(info.z).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		//write scale
		info = obj->GetScale();
		l2Node = sceneDoc->allocate_node(node_element, sceneDoc->allocate_string("Scale"));
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("x"), sceneDoc->allocate_string(to_string(info.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("y"), sceneDoc->allocate_string(to_string(info.y).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc->allocate_attribute(sceneDoc->allocate_string("z"), sceneDoc->allocate_string(to_string(info.z).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		//write child objects
		if (obj->GetFirstChild() != nullptr)
		{
			l2Node = sceneDoc->allocate_node(node_element, sceneDoc->allocate_string("Children"));
			l1Node->append_node(l2Node);
			WriteObj2XMLNode(sceneDoc, l2Node, obj->GetFirstChild());
		}

		obj = obj->GetNext();
		if (obj == nullptr)
			break;
	}
}

void SceneManager::ReadObjFromXMLNode(xml_node<>* xmlNode, Object * sceneNodeParent)
{
	if (strcmp(xmlNode->name(), "Object") == 0)
	{
		Object* newObj = new Object();
		sceneNodeParent->AddChild(newObj);
		newObj->SetVisible((int)atof(xmlNode->first_attribute("Visible")->value()));
	}
	else if (strcmp(xmlNode->name(), "StaticMeshObject") == 0)
	{
		MeshObject* newMeshObj = new MeshObject();
		sceneNodeParent->AddChild(newMeshObj);

		MeshData* data = FbxImportManager::Instance()->ImportFbxModel(xmlNode->first_attribute("Path")->value(), xmlNode->first_attribute("SubMesh")->value());
		newMeshObj->SetMeshData(data);
		newMeshObj->SetVisible((bool)atoi(xmlNode->first_attribute("Visible")->value()));

		unsigned int matID = (unsigned int)atoi(xmlNode->first_node("MaterialID")->value());
		newMeshObj->SetMaterial(matID);
	}

	//read transform info
	Object* newObj = sceneNodeParent->GetLastChild();

	newObj->SetName(xmlNode->first_attribute("Name")->value());

	float3 transInfo;
	xml_node<>* l0Node = xmlNode->first_node("Position");
	transInfo.x = atof(l0Node->first_attribute("x")->value());
	transInfo.y = atof(l0Node->first_attribute("y")->value());
	transInfo.z = atof(l0Node->first_attribute("z")->value());
	newObj->SetPosition(transInfo);

	l0Node = xmlNode->first_node("Rotation");
	transInfo.x = atof(l0Node->first_attribute("x")->value());
	transInfo.y = atof(l0Node->first_attribute("y")->value());
	transInfo.z = atof(l0Node->first_attribute("z")->value());
	newObj->SetRotation(transInfo);

	l0Node = xmlNode->first_node("Scale");
	transInfo.x = atof(l0Node->first_attribute("x")->value());
	transInfo.y = atof(l0Node->first_attribute("y")->value());
	transInfo.z = atof(l0Node->first_attribute("z")->value());
	newObj->SetScale(transInfo);

	//read child
	xml_node<>* childRoot = xmlNode->first_node("Children");
	if (childRoot != nullptr)
	{
		xml_node<>* child = childRoot->first_node();
		ReadObjFromXMLNode(child, newObj);
	}

	//read next
	if (xmlNode->next_sibling() != nullptr)
		ReadObjFromXMLNode(xmlNode->next_sibling(), sceneNodeParent);
}

void SceneManager::SaveScene(string sceneName)
{
	char* realDir = "Resources\\Scene\\";
	realDir = FileReader::BindString(realDir, (char*)sceneName.c_str());
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
	
	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("WeatherMap"),
		sceneDoc.allocate_string(WeatherSystem::Instance()->GetWeatherMapPath().c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("CloudMaxAltitude"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetCloudMaxAltitude()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("CloudMinAltitude"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetCloudMinAltitude()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("CloudCoverage"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetCloudCoverage()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("CloudPrecipitation"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetCloudPrecipitation()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("FogDensity"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetFogDensity()).c_str()));
	l0Node->append_node(l1Node);

	float3 fogColor = WeatherSystem::Instance()->GetFogColor();
	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("FogColor"));
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("r"),
		sceneDoc.allocate_string(to_string(fogColor.x).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("g"),
		sceneDoc.allocate_string(to_string(fogColor.y).c_str()));
	l1Node->append_attribute(attrib);
	attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("b"),
		sceneDoc.allocate_string(to_string(fogColor.z).c_str()));
	l1Node->append_attribute(attrib);
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("FogMaxAltitude"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetFogMaxAltitude()).c_str()));
	l0Node->append_node(l1Node);

	l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("FogPrecipitation"),
		sceneDoc.allocate_string(to_string(WeatherSystem::Instance()->GetFogPrecipitation()).c_str()));
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

	//Materials
	l0Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Materials"), nullptr);
	rootNode->append_node(l0Node);
	for (unordered_map<unsigned int, Material*>::iterator i = MaterialManager::Instance()->materials.begin(); i != MaterialManager::Instance()->materials.end(); i++)
	{
		l1Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Material"), nullptr);
		l0Node->append_node(l1Node);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("ID"), sceneDoc.allocate_string(to_string(i->second->GetID()).c_str()));
		l1Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("Name"), sceneDoc.allocate_string(i->second->GetName().c_str()));
		l1Node->append_attribute(attrib);

		xml_node<>* l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Metallic"), sceneDoc.allocate_string(to_string(i->second->GetMetallic()).c_str()));
		l1Node->append_node(l2Node);
		l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Roughness"), sceneDoc.allocate_string(to_string(i->second->GetRoughness()).c_str()));
		l1Node->append_node(l2Node);

		l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Color"));
		float4 color = i->second->GetColor();
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("R"), sceneDoc.allocate_string(to_string(color.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("G"), sceneDoc.allocate_string(to_string(color.y).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("B"), sceneDoc.allocate_string(to_string(color.z).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("A"), sceneDoc.allocate_string(to_string(color.w).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Diffuse"), sceneDoc.allocate_string(i->second->GetDiffusePath().c_str()));
		float3 diffTilling = i->second->GetDiffuseTilling();
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("tillingX"), sceneDoc.allocate_string(to_string(diffTilling.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("tillingY"), sceneDoc.allocate_string(to_string(diffTilling.y).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Normal"), sceneDoc.allocate_string(i->second->GetNormalPath().c_str()));
		float3 norlTilling = i->second->GetNormalTilling();
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("tillingX"), sceneDoc.allocate_string(to_string(norlTilling.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("tillingY"), sceneDoc.allocate_string(to_string(norlTilling.y).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Specular"), sceneDoc.allocate_string(i->second->GetSpecularPath().c_str()));
		float3 specTilling = i->second->GetSpecularTilling();
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("tillingX"), sceneDoc.allocate_string(to_string(specTilling.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("tillingY"), sceneDoc.allocate_string(to_string(specTilling.y).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("EmissionColor"));
		float3 emisColor = i->second->GetEmissionColor();
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("R"), sceneDoc.allocate_string(to_string(emisColor.x).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("G"), sceneDoc.allocate_string(to_string(emisColor.y).c_str()));
		l2Node->append_attribute(attrib);
		attrib = sceneDoc.allocate_attribute(sceneDoc.allocate_string("B"), sceneDoc.allocate_string(to_string(emisColor.z).c_str()));
		l2Node->append_attribute(attrib);
		l1Node->append_node(l2Node);

		l2Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("EmissionIntensity"), sceneDoc.allocate_string(to_string(i->second->GetEmissionIntensity()).c_str()));
		l1Node->append_node(l2Node);
	}

	//Objs
	l0Node = sceneDoc.allocate_node(node_element, sceneDoc.allocate_string("Objects"), nullptr);
	rootNode->append_node(l0Node);

	WriteObj2XMLNode(&sceneDoc, l0Node, sceneRoot.GetFirstChild());

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
	Draw(&sceneRoot, shaderProgram);
	if (TerrainManager::Instance()->UseTerrain())
		TerrainManager::Instance()->GetTerrainObject()->Render(shaderProgram);

	if (TerrainManager::Instance()->UseOcean())
		TerrainManager::Instance()->GetOceanObject()->Render(shaderProgram);
}

Object * SceneManager::GetRootNode()
{
	return &sceneRoot;
}

void SceneManager::DeleteSceneNode(Object * obj)
{
	if (obj == nullptr)return;

	Object* root = obj;
	if (obj == obj->GetParent()->GetFirstChild())
	{
		obj->GetParent()->SetFirstChild(obj->GetNext());
	}
	if (obj->GetPrevious() != nullptr)
	{
		obj->GetPrevious()->SetNext(obj->GetNext());
	}
	if (obj->GetNext() != nullptr)
	{
		obj->GetNext()->SetPrevious(obj->GetPrevious());
	}

	if (root->GetChildCount() > 0)
	{
		Object* curPtr = root->GetFirstChild();
		while (true)
		{
			if (curPtr == root)
			{
				DeleteObject(curPtr);
				break;
			}

			if (curPtr->GetChildCount() > 0)
			{
				curPtr = curPtr->GetFirstChild();
				continue;
			}
			else
			{
				Object* temp = curPtr;
				if (curPtr->GetNext() != nullptr)
					curPtr = curPtr->GetNext();
				else
					curPtr = curPtr->GetParent();
				DeleteObject(temp);
				continue;
			}
		}
	}
	else
	{
		DeleteObject(root);
	}
}

void SceneManager::ClearCurScene()
{
	Object* child = sceneRoot.GetFirstChild();
	while (child != nullptr)
	{
		DeleteSceneNode(child);
		child = sceneRoot.GetFirstChild();
	}
}

void SceneManager::SetCurSceneName(string name)
{
	curSceneName = name;
}

string SceneManager::GetCurSceneName()
{
	return curSceneName;
}
