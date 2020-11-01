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

	showModelImportDialog = false;
	showWorldEditorDialog = true;
	showSceneEditorDialog = true;
	showObjectEditorDialog = true;

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
		showModelImportDialog = true;
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

void MenuManager::DrawImportModelDialog()
{
	string modelResourcePath = "Resources\\Models";
	static int selected = -1;
	static string selectedFileName = "";
	modelList.clear();
	for (const auto & entry : directory_iterator(modelResourcePath))
	{
		if (strstr(entry.path().string().c_str(), ".fbx"))
		{
			modelList.emplace_back(entry.path().filename().string());
		}
	}

	ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;
	//flags |= ImGuiWindowFlags_AlwaysAutoResize;

	if (ImGui::Begin("Import Model", &showModelImportDialog, flags))
	{
		ImGui::BeginChild("Model List", ImVec2(200, 0), true);
		for (int i = 0; i < modelList.size(); i++)
		{
			string fileName = modelList[i];
			if (ImGui::Selectable(fileName.c_str(), selected == i))
			{
				selected = i;
				selectedFileName = fileName;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild("Op Panel", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		if (ImGui::Button("Import") && !selectedFileName.empty())
		{
			Object* newObj = new Object();
			SceneManager::Instance()->GetRootNode()->AddChild(newObj);
			int pos = selectedFileName.find(".fbx");
			if (pos >= 0)
				FbxImportManager::Instance()->ImportFbxModel((char *)selectedFileName.erase(pos).c_str(), newObj);
		}
		ImGui::EndChild();
	}
	ImGui::End();
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
	ImGuiTreeNodeFlags trueFlags = flags;

	if (node->GetChildCount() == 0)
		trueFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (selectedObj == node)
		trueFlags |= ImGuiTreeNodeFlags_Selected;

	bool isExtend = ImGui::TreeNodeEx(node->GetName().c_str(), trueFlags);
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
		Object* sceneRoot = SceneManager::Instance()->GetRootNode();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		DrawSceneNode(sceneRoot, flags);
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

				ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f));

				ImGui::SameLine();
				ImGui::Text("Albedo");

				if (mat->HasNormalTexture())
					texID = (ImTextureID)mat->GetNormal();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f));

				ImGui::SameLine();
				ImGui::Text("Normal");

				if (mat->HasSpecularTexture())
					texID = (ImTextureID)mat->GetSpecular();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f));

				ImGui::SameLine();
				ImGui::Text("Specular(r:Roughness g:Metallic)");

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

	if (showModelImportDialog)
		DrawImportModelDialog();

	if (showWorldEditorDialog)
		DrawWorldEditorDialog();

	if (showSceneEditorDialog)
		DrawSceneEditorDialog();

	if (showObjectEditorDialog)
		DrawObjectEditorDialog();
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

MenuManager::~MenuManager()
{
	modelList.clear();
	modelList.shrink_to_fit();
	ins = nullptr;
}

MenuManager * MenuManager::Instance()
{
	if (ins == nullptr)
		ins = new MenuManager();

	return ins;
}
