#include "..\Inc\CRControllerManager.h"

ControllerManager* ControllerManager::ins = nullptr;

ControllerManager::ControllerManager()
{
}

ControllerManager::~ControllerManager()
{
	while (!ctrls.empty())
		ctrls.pop();

	ins = nullptr;
}

ControllerManager * ControllerManager::Instance()
{
	if (ins == nullptr)
		ins = new ControllerManager();

	return ins;
}

void ControllerManager::Push(Controller* ctrl)
{
	ctrls.push(ctrl);
}

void ControllerManager::Pop()
{
	delete ctrls.top();
	ctrls.pop();
}

Controller * ControllerManager::GetCurrentController()
{
	return ctrls.top();
}
