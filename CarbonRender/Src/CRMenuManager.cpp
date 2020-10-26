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
	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::Instance()->GetWindow(), true);
	ImGui_ImplOpenGL3_Init();

	showModelImportDialog = false;
	showWorldEditorDialog = false;
	showSceneEditorDialog = false;
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
	ImGui::SetNextWindowSize(ImVec2(500, 800), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags flags = 0;
	flags |= flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	if (ImGui::Begin("World Editor", &showWorldEditorDialog, flags))
	{
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::Text("Time");
		{
			float timeSpeed = WeatherSystem::Instance()->GetTimeSpeed();
			ImGui::DragFloat("Time Lapse Speed", &timeSpeed, 0.1f, -FLT_MAX, +FLT_MAX);
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
			static float tempColor[3] = { fogColor.x, fogColor.y, fogColor.z };
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

void MenuManager::DrawSceneEditorDialog()
{
	Object* sceneRoot = SceneManager::Instance()->GetRootNode();
}

void MenuManager::DrawObjectEditorDialog()
{
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

	DrawMainMenuBar();

	if (showModelImportDialog)
		DrawImportModelDialog();

	if (showWorldEditorDialog)
		DrawWorldEditorDialog();

	if (showSceneEditorDialog)
		DrawSceneEditorDialog();

	if (showObjectEditorDialog)
		DrawObjectEditorDialog();
	/**
	ImGui::Begin("CarbonRender");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	static bool tmp0 = true;
	static bool tmp1 = false;
	ImGui::Checkbox("Demo Window", &tmp0);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &tmp1);

	static float f = 0.0f;
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImVec4 tmp2 = ImVec4(0.5, 0.2, 0.4, 0.1);
	ImGui::ColorEdit3("clear color", (float*)&tmp2); // Edit 3 floats representing a color

	static int counter = 0;
	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	/**/
}

void MenuManager::ToogleMenu()
{
	showMenu = !showMenu;
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
