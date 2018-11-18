#ifndef CR_SSR
#define CR_SSR

#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRCameraManager.h"

class SSRPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	float targetScale = 0.25f;
public:
	void Init();
};

#endif
