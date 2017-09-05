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
public:
	void Init();
};

#endif