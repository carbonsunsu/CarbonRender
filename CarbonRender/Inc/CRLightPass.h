#ifndef CR_LIGHTPASS
#define CR_LIGHTPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRWeatherSystem.h"
#include "..\Inc\CRCameraManager.h"

class LightPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);
public:
	void Init();
};

#endif
