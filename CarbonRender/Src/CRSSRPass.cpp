#include "..\Inc\CRSSRPass.h"

void SSRPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint reflectionRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	reflectionRt = SetGLRenderTexture(size.w * targetScale, size.h * targetScale, GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = reflectionRt;
}

void SSRPass::Render(PassOutput * input)
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
	}
	glActiveTexture(GL_TEXTURE1 + input->cout - 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, input->RTS[input->cout - 1]);

	float3 camPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "colorMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "nMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "pMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "paraMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "cubeMap");
	glUniform1i(location, input->cout);
	location = glGetUniformLocation(shaderProgram, "viewMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetViewMatrix().matrix);
	location = glGetUniformLocation(shaderProgram, "projectMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, CameraManager::Instance()->GetCurrentCamera()->GetProjectionMatrix().matrix);
	location = glGetUniformLocation(shaderProgram, "wsCamPos");
	glUniform4f(location, camPos.x, camPos.y, camPos.z, 1.0f);

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

void SSRPass::Init()
{
	targetScale = 0.5f;
	shaderProgram = ShaderManager::Instance()->LoadShader("ScreenQuad.vert", "SSR.frag");
}
