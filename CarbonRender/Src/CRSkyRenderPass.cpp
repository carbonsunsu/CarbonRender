#include "..\Inc\CRSkyRenderPass.h"

void SkyRenderPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("Atmosphere.vert", "Atmosphere.frag");
	FbxImportManager::Instance()->ImportFbxModel("sphere", &sphere);
	sphere.GetReady4Rending();
	sphere.SetScale(float3(10000.0f));
	sphere.SetRotation(float3(45.0f, 90.0f, 45.0f));
	milkwayTex = TextureManager::Instance()->LoadTexture("Milkway");
}

void SkyRenderPass::GetReady4Render(PassOutput* input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt0, cubeRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	rt0 = SetGLRenderTexture(size.w, size.h, GL_RGB32F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glGenFramebuffers(1, &fboCube);
	glBindFramebuffer(GL_FRAMEBUFFER, fboCube);
	cubeRt = SetGLCubeRenderTexture(cubeSize, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 2;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = rt0;
	output.RTS[1] = cubeRt;
}

void SkyRenderPass::Render(PassOutput* input)
{
	sphere.SetPosition(CameraManager::Instance()->GetCurrentCamera()->GetPosition());
	float3 para = CameraManager::Instance()->GetCurrentCamera()->GetCameraPara();
	Camera cam;
	cam.SetPerspectiveCamera(para.x, 1000.0f, 20000.0f);
	cam.SetPosition(CameraManager::Instance()->GetCurrentCamera()->GetPosition());
	cam.SetRotation(CameraManager::Instance()->GetCurrentCamera()->GetRotation());
	cam.UpdateViewMatrix();
	CameraManager::Instance()->Push(cam);
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w, size.h);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	
	ShaderManager::Instance()->UseShader(shaderProgram);
	float* shaderParas = WeatherSystem::Instance()->GetShaderParas();
	GLuint location = glGetUniformLocation(shaderProgram, "t");
	glUniform1f(location, shaderParas[0]);
	location = glGetUniformLocation(shaderProgram, "exposure");
	glUniform1f(location, shaderParas[1]);
	location = glGetUniformLocation(shaderProgram, "thetaS");
	glUniform1f(location, shaderParas[2]);
	location = glGetUniformLocation(shaderProgram, "wsSunPos");
	glUniform4f(location, shaderParas[3], shaderParas[4], shaderParas[5], shaderParas[6]);
	location = glGetUniformLocation(shaderProgram, "zenith");
	glUniform3f(location, shaderParas[7], shaderParas[8], shaderParas[9]);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, milkwayTex);
	location = glGetUniformLocation(shaderProgram, "milkwayTex");
	glUniform1i(location, 4);

	sphere.Render(shaderProgram);

	//render cube
	Camera camCube;
	camCube.SetPerspectiveCamera(90.0f, 1000.0f, 20000.0f, cubeSize);
	camCube.SetPosition(CameraManager::Instance()->GetCurrentCamera()->GetPosition());
	camCube.SetRotation(CameraManager::Instance()->GetCurrentCamera()->GetRotation());
	camCube.UpdateViewMatrix();
	CameraManager::Instance()->Push(camCube);
	glViewport(0, 0, cubeSize, cubeSize);

	float3 rs[6] = { float3(180.0f, -90.0f, 0.0f),
					float3(180.0f, 90.0f, 0.0f),
					float3(-90.0f, 0.0f, 0.0f),
					float3(90.0f, 0.0f, 0.0f) ,
					float3(0.0f, 180.0f, 180.0f) ,
					float3(0.0f, 0.0f, 180.0f) };

	glBindFramebuffer(GL_FRAMEBUFFER, fboCube);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(1, drawBuffers);
	for (int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, output.RTS[1], 0);
		CameraManager::Instance()->GetCurrentCamera()->SetRotation(rs[i]);
		CameraManager::Instance()->GetCurrentCamera()->UpdateViewMatrix();
		sphere.Render(shaderProgram);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, output.RTS[1]);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glViewport(0, 0, size.w, size.h);
	CameraManager::Instance()->Pop();

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	CameraManager::Instance()->Pop();
}

