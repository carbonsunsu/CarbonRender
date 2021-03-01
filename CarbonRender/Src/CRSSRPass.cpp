#include "..\Inc\CRSSRPass.h"

void SSRPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint reflectionRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	reflectionRt = GLHelper::SetGLRenderTexture(size.w * targetScale, size.h * targetScale, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR, GL_COLOR_ATTACHMENT0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = reflectionRt;
}

void SSRPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w * targetScale, size.h * targetScale);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}

	float3 camPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
	GLuint shaderProgram = ShaderManager::Instance()->GetShaderProgram(shaderProgramIndex);
	glUseProgram(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "pureLightMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "refMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "paraMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 5);
	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 6);
	location = glGetUniformLocation(shaderProgram, "viewMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix().matrix);
	location = glGetUniformLocation(shaderProgram, "projectMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix().matrix);
	location = glGetUniformLocation(shaderProgram, "wsCamPos");
	glUniform4f(location, camPos.x, camPos.y, camPos.z, 1.0f);
	location = glGetUniformLocation(shaderProgram, "depthClampPara");
	Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
	float3 camPara = curCam->GetCameraPara();
	glUniform2f(location, camPara.y, 1.0f / camPara.z);

	DrawFullScreenQuad();

	glBindTexture(GL_TEXTURE_2D, output.RTS[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glViewport(0, 0, size.w, size.h);
}

void SSRPass::Init()
{
	targetScale = 0.5f;
	shaderProgramIndex = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "SSR.frag");
}
