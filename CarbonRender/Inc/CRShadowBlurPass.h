#ifndef CR_BLURPASS
#define CR_BLURPASS

#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRCameraManager.h"

class ShadowBlurPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	float stepSize = 1.0f;
public:
	void Init();
};

#endif