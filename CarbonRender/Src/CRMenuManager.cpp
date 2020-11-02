#include "..\Inc\CRMenuManager.h"

MenuManager* MenuManager::ins = nullptr;

MenuManager::MenuManager()
{
}

void MenuManager::InitMenu()
{
	showMenu = true;
	IMGUI_CHECKVERSION();
	guiContext = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::Instance()->GetWindow(), true);
	ImGui_ImplOpenGL3_Init();

	showFileBroser = false;
	showWorldEditorDialog = true;
	showSceneEditorDialog = true;
	showObjectEditorDialog = true;
	sceneTreeNodeIndex = 0;

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 2.0f;
	style.WindowBorderSize = 2.0f;
	style.PopupBorderSize = 2.0f;
}

void MenuManager::DrawMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Scene"))
		{
			DrawMainMenuBar_Scene();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor"))
		{
			DrawMainMenuBar_Editor();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MenuManager::DrawMainMenuBar_Scene()
{
	if (ImGui::MenuItem("Save Scene"))
	{
		SceneManager::Instance()->SaveScene(ConfigManager::Instance()->GetScenePath());
	}
	if (ImGui::MenuItem("Import Model"))
	{
		InitFileBrowser("Resources\\Models", ".fbx", &MenuManager::ImportModel);
	}
}

void MenuManager::DrawMainMenuBar_Editor()
{
	if (ImGui::MenuItem("Scene Editor"))
	{
		showSceneEditorDialog = true;
	}
	if (ImGui::MenuItem("Object Editor"))
	{
		showObjectEditorDialog = true;
	}
	if (ImGui::MenuItem("World Editor"))
	{
		showWorldEditorDialog = true;
	}
}

void MenuManager::DrawWorldEditorDialog()
{
	ImGui::SetNextWindowSize(ImVec2(300, 570), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("World Editor", &showWorldEditorDialog, flags))
	{
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::Text("Time");
		{
			bool useTimeLapse = !WeatherSystem::Instance()->IsTimeStop();
			ImGui::Checkbox("Time Lapse", &useTimeLapse);
			WeatherSystem::Instance()->SetTimeStop(!useTimeLapse);

			float timeSpeed = WeatherSystem::Instance()->GetTimeSpeed();
			ImGui::DragFloat("Time Lapse Speed", &timeSpeed, 1.0f, -FLT_MAX, +FLT_MAX);
			WeatherSystem::Instance()->SetTimeSpeed(timeSpeed);

			int day = WeatherSystem::Instance()->GetDay();
			ImGui::DragInt("Day", &day, 1.0f, 1, 365);
			WeatherSystem::Instance()->SetDay(day);

			float hour = WeatherSystem::Instance()->GetHour();
			ImGui::DragFloat("Hour", &hour, 0.01f, 0.0f, 24.0f);
			WeatherSystem::Instance()->SetHour(hour);
		}
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Atmosphere");
		{
			float latitude = WeatherSystem::Instance()->GetLatitude();
			ImGui::DragFloat("Latitude", &latitude, 0.1f, 0.0f, 90.0f);
			WeatherSystem::Instance()->SetLatitude(latitude);

			float turbidity = WeatherSystem::Instance()->GetTurbidity();
			ImGui::DragFloat("Turbidity", &turbidity, 0.001f, -FLT_MAX, +FLT_MAX);
			WeatherSystem::Instance()->SetTurbidity(turbidity);

			float exposure = WeatherSystem::Instance()->GetExposure();
			ImGui::DragFloat("Exposure", &exposure, 0.001f, -FLT_MAX, +FLT_MAX);
			WeatherSystem::Instance()->SetExposure(exposure);
		}
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Wind");
		{
			float3 dir = WeatherSystem::Instance()->GetWindDirection();
			float windDirc[2] = {dir.x, dir.z};
			ImGui::DragFloat2("Wind Direction", windDirc, 0.001f, 0.0f, 1.0f);
			dir.x = windDirc[0];
			dir.z = windDirc[1];
			WeatherSystem::Instance()->SetWindDirection(dir);

			float windStren = WeatherSystem::Instance()->GetWindStrength();
			ImGui::DragFloat("Wind Stength", &windStren, 0.001f, -FLT_MAX, +FLT_MAX);
			WeatherSystem::Instance()->SetWindStrength(windStren);
		}
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Cloud");
		{
			float cloudMax = WeatherSystem::Instance()->GetCloudMaxAltitude();
			float cloudMin = WeatherSystem::Instance()->GetCloudMinAltitude();
			ImGui::DragFloatRange2("Cloud Altitude", &cloudMin, &cloudMax, 1.0f, -FLT_MAX, +FLT_MAX, "Min: %.0f", "Max: %.0f");
			WeatherSystem::Instance()->SetCloudMaxAltitude(cloudMax);
			WeatherSystem::Instance()->SetCloudMinAltitude(cloudMin);

			float cloudCover = WeatherSystem::Instance()->GetCloudCoverage();
			ImGui::DragFloat("Cloud Coverage", &cloudCover, 0.001f, 0.0f, 1.0f);
			WeatherSystem::Instance()->SetCloudCoverage(cloudCover);

			float cloudPre = WeatherSystem::Instance()->GetCloudPrecipitation();
			ImGui::DragFloat("Cloud Precipitation", &cloudPre, 0.01f, 0.0f, +FLT_MAX);
			WeatherSystem::Instance()->SetCloudPrecipitation(cloudPre);

			float3 cloudBias = WeatherSystem::Instance()->GetCloudBias();
			float bias[2] = { cloudBias.x, cloudBias.z };
			ImGui::DragFloat2("Cloud Move Bias", bias, 1000.0f, -FLT_MAX, +FLT_MAX);
			cloudBias.x = bias[0];
			cloudBias.z = bias[1];
			float3 windDir = WeatherSystem::Instance()->GetWindDirection();
			WeatherSystem::Instance()->SetCloudBias(cloudBias);

			float tempBias = 0.0f;
			if (!ImGui::DragFloat("Drag to Move Cloud", &tempBias, 1.0f, -FLT_MAX, +FLT_MAX))
				tempBias = 0.0f;
			else
				WeatherSystem::Instance()->SetCloudBias(WeatherSystem::Instance()->GetCloudBias() + WeatherSystem::Instance()->GetWindDirection() * tempBias * 1000.0f);
		}
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Fog");
		{
			float3 fogColor = WeatherSystem::Instance()->GetFogColor();
			float tempColor[3] = { fogColor.x, fogColor.y, fogColor.z };
			ImGui::ColorEdit3("Fog Color", tempColor);
			WeatherSystem::Instance()->SetFogColor(float3(tempColor[0], tempColor[1], tempColor[2]));

			float fogDen = WeatherSystem::Instance()->GetFogDensity();
			ImGui::DragFloat("Fog Density", &fogDen, 0.00001f, 0.0f, 1.0f, "%.5f");
			WeatherSystem::Instance()->SetFogDensity(fogDen);

			float fogPrec = WeatherSystem::Instance()->GetFogPrecipitation();
			ImGui::DragFloat("Fog Precipitation", &fogPrec, 0.1f, 0.0f, +FLT_MAX);
			WeatherSystem::Instance()->SetFogPrecipitation(fogPrec);

			float fogMax = WeatherSystem::Instance()->GetFogMaxAltitude();
			ImGui::DragFloat("Fog Max Altitude", &fogMax, 0.1f, -FLT_MAX, +FLT_MAX);
			WeatherSystem::Instance()->SetFogMaxAltitude(fogMax);
		}
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PopItemWidth();
	}
	ImGui::End();
}

void MenuManager::DrawSceneNode(Object * node, ImGuiTreeNodeFlags flags)
{
	if (node == nullptr)
		return;

	ImGuiTreeNodeFlags trueFlags = flags;

	if (node->GetChildCount() == 0)
		trueFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (selectedObj == node)
		trueFlags |= ImGuiTreeNodeFlags_Selected;

	bool isExtend = ImGui::TreeNodeEx((node->GetName() + "##" + to_string(sceneTreeNodeIndex)).c_str(), trueFlags);
	sceneTreeNodeIndex++;

	if (ImGui::IsItemClicked())
	{
		selectedObj = node;
		showObjectEditorDialog = true;
	}

	if (node->GetChildCount() > 0)
	{
		if (isExtend)
		{
			Object* child = node->GetFirstChild();
			DrawSceneNode(child, flags);
			ImGui::TreePop();
		}
	}
	if (node->GetNext() != nullptr)
	{
		DrawSceneNode(node->GetNext(), flags);
	}
}

void MenuManager::DrawSceneEditorDialog()
{
	ImGui::SetNextWindowSize(ImVec2(250, 800), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Scene Editor", &showSceneEditorDialog, windowFlags))
	{
		sceneTreeNodeIndex = 0;

		Object* sceneRoot = SceneManager::Instance()->GetRootNode();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		DrawSceneNode(sceneRoot->GetFirstChild(), flags);
	}
	ImGui::End();
}

void MenuManager::DrawObjectEditorDialog()
{
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Object Editor", &showObjectEditorDialog, windowFlags))
	{
		string objName = "";
		if (selectedObj != nullptr)
			objName = selectedObj->GetName();

		char objNameChar[32];
		strcpy_s(objNameChar, objName.c_str());
		ImGui::InputText("Object Name", objNameChar, IM_ARRAYSIZE(objNameChar));

		if (selectedObj != nullptr)
			selectedObj->SetName(string(objNameChar));

		ImGui::Text("Transform");
		{
			float pos[3] = { 0.0f, 0.0f, 0.0f };
			float rot[3] = { 0.0f, 0.0f, 0.0f };
			float scale[3] = { 0.0f, 0.0f, 0.0f };

			if (selectedObj != nullptr)
			{
				float3 objPos = selectedObj->GetPosition();
				float3 objRot = selectedObj->GetRotation();
				float3 objScale = selectedObj->GetScale();

				pos[0] = objPos.x;
				pos[1] = objPos.y;
				pos[2] = objPos.z;

				rot[0] = objRot.x;
				rot[1] = objRot.y;
				rot[2] = objRot.z;

				scale[0] = objScale.x;
				scale[1] = objScale.y;
				scale[2] = objScale.z;
			}

			ImGui::DragFloat3("Position", pos, 0.01f, -FLT_MAX, +FLT_MAX, "%.6f");
			ImGui::DragFloat3("Rotation", rot, 0.1f, -FLT_MAX, +FLT_MAX, "%.6f");
			ImGui::DragFloat3("Scale", scale, 0.1f, -FLT_MAX, +FLT_MAX, "%.6f");

			if (selectedObj != nullptr)
			{
				selectedObj->SetPosition(float3(pos[0], pos[1], pos[2]));
				selectedObj->SetRotation(float3(rot[0], rot[1], rot[2]));
				selectedObj->SetScale(float3(scale[0], scale[1], scale[2]));
			}
		}

		ImGui::Separator();
		ImGui::Spacing();

		if (selectedObj != nullptr && selectedObj->GetType() == ObjectType::eMesh)
		{
			MeshObject* meshObj = (MeshObject*)selectedObj;

			ImGui::Text("Material");
			{
				Material* mat = meshObj->GetMaterial();
				float4 matColor = mat->GetColor();
				float tempColor[4] = { matColor.x, matColor.y, matColor.z, matColor.w };
				ImGui::ColorEdit4("Color", tempColor);
				mat->SetColor(float4(tempColor[0], tempColor[1], tempColor[2], tempColor[3]));

				ImTextureID texID;
				if (mat->HasDiffuseTexture())
					texID = (ImTextureID)mat->GetDiffuse();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				if (ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f)))
					InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportDiffuse);

				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("Albedo");
				if (ImGui::Button("Clear##D"))
					mat->RemoveDiffuse();
				ImGui::EndGroup();

				if (mat->HasNormalTexture())
					texID = (ImTextureID)mat->GetNormal();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				if (ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f)))
					InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportNormal);

				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("Normal");
				if (ImGui::Button("Clear##N"))
					mat->RemoveNormal();
				ImGui::EndGroup();

				if (mat->HasSpecularTexture())
					texID = (ImTextureID)mat->GetSpecular();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				if (ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f)))
					InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportSpecular);

				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("Specular(r:Roughness g:Metallic)");
				if (ImGui::Button("Clear##S"))
					mat->RemoveSpecular();
				ImGui::EndGroup();

				if (!mat->HasSpecularTexture())
				{
					float roughness = mat->GetRoughness();
					ImGui::DragFloat("Roughness", &roughness, 0.01f, 0.0f, 1.0f, "%.2f");
					mat->SetRoughness(roughness);

					float metallic = mat->GetMetallic();
					ImGui::DragFloat("Metallic", &metallic, 0.01f, 0.0f, 1.0f, "%.2f");
					mat->SetMetallic(metallic);
				}
			}
		}
	}
	ImGui::End();
}

void MenuManager::InitFileBrowser(string path, string suffix, FileImportCallback callback)
{
	selectedFileName = "";
	startPath = path;
	curPath = path;
	fileSuffix = suffix;
	fileImportCallback = callback;
	showFileBroser = true;
}

void MenuManager::DrawFileBrowser()
{
	ImGui::SetNextWindowSize(ImVec2(600, 450), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Files", &showFileBroser, flags))
	{
		ImGui::Text(curPath.c_str());
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::BeginChild("filelist", ImVec2(ImGui::GetWindowSize().x - 20, ImGui::GetWindowSize().y - 100), true);
		if (curPath != startPath)
		{
			if (ImGui::Selectable(".."))
			{
				int pos = curPath.find_last_of("\\");
				curPath = curPath.erase(pos);
			}
		}
		for (const auto & entry : directory_iterator(curPath))
		{
			string fName = entry.path().filename().string();
			if (entry.is_directory())
			{
				if (ImGui::Selectable(fName.c_str()))
				{
					curPath = curPath + "\\" + fName;
					ImGui::EndChild();
					ImGui::End();
					return;
				}
			}
		}
		for (const auto & entry : directory_iterator(curPath))
		{
			string fName = entry.path().filename().string();
			if (!entry.is_directory() && (int)fName.find(fileSuffix) >= 0)
			{
				string fileFullPath = curPath + "\\" + fName;
				if (ImGui::Selectable(fName.c_str(), selectedFileName == fileFullPath))
				{
					selectedFileName = fileFullPath;
				}
			}
		}
		ImGui::EndChild();

		ImGui::BeginChild("buttons", ImVec2(ImGui::GetWindowSize().x - 20, 20));
		if (ImGui::Button("Import"))
		{

			(this->*fileImportCallback)(selectedFileName.erase(0, startPath.length() + 1));
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void MenuManager::ImportModel(string path)
{
	Object* newObj = new Object();
	SceneManager::Instance()->GetRootNode()->AddChild(newObj);
	int pos = path.find(".fbx");
	if (pos >= 0)
		FbxImportManager::Instance()->ImportFbxModel((char *)path.erase(pos).c_str(), newObj);
}

void MenuManager::ImportDiffuse(string path)
{
	MeshObject* meshObj = (MeshObject*)selectedObj;
	int pos = path.find(".tga");
	if (meshObj != nullptr && pos >= 0)
		meshObj->GetMaterial()->SetDiffuse(path.erase(pos));
}

void MenuManager::ImportNormal(string path)
{
	MeshObject* meshObj = (MeshObject*)selectedObj;
	int pos = path.find(".tga");
	if (meshObj != nullptr && pos >= 0)
		meshObj->GetMaterial()->SetNormal(path.erase(pos));
}

void MenuManager::ImportSpecular(string path)
{
	MeshObject* meshObj = (MeshObject*)selectedObj;
	int pos = path.find(".tga");
	if (meshObj != nullptr && pos >= 0)
		meshObj->GetMaterial()->SetSpecular(path.erase(pos));
}

void MenuManager::RenderMenu()
{
	if (!showMenu)
		return;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuManager::UpdateMenu()
{
	if (!showMenu)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	DrawMainMenuBar();

	if (showWorldEditorDialog)
		DrawWorldEditorDialog();

	if (showSceneEditorDialog)
		DrawSceneEditorDialog();

	if (showObjectEditorDialog)
		DrawObjectEditorDialog();

	if (showFileBroser)
		DrawFileBrowser();
}

void MenuManager::ToogleMenu()
{
	showMenu = !showMenu;
}

bool MenuManager::MenuStatus()
{
	return showMenu;
}

bool MenuManager::MouseOnMenu()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse;
}

Object * MenuManager::GetSelectedObj()
{
	return selectedObj;
}

void MenuManager::ResetSelectedObj()
{
	selectedObj = nullptr;
}

MenuManager::~MenuManager()
{
	ins = nullptr;
}

MenuManager * MenuManager::Instance()
{
	if (ins == nullptr)
		ins = new MenuManager();

	return ins;
}
