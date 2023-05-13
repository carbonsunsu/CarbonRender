#include "..\Inc\CRSMPass.h"

void SMPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint depthMapLv0, depthMapLv1, depthMapLv2, vplPos, vplAlbedo, vplNormal;
	unsigned int sunLightID = WeatherSystem::Instance()->GetSunLightID();
	Light* sunLight = LightManager::Instance()->GetLight(sunLightID);
	int shadowMapSize = sunLight->GetShadowMapSize();
	depthMapLv0 = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_R32F, GL_RED, GL_FLOAT, GL_NEAREST, GL_COLOR_ATTACHMENT0);
	vplPos = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_RGB32F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_COLOR_ATTACHMENT1);
	vplAlbedo = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_COLOR_ATTACHMENT2);
	vplNormal = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_RGB32F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_COLOR_ATTACHMENT3);
	depthMapLv1 = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_R32F, GL_RED, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT4);
	depthMapLv2 = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_R32F, GL_RED, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT5);

	dBuffer = GLHelper::SetGLDepthBuffer(shadowMapSize, shadowMapSize);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 6;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = vplPos;
	output.RTS[1] = vplAlbedo;
	output.RTS[2] = vplNormal;
	output.RTS[3] = depthMapLv0;
	output.RTS[4] = depthMapLv1;
	output.RTS[5] = depthMapLv2;
	output.mats = new Matrix4x4[4];
}

void SMPass::Render(PassOutput * input)
{
	unsigned int sunLightID = WeatherSystem::Instance()->GetSunLightID();
	Light* sunLight = LightManager::Instance()->GetLight(sunLightID);
	float farClip = sunLight->GetFarClip();
	float nearClip = sunLight->GetNearClip();
	int shadowMapSize = sunLight->GetShadowMapSize();
	float3 depthClampPara = sunLight->GetDepthClampPara();
	unsigned int* levelSetting = LightManager::Instance()->GetShadowMapLevelSetting();

	//Mipmap level 0
	float3 lookPos = float3(0.0f);
	if (followCam)
	{
		lookPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
		lookPos.y = 0.0f;
	}
	Camera* cam = CameraManager::Instance()->Push();
	cam->SetOrthoCamera(levelSetting[0], nearClip, farClip, shadowMapSize);
	cam->SetPosition(sunLight->GetPosition() + lookPos);
	cam->SetRotation(sunLight->GetRotation());
	cam->UpdateViewMatrix();
	output.mats[0] = cam->GetViewMatrix();
	output.mats[1] = cam->GetProjectionMatrix();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint shaderProgram = ShaderManager::Instance()->GetShaderProgram(shaderProgramIndex);
	glUseProgram(shaderProgram);

	GLint location = glGetUniformLocation(shaderProgram, "depthClampPara");
	glUniform2f(location, depthClampPara.x, depthClampPara.z);
	location = glGetUniformLocation(shaderProgram, "depthOnly");
	glUniform1i(location, 0);

	SceneManager::Instance()->DrawScene(shaderProgram);

	//Mipmap level 1
	cam->SetOrthoCamera(levelSetting[1], nearClip, farClip, shadowMapSize);
	output.mats[2] = cam->GetProjectionMatrix();

	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	location = glGetUniformLocation(shaderProgram, "depthOnly");
	glUniform1i(location, 1);

	SceneManager::Instance()->DrawScene(shaderProgram);

	//Mipmap level 2
	cam->SetOrthoCamera(levelSetting[2], nearClip, farClip, shadowMapSize);
	output.mats[3] = cam->GetProjectionMatrix();

	glDrawBuffer(GL_COLOR_ATTACHMENT5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SceneManager::Instance()->DrawScene(shaderProgram);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w, size.h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CameraManager::Instance()->Pop();
}

void SMPass::Init()
{
	followCam = true;
	shaderProgramIndex = ShaderManager::Instance()->LoadShader("SM.vert", "SM.frag");
}
