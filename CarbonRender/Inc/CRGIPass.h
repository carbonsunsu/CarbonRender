#ifndef CR_GIPASS
#define CR_GIPASS

#include "..\Inc\CRRenderPass.h"

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