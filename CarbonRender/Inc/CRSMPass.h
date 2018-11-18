#ifndef CR_SMPASS
#define CR_SMPASS

#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRSceneManager.h"

class SMPass : public RenderPass
{
private:
	int shadowMapScale = 4;
	bool followCam;
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);
public:
	void Init();
};

#endif