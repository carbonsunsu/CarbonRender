#include "..\Inc\CRVolumetricCloudPass.h"

void VolumetricCloudPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint couldRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	couldRt = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT0);
	dBuffer = GLHelper::SetGLDepthBuffer(size.w, size.h);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = couldRt;
}

void VolumetricCloudPass::Render(PassOutput * input)
{
	Camera cam;
	Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
	cam.SetPerspectiveCamera(curCam->GetCameraPara().x, 1.0f, 100.0f);
	cam.SetPosition(curCam->GetPosition());
	cam.SetRotation(curCam->GetRotation());
	cam.UpdateViewMatrix();
	CameraManager::Instance()->Push(cam);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, noises[0]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, noises[1]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, noises[2]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, weatherData);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE5 + i);
		glBindTexture(GL_TEXTURE_2D, input->RTS[i]);
	}

	Light* sun = LightManager::Instance()->GetLight(0);
	float3 camPos = cam.GetPosition();
	float3 camRot = cam.GetRotation();
	float3 sunColor = sun->GetColor();
	float4 zenithColor = LightManager::Instance()->GetZenithColor();
	float3 sunPos = sun->GetPosition();
	float3 cloudBias = WeatherSystem::Instance()->GetCloudBias();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "perlinWorleyMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "worleyMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "curlMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "weatherMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "depthMap");
	glUniform1i(location, 5);

	location = glGetUniformLocation(shaderProgram, "sunColor");
	glUniform3f(location, sunColor.x, sunColor.y, sunColor.z);
	location = glGetUniformLocation(shaderProgram, "zenithColor");
	glUniform3f(location, zenithColor.x, zenithColor.y, zenithColor.z);
	

	location = glGetUniformLocation(shaderProgram, "wsCamPos");
	glUniform3f(location, camPos.x, camPos.y, camPos.z);
	location = glGetUniformLocation(shaderProgram, "wsSunPos");
	glUniform3f(location, sunPos.x, sunPos.y, sunPos.z);

	location = glGetUniformLocation(shaderProgram, "cloudBias");
	glUniform3f(location, cloudBias.x, cloudBias.y, cloudBias.z);

	location = glGetUniformLocation(shaderProgram, "CoverageFactor");
	glUniform1f(location, WeatherSystem::Instance()->GetCloudCoverage());
	cout << WeatherSystem::Instance()->GetCloudCoverage() << endl;

	cloudBox.SetPosition(camPos);
	cloudBox.SetScale(float3(20.0f, 20.0f, 20.0f));
	cloudBox.SetRotation(camRot);
	MeshObject* cloudBoxMesh = (MeshObject*)cloudBox.GetFirstChild();
	cloudBoxMesh->Render(shaderProgram, false);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < input->cout; i++)
	{
		glActiveTexture(GL_TEXTURE5 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	CameraManager::Instance()->Pop();
}

void VolumetricCloudPass::Init()
{
	FbxImportManager::Instance()->ImportFbxModel("Box", &cloudBox);
	shaderProgram = ShaderManager::Instance()->LoadShader("VolumatricCloud.vert", "VolumatricCloud.frag");
	GenerateTex();
}

void VolumetricCloudPass::GenerateTex()
{
	GLubyte* perlinWorley = new GLubyte[128 * 128 * 128 * 4];
	GLubyte* worley = new GLubyte[32 * 32 * 32 * 3];
	GLubyte* curl = new GLubyte[128 * 128 * 3];
	
	bool fileExist = true;
	fstream pwNoiseFile;
	pwNoiseFile.open("Resources\\Textures\\PerlinWorley.noise", ios::in | ios::binary);
	if (!pwNoiseFile)
	{
		fileExist = false;
		pwNoiseFile.open("Resources\\Textures\\PerlinWorley.noise", ios::out | ios::binary);
	}
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 128; j++)
			for (int k = 0; k < 128; k++)
			{
				float3 uv = float3(i * 0.0078125f, j * 0.0078125f, k * 0.0078125f);
				if (fileExist)
				{
					float noise;
					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 0] = (GLubyte)noise;

					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 1] = (GLubyte)noise;

					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 2] = (GLubyte)noise;

					pwNoiseFile.read((char*)&noise, sizeof(float));
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 3] = (GLubyte)noise;
				}
				else
				{
					float cellCount = 16.0f;
					float worleyNoise0 = Noise::WorleyNoise(uv, cellCount * 2.0f);
					float worleyNoise1 = Noise::WorleyNoise(uv, cellCount * 4.0f);
					float worleyNoise2 = Noise::WorleyNoise(uv, cellCount * 8.0f);
					float worleyNoise3 = Noise::WorleyNoise(uv, cellCount * 14.0f);
					float worleyNoise4 = Noise::WorleyNoise(uv, cellCount * 16.0f);
					float worleyNoise5 = Noise::WorleyNoise(uv, cellCount * 32.0f);
					
					float worleyFbm = worleyNoise0 * 0.625f + worleyNoise2 * 0.25f + worleyNoise3 * 0.125f;
					float noise = Math::Remap(Noise::PerlinFbm(uv, 8.0f, 3), 0.0f, 1.0f, worleyFbm, 1.0f) * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 0] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise0 * 0.625f + worleyNoise1 * 0.25f + worleyNoise2 * 0.125f;
					noise = worleyFbm * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 1] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise1 * 0.625f + worleyNoise2 * 0.25f + worleyNoise3 * 0.125f;
					noise = worleyFbm * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 2] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise2 * 0.625f + worleyNoise4 * 0.25f + worleyNoise5 * 0.125f;
					noise = worleyFbm * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 3] = (GLubyte)noise;
					pwNoiseFile.write((char*)&noise, sizeof(float));
				}
			}
	pwNoiseFile.close();

	fileExist = true;
	fstream wNoiseFile;
	wNoiseFile.open("Resources\\Textures\\Worley.noise", ios::in | ios::binary);
	if (!wNoiseFile)
	{
		fileExist = false;
		wNoiseFile.open("Resources\\Textures\\Worley.noise", ios::out | ios::binary);
	}
	for (int i = 0; i < 32; i++)
		for (int j = 0; j < 32; j++)
			for (int k = 0; k < 32; k++)
			{
				float3 uv = float3(i * 0.03125f, j * 0.03125f, k * 0.03125f);
				if (fileExist)
				{
					float noise;
					wNoiseFile.read((char*)&noise, sizeof(float));
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 0] = (GLubyte)noise;

					wNoiseFile.read((char*)&noise, sizeof(float));
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 1] = (GLubyte)noise;

					wNoiseFile.read((char*)&noise, sizeof(float));
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 2] = (GLubyte)noise;
				}
				else
				{
					float cellCount = 8.0f;
					float worleyNoise0 = Noise::WorleyNoise(uv, cellCount * 1.0f);
					float worleyNoise1 = Noise::WorleyNoise(uv, cellCount * 2.0f);
					float worleyNoise2 = Noise::WorleyNoise(uv, cellCount * 4.0f);
					float worleyNoise3 = Noise::WorleyNoise(uv, cellCount * 8.0f);
					float worleyNoise4 = Noise::WorleyNoise(uv, cellCount * 16.0f);

					float worleyFbm = worleyNoise0 * 0.625f + worleyNoise1 * 0.25f + worleyNoise2 * 0.125f;
					float noise = worleyFbm * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 0] = (GLubyte)noise;
					wNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise1 * 0.625f + worleyNoise2 * 0.25f + worleyNoise3 * 0.125f;
					noise = worleyFbm * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 1] = (GLubyte)noise;
					wNoiseFile.write((char*)&noise, sizeof(float));

					worleyFbm = worleyNoise2 * 0.625f + worleyNoise3 * 0.25f + worleyNoise4 * 0.125f;
					noise = worleyFbm * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 2] = (GLubyte)noise;
					wNoiseFile.write((char*)&noise, sizeof(float));
				}
			}
	wNoiseFile.close();

	glGenTextures(3, noises);
	glBindTexture(GL_TEXTURE_3D, noises[0]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, 128, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, perlinWorley);
	glGenerateMipmap(GL_TEXTURE_3D);

	glBindTexture(GL_TEXTURE_3D, noises[1]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, 32, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, worley);
	glGenerateMipmap(GL_TEXTURE_3D);

	noises[2] = TextureManager::Instance()->LoadTexture("CurlNoise");

	weatherData = TextureManager::Instance()->LoadTexture("Weather1");
}