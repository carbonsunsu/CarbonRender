#ifndef CR_SSAOPASS
#define CR_SSAOPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRTextureManager.h"
#include "..\Inc\CRCameraManager.h"

class SSAOPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	GLuint rnmTex;
public:
	void Init();
};

#endif