#include "..\Inc\CRLightPass.h"

void LightPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint pureLRt, refRt, paraRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	pureLRt = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT0);
	refRt = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT1);
	paraRt = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT2);

	GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 3;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = pureLRt;
	output.RTS[1] = refRt;
	output.RTS[2] = paraRt;
}

void LightPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	for (int i = 0; i < input->cout - 1; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}
	glActiveTexture(GL_TEXTURE1 + input->cout - 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, input->RTS[input->cout - 1]);

	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "albedoMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "normalMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "sMap");
	glUniform1i(location, 5);
	location = glGetUniformLocation(shaderProgram, "giMap");
	glUniform1i(location, 6);
	location = glGetUniformLocation(shaderProgram, "cubeMap");
	glUniform1i(location, 7);
	
	unsigned int sunLightID = WeatherSystem::Instance()->GetSunLightID();
	Light* sunLight = LightManager::Instance()->GetLight(sunLightID);
	float3 sColor = sunLight->GetColor();
	float3 wsSunPos = sunLight->GetPosition();
	float3 wsCamPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
	location = glGetUniformLocation(shaderProgram, "sunColor");
	glUniform3f(location, sColor.x, sColor.y, sColor.z);
	location = glGetUniformLocation(shaderProgram, "wsSunPos");
	glUniform3f(location, wsSunPos.x, wsSunPos.y, wsSunPos.z);
	location = glGetUniformLocation(shaderProgram, "wsCamPos");
	glUniform3f(location, wsCamPos.x, wsCamPos.y, wsCamPos.z);

	DrawFullScreenQuad();

	for (int i = 0; i < input->cout - 1; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE1 + input->cout - 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void LightPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "LightPass.frag");
}
