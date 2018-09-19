#include "..\Inc\CRSSAOPass.h"

void SSAOPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLHelper::SetGLRenderTexture(input->RTS[2], GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = input->RTS[2];
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

	ShaderManager::Instance()->UseShader(shaderProgram);
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
	shaderProgram = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "SSAO.frag");
	rnmTex = TextureManager::Instance()->LoadTexture("rnm");
}
