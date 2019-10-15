#include "..\Inc\CRShadowPass.h"

void ShadowPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint sRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	sRt = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGB, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

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

	int shadowMapSize = LightManager::Instance()->GetLight(0)->GetShadowMapSize();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "smMapLv0");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "smMapLv1");
	glUniform1i(location, 5);
	location = glGetUniformLocation(shaderProgram, "smMapLv2");
	glUniform1i(location, 6);
	location = glGetUniformLocation(shaderProgram, "stepUnit");
	glUniform2f(location, 1.0f / shadowMapSize, 1.0f / shadowMapSize);
	location = glGetUniformLocation(shaderProgram, "depthClampPara");
	Light* sun = LightManager::Instance()->GetLight(0);
	float nearClip = sun->GetNearClip();
	float farClip = sun->GetFarClip();
	glUniform2f(location, nearClip, 1.0f / (farClip - nearClip));
	location = glGetUniformLocation(shaderProgram, "lightPos");
	float3 sunPos = sun->GetPosition();
	glUniform3f(location, sunPos.x, sunPos.y, sunPos.z);
	location = glGetUniformLocation(shaderProgram, "lightSize");
	glUniform1f(location, sun->GetLightSize());

	location = glGetUniformLocation(shaderProgram, "smViewMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[0].matrix);
	location = glGetUniformLocation(shaderProgram, "smProMatLv0");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[1].matrix);
	location = glGetUniformLocation(shaderProgram, "smProMatLv1");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[2].matrix);
	location = glGetUniformLocation(shaderProgram, "smProMatLv2");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[3].matrix);

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
