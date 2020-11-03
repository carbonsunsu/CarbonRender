#include "..\Inc\CRFreeController.h"

void FreeController::Init()
{
	rotSensitivity[0] = 0.2f;
	rotSensitivity[1] = 0.1f;

	mouseMovSensitivity[0] = 0.02f;
	mouseMovSensitivity[1] = 0.01f;
	mouseMovSensitivity[2] = 0.1f;

	moveSpeedLow = 20.0f;
	moveSpeedHigh = 2000.0f;
	moveSpeed = moveSpeedLow;
	ctrlPressed = false;
	v = float3(0.0f);
}

void FreeController::Update()
{
	Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
	if (!MenuManager::Instance()->MenuStatus() || curMouseButton == GLFW_MOUSE_BUTTON_RIGHT)
	{
		float3 curPos = curCam->GetPosition();
		curPos = curPos + (curCam->GetForward().normalize() * v.x  * moveSpeed +
			curCam->GetRight().normalize() * v.y  * moveSpeed +
			curCam->GetUp().normalize() * v.z  * moveSpeed) * FIXEDUPDATE_TIME;
		CameraManager::Instance()->GetCurrentCamera()->SetPosition(curPos);
	}
}

void FreeController::KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
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
	case GLFW_KEY_W:
	{
		if (action == GLFW_RELEASE)
			v.x = 0.0f;
	}
	break;
	case GLFW_KEY_S:
	{
		if (action == GLFW_RELEASE)
			v.x = 0.0f;
	}
	break;
	case GLFW_KEY_A:
	{
		if (action == GLFW_RELEASE)
			v.y = 0.0f;
	}
	break;
	case GLFW_KEY_D:
	{
		if (action == GLFW_RELEASE)
			v.y = 0.0f;
	}
	break;
	case GLFW_KEY_Q:
	{
		if (action == GLFW_RELEASE)
		v.z = 0.0f;
	}
	break;
	case GLFW_KEY_E:
	{
		if (action == GLFW_RELEASE)
			v.z = 0.0f;
	}
	break;
	case GLFW_KEY_LEFT_SHIFT:
	{
		if (action == GLFW_RELEASE)
			moveSpeed = moveSpeedLow;
	}
	break;
	case GLFW_KEY_LEFT_CONTROL:
	{
		if (action == GLFW_RELEASE)
			ctrlPressed = false;
	}
	break;
	case GLFW_KEY_DELETE:
	{
		if (action == GLFW_RELEASE)
		{
			Object* selectedObj = MenuManager::Instance()->GetSelectedObj();
			if (selectedObj != nullptr && selectedObj != SceneManager::Instance()->GetRootNode())
			{
				SceneManager::Instance()->DeleteSceneNode(selectedObj);
				MenuManager::Instance()->ResetSelectedObj();
			}
		}
	}
	break;
	}

	if (MenuManager::Instance()->MenuStatus() && curMouseButton != GLFW_MOUSE_BUTTON_RIGHT)
		return;

	switch (key)
	{

	default:
		break;
	case GLFW_KEY_W:
	{
		v.x = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1.0f : 0.0f;
	}
	break;
	case GLFW_KEY_S:
	{
		v.x = (action == GLFW_PRESS || action == GLFW_REPEAT) ? -1.0f : 0.0f;
	}
	break;
	case GLFW_KEY_A:
	{
		v.y = (action == GLFW_PRESS || action == GLFW_REPEAT) ? -1.0f : 0.0f;
	}
	break;
	case GLFW_KEY_D:
	{
		v.y = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1.0f : 0.0f;
	}
	break;
	case GLFW_KEY_Q:
	{
		v.z = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1.0f : 0.0f;
	}
	break;
	case GLFW_KEY_E:
	{
		v.z = (action == GLFW_PRESS || action == GLFW_REPEAT) ? -1.0f : 0.0f;
	}
	break;
	case GLFW_KEY_LEFT_SHIFT:
	{
		moveSpeed = (action == GLFW_PRESS || action == GLFW_REPEAT) ? moveSpeedHigh : moveSpeedLow;
	}
	break;
	case GLFW_KEY_T:
	{
		if (action == GLFW_PRESS)
			WeatherSystem::Instance()->ToggleTimeLapse();
	}
	break;
	case GLFW_KEY_LEFT_CONTROL:
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			ctrlPressed = true;
		else
			ctrlPressed = false;
	}
	break;

	}
}

void FreeController::MouseMotionCallback(GLFWwindow* window, double x, double y)
{
	if (MenuManager::Instance()->MouseOnMenu())
		return;

	switch (curMouseButton)
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		if (!ctrlPressed)
		{
			float3 curR = CameraManager::Instance()->GetCurrentCamera()->GetRotation();
			curR.y += (x - lastMousePos[0]) * rotSensitivity[0];
			curR.x += (y - lastMousePos[1]) * rotSensitivity[1];
			if (curR.x > 90.0f) curR.x = 90.0f;
			if (curR.x < -90.0f) curR.x = -90.0f;
			CameraManager::Instance()->GetCurrentCamera()->SetRotation(curR);
		}
		else
		{
			Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
			float3 curPos = curCam->GetPosition();
			float3 curFoward = curCam->GetForward();

			curPos = curPos - (y - lastMousePos[1]) * mouseMovSensitivity[2] * curFoward;
			curCam->SetPosition(curPos);
		}
	}
		break;
	case GLFW_MOUSE_BUTTON_LEFT:
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
		float3 curPos = curCam->GetPosition();
		float3 curUp = curCam->GetUp();
		float3 curRight = curCam->GetRight();

		curPos = curPos - (x - lastMousePos[0]) * mouseMovSensitivity[0] * curRight + (y - lastMousePos[1]) * mouseMovSensitivity[1] * curUp;
		curCam->SetPosition(curPos);
		break;
	}
	lastMousePos[0] = x;
	lastMousePos[1] = y;
}

void FreeController::ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
	float3 curPos = curCam->GetPosition();
	float3 curFoward = curCam->GetForward();

	curPos = curPos + mouseMovSensitivity[2] * curFoward * yOffset;
	curCam->SetPosition(curPos);
}
