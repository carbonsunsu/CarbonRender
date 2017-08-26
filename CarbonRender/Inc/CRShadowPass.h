#ifndef CR_SHADOWPASS
#define CR_SHADOWPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWindowManager.h"

class ShadowPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);
public:
	void Init();
};

#endif