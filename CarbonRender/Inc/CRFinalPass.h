#ifndef CR_FINALPASS
#define CR_FINALPASS

#include "..\Inc\CRRenderPass.h"

class FinalPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);
public:
	void Init();
};

#endif