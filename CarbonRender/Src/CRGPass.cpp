#include "..\Inc\CRGPass.h"

void GPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt_n, rt_albedo, rt_wsP, rt_t;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	rt_n = SetGLRenderTexture(size.w, size.h, GL_RGBA, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0);
	rt_albedo = SetGLRenderTexture(size.w, size.h, GL_RGBA, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1);
	rt_wsP = SetGLRenderTexture(size.w, size.h, GL_RGBA, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT2);
	rt_t = SetGLRenderTexture(size.w, size.h, GL_RGBA, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT3);

	dBuffer = SetGLDepthBuffer(size.w, size.h);

	GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 4;
	output.RTS = new GLuint[4];
	output.RTS[0] = rt_albedo;
	output.RTS[1] = rt_n;
	output.RTS[2] = rt_wsP;
	output.RTS[3] = rt_t;
}

void GPass::Render(PassOutput * input)
{
	CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();

	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.Render();
}

void GPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("GPass.vert", "GPass.frag");
	FbxImportManager::Instance()->ImportFbxModel("Hangar.fbx", &scene);
	scene.GetReady4Rending();
	scene.AttachShader(shaderProgram);
}
