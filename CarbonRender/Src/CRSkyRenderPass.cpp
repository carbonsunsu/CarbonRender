#include "..\Inc\CRSkyRenderPass.h"

void SkyRenderPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("Atmosphere.vert", "Atmosphere.frag");
	FbxImportManager::Instance()->ImportFbxModel("sphere", &sphere);
	sphere.SetScale(float3(10000.0f, 5000.0f, 10000.0f));
	sphere.SetPosition(float3(0.0f));
	sphere.SetRotation(float3(45.0f, 135.0f, 0.0f));
	milkwayTex = TextureManager::Instance()->LoadTexture("Milkway");
}

void SkyRenderPass::GetReady4Render(PassOutput* input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt0, cubeRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	rt0 = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0, false);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glGenFramebuffers(1, &fboCube);
	glBindFramebuffer(GL_FRAMEBUFFER, fboCube);
	cubeRt = GLHelper::SetGLCubeRenderTexture(cubeSize, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 2;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = rt0;
	output.RTS[1] = cubeRt;
}

void SkyRenderPass::Render(PassOutput* input)
{
	float3 para = CameraManager::Instance()->GetCurrentCamera()->GetCameraPara();
	Camera cam;
	cam.SetPerspectiveCamera(para.x, 1000.0f, 20000.0f);
	cam.SetPosition(float3(0.0f));
	cam.SetRotation(CameraManager::Instance()->GetCurrentCamera()->GetRotation());
	cam.UpdateViewMatrix();
	CameraManager::Instance()->Push(cam);
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	glViewport(0, 0, size.w, size.h);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
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

	MeshObject* sphereMesh = (MeshObject*)sphere.GetFirstChild();
	sphereMesh->Render(shaderProgram);

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
		sphereMesh->Render(shaderProgram);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, output.RTS[1]);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glViewport(0, 0, size.w, size.h);
	CameraManager::Instance()->Pop();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	CameraManager::Instance()->Pop();
}

