#ifndef CR_CTRLMANAGER
#define CR_CTRLMANAGER

#include "..\Inc\CRController.h"
#include "..\Inc\CRFreeController.h"

class ControllerManager 
{
private:
	ControllerManager();
	static ControllerManager* ins;
	stack<Controller*> ctrls;

public:
	~ControllerManager();
	static ControllerManager* Instance();
	void Push(Controller* ctrl);
	void Pop();
	Controller* GetCurrentController();
};

#endif