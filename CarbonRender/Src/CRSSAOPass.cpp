#include "..\Inc\CRSSAOPass.h"

void SSAOPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	rt = SetGLRenderTexture(size.w, size.h, GL_RGB, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = rt;
}

void SSAOPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}
	glActiveTexture(GL_TEXTURE1 + input->cout);
	glBindTexture(GL_TEXTURE_2D, rnmTex);

	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "sMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "rnMap");
	glUniform1i(location, 4);

	float3 wsCamPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
	float3 para = CameraManager::Instance()->GetCurrentCamera()->GetCameraPara();
	location = glGetUniformLocation(shaderProgram, "scalePara");
	glUniform1f(location, para.z - para.y);

	DrawFullScreenQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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
	shaderProgram = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "SSAO.frag");
	rnmTex = TextureManager::Instance()->LoadTexture("rnm");
}
