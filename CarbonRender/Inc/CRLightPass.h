#ifndef CR_LIGHTPASS
#define CR_LIGHTPASS

#include "..\Inc\CRRenderPass.h"
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
