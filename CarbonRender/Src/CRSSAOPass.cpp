#include "..\Inc\CRSSAOPass.h"

void SSAOPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	GLuint aoRt = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_COLOR_ATTACHMENT0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = aoRt;
}

void SSAOPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}
	glActiveTexture(GL_TEXTURE1 + input->cout);
	glBindTexture(GL_TEXTURE_2D, rnmTex);

	GLuint shaderProgram = ShaderManager::Instance()->GetShaderProgram(shaderProgramIndex);
	glUseProgram(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "sMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "rnMap");
	glUniform1i(location, 5);

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	float3 para = CameraManager::Instance()->GetCurrentCamera()->GetCameraPara();
	location = glGetUniformLocation(shaderProgram, "scalePara");
	glUniform3f(location, 1.0f / size.w, 1.0f / size.h, para.z - para.y);
	location = glGetUniformLocation(shaderProgram, "viewMat");
	Matrix3x3 normalMat = CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix();
	glUniformMatrix3fv(location, 1, GL_FALSE, normalMat.matrix);

	DrawFullScreenQuad();

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE1 + input->cout - 1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SSAOPass::Init()
{
	shaderProgramIndex = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "SSAO.frag");
	rnmTex = TextureManager::Instance()->LoadTexture("rnm");
}
