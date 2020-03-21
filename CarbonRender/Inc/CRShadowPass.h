#ifndef CR_SHADOWPASS
#define CR_SHADOWPASS

#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRWeatherSystem.h"

class ShadowPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);
public:
	void Init();
};

#endif