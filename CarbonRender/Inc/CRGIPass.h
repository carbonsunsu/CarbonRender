#ifndef CR_GIPASS
#define CR_GIPASS

#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRWeatherSystem.h"

class GIPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	float targetScale = 0.25f;
	unsigned int denoiseShaderProgramIndex;
	GLuint denoiseRt;
	float denoiseStepSize;
public:
	void Init();
};

#endif