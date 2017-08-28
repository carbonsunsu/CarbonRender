#include "..\Inc\CRSMPass.h"

void SMPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt_sm, rt_vpl;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	rt_sm = SetGLRenderTexture(size.w * shadowMapScale, size.h * shadowMapScale, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0);
	rt_vpl = SetGLRenderTexture(size.w * shadowMapScale, size.h * shadowMapScale, GL_RGB32F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT1);

	dBuffer = SetGLDepthBuffer(size.w * shadowMapScale, size.h * shadowMapScale);

	GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 2;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = rt_sm;
	output.RTS[1] = rt_vpl;
	output.mats = new Matrix4x4[1];
}

void SMPass::Render(PassOutput * input)
{
	float farCip = 10000.0f;
	Camera cam;
	cam.SetOrthoCamera(50.0f, 1.0f, farCip);
	cam.SetPosition(WeatherSystem::Instance()->GetWsSunPos().normalize() * farCip * 0.5f + CameraManager::Instance()->GetCurrentCamera()->GetPosition());
	cam.LookAt(CameraManager::Instance()->GetCurrentCamera()->GetPosition());
	CameraManager::Instance()->Push(cam);
	CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();
	output.mats[0] = CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix() * CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix();

	glEnable(GL_DEPTH_TEST);
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w * shadowMapScale, size.h * shadowMapScale);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderManager::Instance()->UseShader(shaderProgram);
	SceneManager::Instance()->DrawScene(shaderProgram);

	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glViewport(0, 0, size.w, size.h);

	CameraManager::Instance()->Pop();
}

void SMPass::Init()
{
	shadowMapScale = 4;
	shaderProgram = ShaderManager::Instance()->LoadShader("SM.vert", "SM.frag");
}
