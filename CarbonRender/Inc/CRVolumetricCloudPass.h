#ifndef CR_VOLCLOUD
#define	CR_VOLCLOUD

#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRWeatherSystem.h"

class VolumetricCloudPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	GLuint dBuffer;
	MeshObject cloudBox;
	float targetSizeScaler;
public:
	void Init();
};

#endif