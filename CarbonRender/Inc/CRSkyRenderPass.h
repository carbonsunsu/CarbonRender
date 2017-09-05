#ifndef CR_SKYPASS
#define CR_SKYPASS

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWeatherSystem.h"

class SkyRenderPass : public RenderPass
{
private:
	MeshObject sphere;
	int cubeSize = 256;
	GLuint fboCube;
	GLuint milkwayTex;

	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);
public:
	void Init();
};

#endif