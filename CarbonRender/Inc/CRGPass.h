#ifndef CR_GPASS
#define CR_GPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWeatherSystem.h"

class GPass : public RenderPass
{
private:
	MeshObject scene;
	MeshObject type59;

	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);
public:
	void Init();
};

#endif