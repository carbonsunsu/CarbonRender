#include "..\Inc\CRLightPass.h"

void LightPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint lRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	lRt = SetGLRenderTexture(size.w, size.h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = lRt;
}

void LightPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

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
	location = glGetUniformLocation(shaderProgram, "sMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "cubeMap");
	glUniform1i(location, 5);
	
	float4 zColor = WeatherSystem::Instance()->GetSkyUpColor();
	float4 sColor = WeatherSystem::Instance()->GetSunColor();
	float3 wsSunPos = WeatherSystem::Instance()->GetWsSunPos();
	float3 wsCamPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
	location = glGetUniformLocation(shaderProgram, "zenithColor");
	glUniform4f(location, zColor.x, zColor.y, zColor.z, zColor.w);
	location = glGetUniformLocation(shaderProgram, "sunColor");
	glUniform4f(location, sColor.x, sColor.y, sColor.z, sColor.w);
	location = glGetUniformLocation(shaderProgram, "wsSunPos");
	glUniform3f(location, wsSunPos.x, wsSunPos.y, wsSunPos.z);
	location = glGetUniformLocation(shaderProgram, "wsCamPos");
	glUniform3f(location, wsCamPos.x, wsCamPos.y, wsCamPos.z);

	DrawFullScreenQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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
