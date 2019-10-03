#include "..\Inc\CRSMPass.h"

void SMPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint posDepth, vplAlbedo, vplNormal;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	posDepth = GLHelper::SetGLRenderTexture(size.w * shadowMapScale, size.h * shadowMapScale, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR, GL_COLOR_ATTACHMENT0, true);
	vplAlbedo = GLHelper::SetGLRenderTexture(size.w * shadowMapScale, size.h * shadowMapScale, GL_RGB32F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT1, false);
	vplNormal = GLHelper::SetGLRenderTexture(size.w * shadowMapScale, size.h * shadowMapScale, GL_RGB32F, GL_RGB, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR, GL_COLOR_ATTACHMENT2, true);

	dBuffer = GLHelper::SetGLDepthBuffer(size.w * shadowMapScale, size.h * shadowMapScale);

	GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 3;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = posDepth;
	output.RTS[1] = vplAlbedo;
	output.RTS[2] = vplNormal;
	output.mats = new Matrix4x4[2];
}

void SMPass::Render(PassOutput * input)
{
	Light* sun = LightManager::Instance()->GetLight(0);
	float farClip = sun->GetPosition().Length() * 1.5f;
	float nearClip = 1.0f;
	sun->SetNearClip(nearClip);
	sun->SetFarClip(farClip);

	float3 lookPos = followCam ? CameraManager::Instance()->GetCurrentCamera()->GetPosition() : float3(0.0f);
	Camera cam;
	cam.SetOrthoCamera(30.0f, nearClip, farClip);
	//cam.SetPerspectiveCamera(3.0f, nearClip, farClip);
	cam.SetPosition(sun->GetPosition() + lookPos);
	cam.SetRotation(sun->GetRotation());
	CameraManager::Instance()->Push(cam);
	CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();
	output.mats[0] = CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix();
	output.mats[1] = CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w * shadowMapScale, size.h * shadowMapScale);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderManager::Instance()->UseShader(shaderProgram);

	GLint location = glGetUniformLocation(shaderProgram, "depthClampPara");
	glUniform2f(location, nearClip, 1.0f / (farClip - nearClip));

	SceneManager::Instance()->DrawScene(shaderProgram);

	glBindTexture(GL_TEXTURE_2D, output.RTS[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, output.RTS[2]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glViewport(0, 0, size.w, size.h);

	CameraManager::Instance()->Pop();
}

void SMPass::Init()
{
	shadowMapScale = 2;
	followCam = false;
	shaderProgram = ShaderManager::Instance()->LoadShader("SM.vert", "SM.frag");
}
