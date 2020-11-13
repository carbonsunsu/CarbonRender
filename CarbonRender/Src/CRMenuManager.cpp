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
	showMaterialManager = false;

	selectedObj = nullptr;
	selectedMatID = 0;
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
		ImGui::Text(("<" + SceneManager::Instance()->GetCurSceneName() + ">").c_str());
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
	if (ImGui::MenuItem("New Scene"))
	{
		selectedObj = nullptr;
		SceneManager::Instance()->SetCurSceneName("New Scene");
		SceneManager::Instance()->ClearCurScene();
		SceneManager::Instance()->LoadDefaultScene();
	}
	if (ImGui::MenuItem("Open Scene"))
	{
		InitFileBrowser("Resources\\Scene", ".scene", &MenuManager::OpenNewScene);
	}
	if (ImGui::MenuItem("Save Scene"))
	{
		strcpy_s(curSceneNameChar, SceneManager::Instance()->GetCurSceneName().c_str());
		showSaveSceneDialog = true;
	}
	if (ImGui::MenuItem("Import Model"))
	{
		InitFileBrowser("Resources\\Models", ".fbx", &MenuManager::ImportModel);
	}
	if (ImGui::MenuItem("Add a Cube"))
	{
		MeshObject* cube = new MeshObject();
		cube->SetName("Cube");
		cube->SetMeshData(MeshManager::Instance()->GetBuildinBox());
		cube->SetMaterial(MaterialManager::Instance()->CreateNewMaterial()->GetID());
		SceneManager::Instance()->GetRootNode()->AddChild(cube);
	}
	if (ImGui::MenuItem("Add a Sphere"))
	{

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

			//float sunR = WeatherSystem::Instance()->GetSunR();
			//ImGui::DragFloat("Sun Distance", &sunR, 1.0f, 0.0f, FLT_MAX, "%.0f");
			//WeatherSystem::Instance()->SetSunR(sunR);
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
			ImTextureID texID = (ImTextureID)WeatherSystem::Instance()->GetWeatherMapId();
			ImGui::PushID("cloudMap");
			if (ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f)))
				InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportWeatherMap);
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text("Weather Map");
			ImGui::Text(WeatherSystem::Instance()->GetWeatherMapPath().c_str());
			ImGui::EndGroup();

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
			ImGui::Spacing();

			static float tempBiasScale = 1000.0f;
			ImGui::DragFloat("Manually Move Scale", &tempBiasScale, 1.0f, 1.0f, FLT_MAX, "%.0f");
			float tempBias = 0.0f;
			if (!ImGui::DragFloat("Drag to Move Cloud", &tempBias, 1.0f, -FLT_MAX, +FLT_MAX))
				tempBias = 0.0f;
			else
				WeatherSystem::Instance()->SetCloudBias(WeatherSystem::Instance()->GetCloudBias() + WeatherSystem::Instance()->GetWindDirection() * tempBias * tempBiasScale);
		}
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Fog");
		{
			float3 fogColor = WeatherSystem::Instance()->GetFogColor();
			float fogColorArray[3] = { fogColor.x, fogColor.y, fogColor.z };
			ImGui::ColorEdit3("Fog Color", fogColorArray);
			WeatherSystem::Instance()->SetFogColor(float3(fogColorArray[0], fogColorArray[1], fogColorArray[2]));

			float fogDen = WeatherSystem::Instance()->GetFogDensity();
			ImGui::DragFloat("Fog Density", &fogDen, 0.0001f, 0.0f, 1.0f, "%.4f");
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

	bool visiable = node->IsVisible();
	if (ImGui::Checkbox(("##" + to_string(-sceneTreeNodeIndex)).c_str(), &visiable))
		node->SetVisible(visiable, true, true);

	ImGuiTreeNodeFlags trueFlags = flags;

	if (node->GetChildCount() == 0)
		trueFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (selectedObj == node)
		trueFlags |= ImGuiTreeNodeFlags_Selected;

	ImGui::SameLine();
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
		if (ImGui::Selectable("Main Camera##Main", selectedObj == CameraManager::Instance()->GetCurrentCamera()))
			selectedObj = CameraManager::Instance()->GetCurrentCamera();

		//if (ImGui::Selectable("Main Light##Main", selectedObj == LightManager::Instance()->GetLight(WeatherSystem::Instance()->GetSunLightID())))
			//selectedObj = LightManager::Instance()->GetLight(WeatherSystem::Instance()->GetSunLightID());

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

		if (selectedObj == nullptr)
		{
			ImGui::End();
			return;
		}

		if (selectedObj->GetType() == ObjectType::eMesh)
		{
			MeshObject* meshObj = (MeshObject*)selectedObj;

			ImGui::Text("Material");
			{
				Material* mat = meshObj->GetMaterial();
				//Name
				if (ImGui::Button("Change"))
				{
					selectedMatID = mat->GetID();
					showMaterialManager = true;
				}

				if (mat == MaterialManager::Instance()->GetDefaultMaterial())
				{
					ImGui::SameLine();
					ImGui::Text("Missing Material");
				}
				else
				{
					ImGui::SameLine();
					char matNameChar[128];
					strcpy_s(matNameChar, mat->GetName().c_str());
					ImGui::InputText("##MatName", matNameChar, IM_ARRAYSIZE(matNameChar));
					mat->SetName(matNameChar);

					//Color
					float4 matColor = mat->GetColor();
					float matColorArray[4] = { matColor.x, matColor.y, matColor.z, matColor.w };
					ImGui::ColorEdit4("Color", matColorArray);
					mat->SetColor(float4(matColorArray[0], matColorArray[1], matColorArray[2], matColorArray[3]));

					//Diffuse
					ImTextureID texID;
					if (mat->HasDiffuseTexture())
						texID = (ImTextureID)mat->GetDiffuse();
					else
						texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

					ImGui::PushID(1);
					if (ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f)))
						InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportDiffuse);
					ImGui::PopID();

					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Text("Albedo");
					if (ImGui::Button("Clear##D"))
						mat->RemoveDiffuse();
					ImGui::SameLine();
					ImGui::Text(mat->GetDiffusePath().c_str());
					ImGui::EndGroup();

					float3 diffTilling = mat->GetDiffuseTilling();
					float tillingD[2] = { diffTilling.x, diffTilling.y };
					ImGui::DragFloat2("Tilling##D", tillingD, 0.1f, -FLT_MAX, FLT_MAX, "%.1f");
					mat->SetDiffuseTilling(tillingD[0], tillingD[1]);
					ImGui::Spacing();

					//Normal
					if (mat->HasNormalTexture())
						texID = (ImTextureID)mat->GetNormal();
					else
						texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

					ImGui::PushID(2);
					if (ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f)))
						InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportNormal);
					ImGui::PopID();

					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Text("Normal");
					if (ImGui::Button("Clear##N"))
						mat->RemoveNormal();
					ImGui::SameLine();
					ImGui::Text(mat->GetNormalPath().c_str());
					ImGui::EndGroup();

					float3 norlTilling = mat->GetNormalTilling();
					float tillingN[2] = { norlTilling.x, norlTilling.y };
					ImGui::DragFloat2("Tilling##N", tillingN, 0.1f, -FLT_MAX, FLT_MAX, "%.1f");
					mat->SetNormalTilling(tillingN[0], tillingN[1]);
					ImGui::Spacing();

					//Specular
					if (mat->HasSpecularTexture())
						texID = (ImTextureID)mat->GetSpecular();
					else
						texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

					ImGui::PushID(3);
					if (ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f)))
						InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportSpecular);
					ImGui::PopID();

					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Text("Specular(r:Roughness g:Metallic)");
					if (ImGui::Button("Clear##S"))
						mat->RemoveSpecular();
					ImGui::SameLine();
					ImGui::Text(mat->GetSpecularPath().c_str());
					ImGui::EndGroup();

					float3 specTilling = mat->GetSpecularTilling();
					float tillingS[2] = { specTilling.x, specTilling.y };
					ImGui::DragFloat2("Tilling##S", tillingS, 0.1f, -FLT_MAX, FLT_MAX, "%.1f");
					mat->SetSpecularTilling(tillingS[0], tillingS[1]);
					ImGui::Spacing();

					if (!mat->HasSpecularTexture())
					{
						float roughness = mat->GetRoughness();
						ImGui::DragFloat("Roughness", &roughness, 0.01f, 0.0f, 1.0f, "%.2f");
						mat->SetRoughness(roughness);

						float metallic = mat->GetMetallic();
						ImGui::DragFloat("Metallic", &metallic, 0.01f, 0.0f, 1.0f, "%.2f");
						mat->SetMetallic(metallic);

						ImGui::Spacing();
					}

					float3 emisColor = mat->GetEmissionColor();
					float emisInten = mat->GetEmissionIntensity();
					float emisColorArray[3] = { emisColor.x, emisColor.y, emisColor.z };
					ImGui::ColorEdit3("Emission Color", emisColorArray);
					mat->SetEmissionColor(float3(emisColorArray[0], emisColorArray[1], emisColorArray[2]));
					ImGui::DragFloat("Emission Intensity", &emisInten, 0.1f, 0.0f, FLT_MAX, "%.2f");
					mat->SetEmissionIntensity(emisInten);

					float alphaTestThreshold = mat->GetAlphaTestThreshold();
					ImGui::DragFloat("Alpha Test Threshold", &alphaTestThreshold, 0.001f, 0.0f, 1.0f);
					mat->SetAlphaTestThreshold(alphaTestThreshold);
				}
			}
		}
		else if (selectedObj->GetType() == ObjectType::eCamera)
		{
			Camera* camObj = (Camera*)selectedObj;
			
			float fov = camObj->GetFov();
			ImGui::DragFloat("FOV", &fov, 0.1f, 1.0f, 179.0f, "%.1f");
			camObj->SetFov(fov);

			float nearClip = camObj->GetNearClip();
			float farClip = camObj->GetFarClip();
			ImGui::DragFloatRange2("Clip Range", &nearClip, &farClip, 0.001f, 0.001f, FLT_MAX);
			camObj->SetNearClip(nearClip);
			camObj->SetFarClip(farClip);
		}
		else if (selectedObj->GetType() == ObjectType::eLight)
		{
			Light* lightObj = (Light*)selectedObj;

			float lightSize = lightObj->GetLightSize();
			ImGui::DragFloat("Light Size", &lightSize, 1.0f, 0.0f, +FLT_MAX, "%.0f");
			lightObj->SetLightSize(lightSize);
		}
	}
	ImGui::End();
}

void MenuManager::DrawSaveSceneDialog()
{
	ImGui::OpenPopup("Save Scene");

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
	windowFlags |= ImGuiWindowFlags_NoSavedSettings;
	windowFlags |= ImGuiWindowFlags_NoDocking;

	if (ImGui::BeginPopupModal("Save Scene", &showSaveSceneDialog, windowFlags))
	{
		ImGui::InputText("", curSceneNameChar, IM_ARRAYSIZE(curSceneNameChar));
		ImGui::Separator();
		ImGui::Spacing();
		
		if (ImGui::Button("Save") && string(curSceneNameChar).length() > 0)
		{
			SceneManager::Instance()->SetCurSceneName(string(curSceneNameChar));
			SceneManager::Instance()->SaveScene(SceneManager::Instance()->GetCurSceneName());
			ImGui::CloseCurrentPopup();
			showSaveSceneDialog = false;
		}
		ImGui::EndPopup();
	}
}

void MenuManager::DrawMaterialManagerDialog()
{
	ImGui::SetNextWindowSize(ImVec2(650, 500), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoSavedSettings;
	flags |= ImGuiWindowFlags_NoDocking;

	if (ImGui::Begin("Material Manager", &showMaterialManager, flags))
	{
		ImGui::BeginChild("matlist", ImVec2(ImGui::GetWindowSize().x - 316, ImGui::GetWindowSize().y - 60), true);
		{
			for (unordered_map<unsigned int, Material*>::iterator i = MaterialManager::Instance()->materials.begin(); i != MaterialManager::Instance()->materials.end(); i++)
			{
				if (ImGui::Selectable(i->second->GetName().c_str(), selectedMatID == i->second->GetID()))
				{
					selectedMatID = i->second->GetID();
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild("matPreview", ImVec2(276, ImGui::GetWindowSize().y - 60));
		{
			if (selectedMatID > 0)
			{
				Material* mat = MaterialManager::Instance()->GetMaterial(selectedMatID);

				//Color
				float4 matColor = mat->GetColor();
				ImGui::ColorButton("Material Color", ImVec4(matColor.x, matColor.y, matColor.z, matColor.w), 0, ImVec2(32, 32));

				//Diffuse
				ImTextureID texID;
				if (mat->HasDiffuseTexture())
					texID = (ImTextureID)mat->GetDiffuse();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				ImGui::Image(texID, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.0f, 1.0f, 1.0f , 1.0f));

				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("Albedo");
				ImGui::Text(mat->GetDiffusePath().c_str());
				ImGui::EndGroup();

				float3 diffTilling = mat->GetDiffuseTilling();
				ImGui::Text(("Tilling x:" + to_string(diffTilling.x) + " y:" + to_string(diffTilling.y)).c_str());
				ImGui::Spacing();

				//Normal
				if (mat->HasNormalTexture())
					texID = (ImTextureID)mat->GetNormal();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				ImGui::Image(texID, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("Normal");
				ImGui::Text(mat->GetNormalPath().c_str());
				ImGui::EndGroup();

				float3 norlTilling = mat->GetNormalTilling();
				ImGui::Text(("Tilling x:" + to_string(norlTilling.x) + " y:" + to_string(norlTilling.y)).c_str());
				ImGui::Spacing();

				//Specular
				if (mat->HasSpecularTexture())
					texID = (ImTextureID)mat->GetSpecular();
				else
					texID = (ImTextureID)TextureManager::Instance()->GetNullTex();

				ImGui::Image(texID, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("Specular(r:Roughness g:Metallic)");
				ImGui::Text(mat->GetSpecularPath().c_str());
				ImGui::EndGroup();

				float3 specTilling = mat->GetSpecularTilling();
				ImGui::Text(("Tilling x:" + to_string(specTilling.x) + " y:" + to_string(specTilling.y)).c_str());
				ImGui::Spacing();

				if (!mat->HasSpecularTexture())
				{
					float roughness = mat->GetRoughness();
					ImGui::Text(("Roughness: " + to_string(roughness)).c_str());

					float metallic = mat->GetMetallic();
					ImGui::Text(("Metallic: " + to_string(metallic)).c_str());
					ImGui::Spacing();
				}

				float3 emisColor = mat->GetEmissionColor();
				ImGui::ColorButton("Emission Color", ImVec4(emisColor.x, emisColor.y, emisColor.z, 0.0), 0, ImVec2(32, 32));
				ImGui::Text(("Emission Intensity: " + to_string(mat->GetEmissionIntensity())).c_str());

				ImGui::Text(("Alpha Test Threshold: " + to_string(mat->GetAlphaTestThreshold())).c_str());
			}
		}
		ImGui::EndChild();

		ImGui::BeginChild("buttons", ImVec2(ImGui::GetWindowSize().x, 20));
		if (ImGui::Button("Use", ImVec2(60, 20)))
		{
			MeshObject* meshObj = (MeshObject*)selectedObj;
			if (meshObj != nullptr)
			{
				meshObj->SetMaterial(selectedMatID);
				showMaterialManager = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("New", ImVec2(60, 20)))
		{
			InitInputPopup("New Material", "new_material", &MenuManager::CreateNewMaterial);
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete", ImVec2(60, 20)))
		{
			if (selectedMatID > 0)
			{
				MaterialManager::Instance()->DeleteMaterial(selectedMatID);
				selectedMatID = 0;
			}
		}
		ImGui::EndChild();
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
	ImGui::SetNextWindowSize(ImVec2(650, 500), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoSavedSettings;
	flags |= ImGuiWindowFlags_NoDocking;

	bool isTexFiles = false;
	if (fileSuffix == ".tga")
		isTexFiles = true;

	if (ImGui::Begin("Files", &showFileBroser, flags))
	{
		ImGui::Text(curPath.c_str());
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::BeginChild("filelist", ImVec2(ImGui::GetWindowSize().x - (isTexFiles ? 286 : 20), ImGui::GetWindowSize().y - 85), true);
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

		if (isTexFiles)
		{
			ImGui::SameLine();
			ImGui::BeginChild("preview", ImVec2(256, 276));
			ImGui::Text("Texture Preview");

			ImTextureID texID = 0;
			string temp = selectedFileName;
			if (!temp.empty())
			{
				temp = temp.erase(0, startPath.length() + 1);
				int pos = temp.find(".tga");
				if (pos >= 0)
					temp.erase(pos);
				texID = (ImTextureID)TextureManager::Instance()->LoadTexture((char*)temp.c_str());
			}
			ImGui::Image(texID, ImVec2(256, 256));

			ImGui::EndChild();
		}	

		ImGui::BeginChild("buttons", ImVec2(ImGui::GetWindowSize().x, 20));
		if (ImGui::Button("Open", ImVec2(60, 20)))
		{
			(this->*fileImportCallback)(selectedFileName.erase(0, startPath.length() + 1));
			showFileBroser = false;
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void MenuManager::InitInputPopup(string title, string defaultChar, InputPopupCallback callback)
{
	strcpy_s(newInputInitChar, defaultChar.c_str());
	inputPopupTitle = title;
	inputPopupCallback = callback;
	showInputPopup = true;
}

void MenuManager::DrawInputPopup()
{
	ImGui::OpenPopup(inputPopupTitle.c_str());

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
	windowFlags |= ImGuiWindowFlags_NoSavedSettings;
	windowFlags |= ImGuiWindowFlags_NoDocking;

	if (ImGui::BeginPopupModal(inputPopupTitle.c_str(), &showInputPopup, windowFlags))
	{
		ImGui::InputText("", newInputInitChar, IM_ARRAYSIZE(newInputInitChar));
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Save") && string(newInputInitChar).length() > 0)
		{
			(this->*inputPopupCallback)(newInputInitChar);
			ImGui::CloseCurrentPopup();
			showInputPopup = false;
		}
		ImGui::EndPopup();
	}
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

void MenuManager::OpenNewScene(string path)
{
	int pos = path.find(".scene");
	string sceneName;
	if (pos >= 0)
	{
		sceneName = path.erase(pos);
		selectedObj = nullptr;
		SceneManager::Instance()->SetCurSceneName(sceneName);
		SceneManager::Instance()->ClearCurScene();
		SceneManager::Instance()->LoadScene(sceneName);
	}
}

void MenuManager::ImportWeatherMap(string path)
{
	int pos = path.find(".tga");
	if (pos >= 0)
		WeatherSystem::Instance()->SetWeatherMap(path.erase(pos));
}

void MenuManager::CreateNewMaterial(string name)
{
	selectedMatID = MaterialManager::Instance()->CreateNewMaterial(name)->GetID();
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

	if (showSaveSceneDialog)
		DrawSaveSceneDialog();

	if (showMaterialManager)
		DrawMaterialManagerDialog();

	if (showInputPopup)
		DrawInputPopup();
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
