#ifndef CR_BLURPASS
#define CR_BLURPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWindowManager.h"

class ShadowBlurPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	float step = 1.0f;
	GLuint tempRt;
public:
	void Init();
};

#endif