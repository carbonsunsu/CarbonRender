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
	rt0 = SetGLRenderTexture(size.w, size.h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	
	ShaderManager::Instance()->UseShader(shaderProgram);
	float* shaderParas = WeatherSystem::Instance()->GetShaderParas();
	GLuint location = glGetUniformLocation(shaderProgram, "t");
	glUniform1f(location, shaderParas[0]);
	location = glGetUniformLocation(shaderProgram, "exposure");
	glUniform1f(location, shaderParas[1]);
	location = glGetUniformLocation(shaderProgram, "thetaS");
	glUniform1f(location, shaderParas[2]);
	location = glGetUniformLocation(shaderProgram, "wsSunPos");
	glUniform4f(location, shaderParas[3], shaderParas[4], shaderParas[5], shaderParas[6]);
	location = glGetUniformLocation(shaderProgram, "zenith");
	glUniform3f(location, shaderParas[7], shaderParas[8], shaderParas[9]);

	sphere.Render();

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CameraManager::Instance()->Pop();
}

