#ifndef CR_VOLCLOUD
#define	CR_VOLCLOUD

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRRenderPass.h"
#include "..\Inc\CRMeshObject.h"
#include "..\Inc\CRFbxImportManager.h"
#include "..\Inc\CRWindowManager.h"
#include "..\Inc\CRCameraManager.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWeatherSystem.h"

class VolumetricCloudPass : public RenderPass
{
private:
	void GetReady4Render(PassOutput* input);
	void Render(PassOutput* input);

	GLuint dBuffer;
	MeshObject cloudBox;
	GLuint noises[3];//PerlinWorley, Worley, Curl
	float4 cloudBoxScale;

	void GenerateTex(); 
public:
	void Init();
};

#endif