#ifndef CR_GIPASS
#define CR_GIPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWindowManager.h"

class GIPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	float targetScale = 0.25f;
	GLuint denoiseShaderProgram;
	GLuint denoiseRt;
	float denoiseStepSize;
public:
	void Init();
};

#endif