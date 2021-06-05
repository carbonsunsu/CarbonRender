#include "..\Inc\CRCinematicController.h"

float CinematicController::GetCurrentTime()
{
	clock_t currentTime;
	currentTime = clock();
	return (float)currentTime/CLOCKS_PER_SEC;
}

void CinematicController::Init()
{
	end = true;
	filmStartTime = -1;
}

void CinematicController::Update()
{
	float stage0Cost = 6.0f;
	float stage1Cost = 3.0f;
	float stage2Cost = 6.0f;

	if (!end)
	{
		float currentTime = GetCurrentTime();

		if (currentTime - filmStartTime >= stage0Cost + stage1Cost + stage2Cost)
		{
			end = true;
		}
		else
		{
			float process = (currentTime - filmStartTime) / (stage0Cost + stage1Cost + stage2Cost);
			process = Math::Min(process, 1.0f);
			float sunStartTime = 14.8f;
			float sunEndTime = 16.6f;
			float fogStartPrec = 55.0f;
			float fogEndPrec = 60.0f;
			
			float currentSunTime = (1.0f - process) * sunStartTime + process * sunEndTime;
			WeatherSystem::Instance()->SetHour(currentSunTime);

			float currentFogPrec = (1.0f - process) * fogStartPrec + process * fogEndPrec;
			WeatherSystem::Instance()->SetFogPrecipitation(currentFogPrec);

			if (currentTime - filmStartTime <= stage0Cost)
			{
				float3 camStartPos = float3(11.868318f, 5.0f, -8.464635f);
				float3 camEndPos = float3(11.868318f, 3.151639f, -8.464635f);

				process = (currentTime - filmStartTime) / stage0Cost;
				process = Math::Min(process, 1.0f);
				process = (-Math::Cos(process * PI) + 1.0f) * 0.5f;

				float3 camCurrentPos = (1.0f - process) * camStartPos + process * camEndPos;
				CameraManager::Instance()->GetCurrentCamera()->SetPosition(camCurrentPos);
			}
			else if (currentTime - filmStartTime <= stage0Cost + stage1Cost)
			{
				float3 camStartPos = float3(11.868318f, 3.151639f, -8.464635f);
				float3 camEndPos = float3(9.648738f, 3.151639f, -4.547123f);
				float3 camStartRota = float3(3.600040f, -54.599995f, 0.0f);
				float3 camEndRota = float3(1.500041f, -43.599918f, 0.0f);
				float camStartFov = 55.0f;
				float camEndFov = 60.0f;

				process = (currentTime - filmStartTime - stage0Cost) / stage1Cost;
				process = Math::Min(process, 1.0f);
				process = (-Math::Cos(process * PI) + 1.0f) * 0.5f;

				float3 camCurrentPos = (1.0f - process) * camStartPos + process * camEndPos;
				CameraManager::Instance()->GetCurrentCamera()->SetPosition(camCurrentPos);

				float3 camCurrentRota = (1.0f - process) * camStartRota + process * camEndRota;
				CameraManager::Instance()->GetCurrentCamera()->SetRotation(camCurrentRota);

				float camCurrentFov = (1.0f - process) * camStartFov + process * camEndFov;
				CameraManager::Instance()->GetCurrentCamera()->SetFov(camCurrentFov);
			}
			else
			{
				float3 camStartPos = float3(9.648738f, 3.151639f, -4.547123f);
				float3 camEndPos = float3(11.303256f, 3.151639f, -2.809704f);

				process = (currentTime - filmStartTime - stage0Cost - stage1Cost) / stage2Cost;
				process = Math::Min(process, 1.0f);
				process = (-Math::Cos(process * PI) + 1.0f) * 0.5f;

				float3 camCurrentPos = (1.0f - process) * camStartPos + process * camEndPos;
				CameraManager::Instance()->GetCurrentCamera()->SetPosition(camCurrentPos);
			}
		}
	}
}

void CinematicController::KeyInputCallback(GLFWwindow * window, int key, int scanCode, int action, int mods)
{
	switch (key)
	{
	default:
		break;
	case GLFW_KEY_F1:
	{
		if (action == GLFW_PRESS)
			MenuManager::Instance()->ToogleMenu();
	}
	break;
	case GLFW_KEY_P:
	{
		if (action == GLFW_PRESS)
		{
			ControllerManager::Instance()->Pop();
		}
	}
	break;
	case GLFW_KEY_SPACE:
	{
		if (action == GLFW_PRESS && end)
		{
			WeatherSystem::Instance()->SetHour(14.8f);
			WeatherSystem::Instance()->SetFogPrecipitation(55.0f);
			CameraManager::Instance()->GetCurrentCamera()->SetFov(55.0f);
			CameraManager::Instance()->GetCurrentCamera()->SetPosition(float3(11.868318f, 5.0f, -8.464635f));
			CameraManager::Instance()->GetCurrentCamera()->SetRotation(float3(3.600040f, -54.599995f, 0.0f));

			filmStartTime = GetCurrentTime();
			end = false;
		}
	}
	break;
	case GLFW_KEY_BACKSPACE:
	{
		if (action == GLFW_PRESS)
		{
			filmStartTime = GetCurrentTime();
		}
	}
	break;
	}
}

void CinematicController::MouseMotionCallback(GLFWwindow * window, double x, double y)
{
}

void CinematicController::ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
}
