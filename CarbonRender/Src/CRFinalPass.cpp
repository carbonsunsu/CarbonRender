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

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}

	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "skyRT");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "albedoMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "normalMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "wsPMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "tMap");
	glUniform1i(location, 5);

	DrawFullScreenQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}