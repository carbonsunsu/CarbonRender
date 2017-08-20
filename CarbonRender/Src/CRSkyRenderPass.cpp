#include "..\Inc\CRSkyRenderPass.h"

void SkyRenderPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("Atmosphere.vert", "Atmosphere.frag");
	FbxImportManager::Instance()->ImportFbxModel("sphere.fbx", &sphere);
	sphere.GetReady4Rending();
	sphere.AttachShader(shaderProgram);
	sphere.SetScale(float3(10000.0f));
}

void SkyRenderPass::GetReady4Render(PassOutput* input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt0;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glGenTextures(1, &rt0);
	glBindTexture(GL_TEXTURE_2D, rt0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt0, 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	output.cout = 1;
	output.RTS = new GLuint[1];
	output.RTS[0] = rt0;
}

void SkyRenderPass::Render(PassOutput* input)
{
	sphere.SetPosition(CameraManager::Instance()->GetCurrentCamera()->GetPosition());
	float3 para = CameraManager::Instance()->GetCurrentCamera()->GetCameraPara();
	Camera cam;
	cam.SetPerspectiveCamera(para.x, 1000.0f, 20000.0f);
	cam.SetPosition(CameraManager::Instance()->GetCurrentCamera()->GetPosition());
	cam.SetRotation(CameraManager::Instance()->GetCurrentCamera()->GetRotation());
	cam.UpdateViewMatrix();
	CameraManager::Instance()->Push(cam);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	
	sphere.Render();

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CameraManager::Instance()->Pop();
}

