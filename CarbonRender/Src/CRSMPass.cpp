#include "..\Inc\CRSMPass.h"

void SMPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint depthMapLv0, depthMapLv1, depthMapLv2, vplPos, vplAlbedo, vplNormal;
	int shadowMapSize = LightManager::Instance()->GetLight(0)->GetShadowMapSize();
	depthMapLv0 = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_R32F, GL_RED, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT0);
	vplPos = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_RGB32F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT1);
	vplAlbedo = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_RGB8, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT2);
	vplNormal = GLHelper::SetGLRenderTexture(shadowMapSize, shadowMapSize, GL_RGB32F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT3);
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
	Light* sun = LightManager::Instance()->GetLight(0);
	float farClip = sun->GetFarClip();
	float nearClip = sun->GetNearClip();
	int shadowMapSize = sun->GetShadowMapSize();

	//Mipmap level 0
	float3 lookPos = followCam ? CameraManager::Instance()->GetCurrentCamera()->GetPosition() : float3(0.0f);
	Camera cam;
	cam.SetOrthoCamera(10.0f, nearClip, farClip);
	//cam.SetPerspectiveCamera(3.0f, nearClip, farClip);
	cam.SetPosition(sun->GetPosition() + lookPos);
	cam.SetRotation(sun->GetRotation());
	CameraManager::Instance()->Push(cam);
	CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();
	output.mats[0] = CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix();
	output.mats[1] = CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderManager::Instance()->UseShader(shaderProgram);

	GLint location = glGetUniformLocation(shaderProgram, "depthClampPara");
	glUniform2f(location, nearClip, 1.0f / (farClip - nearClip));
	location = glGetUniformLocation(shaderProgram, "depthOnly");
	glUniform1i(location, 0);

	SceneManager::Instance()->DrawScene(shaderProgram);

	//Mipmap level 1
	CameraManager::Instance()->GetCurrentCamera()->SetOrthoCamera(50.0f, nearClip, farClip);
	output.mats[2] = CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix();

	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	location = glGetUniformLocation(shaderProgram, "depthOnly");
	glUniform1i(location, 1);

	SceneManager::Instance()->DrawScene(shaderProgram);

	//Mipmap level 2
	CameraManager::Instance()->GetCurrentCamera()->SetOrthoCamera(1000.0f, nearClip, farClip);
	output.mats[3] = CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix();

	glDrawBuffer(GL_COLOR_ATTACHMENT5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SceneManager::Instance()->DrawScene(shaderProgram);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w, size.h);

	CameraManager::Instance()->Pop();
}

void SMPass::Init()
{
	followCam = true;
	shaderProgram = ShaderManager::Instance()->LoadShader("SM.vert", "SM.frag");
}
