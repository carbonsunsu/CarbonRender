#include "..\Inc\CRGPass.h"

void GPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt_n, rt_albedo, rt_s;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	rt_albedo = SetGLRenderTexture(size.w, size.h, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0);
	rt_n = SetGLRenderTexture(size.w, size.h, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1);
	rt_s = SetGLRenderTexture(size.w, size.h, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT2);

	dBuffer = SetGLDepthBuffer(size.w, size.h);

	GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 3;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = rt_albedo;
	output.RTS[1] = rt_n;
	output.RTS[2] = rt_s;
}

void GPass::Render(PassOutput * input)
{
	CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();

	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.Render();
	type59.Render();

	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("GPass.vert", "GPass.frag");
	FbxImportManager::Instance()->ImportFbxModel("Terrain.fbx", &scene);
	FbxImportManager::Instance()->ImportFbxModel("Type59.fbx", &type59);
	scene.GetReady4Rending();
	scene.AttachShader(shaderProgram);
	type59.GetReady4Rending();
	type59.AttachShader(shaderProgram);

	type59.SetPosition(float3(0.0f, -0.5f, -17.5f));
}
