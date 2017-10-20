#include "..\Inc\CRShadowPass.h"

void ShadowPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint sRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	sRt = SetGLRenderTexture(size.w, size.h, GL_RGB, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = sRt;
}

void ShadowPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "smMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "stepUnit");
	glUniform2f(location, 1.0f / size.w, 1.0f / size.h);

	location = glGetUniformLocation(shaderProgram, "smMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[0].matrix);

	DrawFullScreenQuad();

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ShadowPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "ShadowPass.frag");
}
