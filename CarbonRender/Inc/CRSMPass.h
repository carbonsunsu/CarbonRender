#ifndef CR_SMPASS
#define CR_SMPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRSceneManager.h"
#include "..\Inc\CRWeatherSystem.h"

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