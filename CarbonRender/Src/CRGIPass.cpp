#include "..\Inc\CRGIPass.h"

void GIPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint giRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	giRt = GLHelper::SetGLRenderTexture(size.w * targetScale, size.h * targetScale, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT0);
	denoiseRt = GLHelper::SetGLRenderTexture(size.w * targetScale, size.h * targetScale, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = giRt;
}

void GIPass::Render(PassOutput * input)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w * targetScale, size.h * targetScale);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}

	GLuint shaderProgram = ShaderManager::Instance()->GetShaderProgram(shaderProgramIndex);
	glUseProgram(shaderProgram);
	unsigned int sunLightID = WeatherSystem::Instance()->GetSunLightID();
	Light* sunLight = LightManager::Instance()->GetLight(sunLightID);
	float3 sunColor = sunLight->GetColor();
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
	location = glGetUniformLocation(shaderProgram, "stenMap");
	glUniform1i(location, 6);

	location = glGetUniformLocation(shaderProgram, "smViewMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[0].matrix);
	location = glGetUniformLocation(shaderProgram, "smProMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[1].matrix);

	int shadowMapSize = sunLight->GetShadowMapSize();
	location = glGetUniformLocation(shaderProgram, "stepUnit");
	glUniform2f(location, 1.0f / shadowMapSize, 1.0f / shadowMapSize);
	location = glGetUniformLocation(shaderProgram, "sunColor");
	glUniform3f(location, sunColor.x, sunColor.y, sunColor.z);

	DrawFullScreenQuad();

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	/**/ 
	//Denoise x 
	glDrawBuffer(GL_COLOR_ATTACHMENT1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, input->RTS[5]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, input->RTS[1]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, output.RTS[0]);
	

	GLuint denoiseShaderProgram = ShaderManager::Instance()->GetShaderProgram(denoiseShaderProgramIndex);
	glUseProgram(denoiseShaderProgram);
	location = glGetUniformLocation(denoiseShaderProgram, "stenMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(denoiseShaderProgram, "nMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(denoiseShaderProgram, "giMap");
	glUniform1i(location, 3);

	location = glGetUniformLocation(denoiseShaderProgram, "stepUnit");
	glUniform3f(location, denoiseStepSize / size.w, 0.0f, denoiseStepSize);

	DrawFullScreenQuad();

	//Denoise y
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, denoiseRt);

	location = glGetUniformLocation(denoiseShaderProgram, "stenMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(denoiseShaderProgram, "nMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(denoiseShaderProgram, "giMap");
	glUniform1i(location, 3);

	location = glGetUniformLocation(denoiseShaderProgram, "stepUnit");
	glUniform3f(location, 0.0f, denoiseStepSize / size.h, denoiseStepSize);

	DrawFullScreenQuad();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	/**/
	glViewport(0, 0, size.w, size.h);
}

void GIPass::Init()
{
	targetScale = 0.5f;
	denoiseStepSize = 1.0f;
	shaderProgramIndex = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "GIPass.frag");
	denoiseShaderProgramIndex = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "GIDenoise.frag");
}
