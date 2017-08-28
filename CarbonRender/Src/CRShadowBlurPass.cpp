#include "..\Inc\CRShadowBlurPass.h"

void ShadowBlurPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint sRt0;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	tempRt = SetGLRenderTexture(size.w, size.h, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0);
	sRt0 = SetGLRenderTexture(size.w, size.h, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = sRt0;
}

void ShadowBlurPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//blur x
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "steps");
	glUniform4f(location, step / size.w, 0.0f, step, 0.0f);

	DrawFullScreenQuad();
	/**/
	//blur y
	drawBuffers[0] = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(1, drawBuffers);

	glActiveTexture(GL_TEXTURE1 + input->cout);
	glBindTexture(GL_TEXTURE_2D, tempRt);

	location = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "steps");
	glUniform4f(location, 0.0f, step / size.h, 0.0f, step);

	DrawFullScreenQuad();
	/**/
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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
	step = 2.0f;
}
