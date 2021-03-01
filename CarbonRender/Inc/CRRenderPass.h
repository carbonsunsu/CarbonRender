#ifndef CR_PASSOBJ
#define CR_PASSOBJ

#include "..\Inc\CRGLHelper.h"
#include "..\Inc\CRLightManager.h"
#include "..\Inc\CRShaderManager.h"
#include "..\Inc\CRWindowManager.h"

struct PassOutput
{
	int cout = 0;
	GLuint* RTS;
	Matrix4x4* mats;
};

class RenderPass
{
protected:
	unsigned int shaderProgramIndex;
	GLuint fbo;
	GLuint dBuffer;

	GLuint quadVao;
	
	GLuint quadBos[3];//VertexBuffer, UVBuffer, ElementBuffer
	PassOutput output;
	bool fullScreenQuadReady = false;
	bool bReady = false;
	bool enable = true;
	
	virtual void GetReady4Render(PassOutput* input);
	virtual void Render(PassOutput* input);
	void DrawFullScreenQuad();
public:
	~RenderPass();
	void AttachShader(char* vShaderName, char* fShaderName);
	virtual void Init();
	PassOutput* Draw(PassOutput* input);
	void Enable(bool bEnable = true);
};

#endif
