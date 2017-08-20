#include "..\Inc\CRFinalPass.h"

void FinalPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("FinalPass.vert", "FinalPass.frag");
}

void FinalPass::GetReady4Render(PassOutput* input)
{
}

void FinalPass::Render(PassOutput* input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, input->RTS[0]);

	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "skyRT");
	glUniform1i(location, 1);

	DrawFullScreenQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}