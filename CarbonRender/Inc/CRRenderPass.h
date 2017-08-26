#ifndef CR_PASSOBJ
#define CR_PASSOBJ

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRMath.h"

struct PassOutput
{
	int cout = 0;
	GLuint* RTS;
	Matrix4x4* mats;
};

class RenderPass
{
protected:
	GLuint shaderProgram;
	GLuint fbo;
	GLuint dBuffer;

	GLuint quadVao;
	
	GLuint quadBos[3];//VertexBuffer, UVBuffer, ElementBuffer
	PassOutput output;
	bool fullScreenQuadReady = false;
	bool bReady = false;
	
	virtual void GetReady4Render(PassOutput* input);
	virtual void Render(PassOutput* input);
	void DrawFullScreenQuad();
public:
	~RenderPass();
	void AttachShader(GLuint program);
	virtual void Init();
	PassOutput* Draw(PassOutput* input);
};

#endif
