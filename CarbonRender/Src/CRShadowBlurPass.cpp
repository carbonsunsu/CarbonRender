#include "..\Inc\CRShadowBlurPass.h"

void ShadowBlurPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	GLHelper::SetGLRenderTexture(GLHelper::GetSharedRT(), GL_COLOR_ATTACHMENT0);
	GLHelper::SetGLRenderTexture(input->RTS[0], GL_COLOR_ATTACHMENT1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = input->RTS[0];
}

void ShadowBlurPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//blur x
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "stepUnit");
	glUniform3f(location, stepSize / size.w, 0.0f, stepSize);
	location = glGetUniformLocation(shaderProgram, "depthClampPara");
	Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
	float3 camPara = curCam->GetCameraPara();
	glUniform2f(location, camPara.y, 1.0f / camPara.z);

	DrawFullScreenQuad();

	//blur y
	glDrawBuffer(GL_COLOR_ATTACHMENT1);

	glActiveTexture(GL_TEXTURE1 + input->cout);
	glBindTexture(GL_TEXTURE_2D, GLHelper::GetSharedRT());

	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(location, 1 + input->cout);
	location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "stepUnit");
	glUniform3f(location, 0.0f, stepSize / size.h, stepSize);

	DrawFullScreenQuad();

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE1 + input->cout);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowBlurPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "ShadowBlur.frag");
	stepSize = 1.0f;
}
