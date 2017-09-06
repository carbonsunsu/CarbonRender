#include "..\Inc\CRGIPass.h"

void GIPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint giRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	giRt = SetGLRenderTexture(size.w * targetScale, size.h * targetScale, GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = giRt;
}

void GIPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w * targetScale, size.h * targetScale);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
		if (i == 3 || i == 2 || i == 4)glGenerateMipmap(GL_TEXTURE_2D);
	}

	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "vplPMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "vplAMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "vplNMap");
	glUniform1i(location, 5);

	location = glGetUniformLocation(shaderProgram, "smMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[0].matrix);

	location = glGetUniformLocation(shaderProgram, "scale");
	glUniform2f(location, 1.0f / size.w, 1.0f / size.h);

	DrawFullScreenQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, size.w, size.h);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GIPass::Init()
{
	targetScale = 0.5f;
	shaderProgram = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "GIPass.frag");
}
