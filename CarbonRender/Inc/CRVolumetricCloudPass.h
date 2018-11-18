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
	GLuint noises[3];//PerlinWorley, Worley, Curl
	GLuint weatherData;
	float4 cloudBoxScale;

	void GenerateTex(); 
public:
	void Init();
};

#endif