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
	glBindTexture(GL_TEXTURE_3D, WeatherSystem::Instance()->GetBaseNoise());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, WeatherSystem::Instance()->GetDetailNoise());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, WeatherSystem::Instance()->GetCurlNoise());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, WeatherSystem::Instance()->GetWeatherMapId());

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
	float3 fogColor = WeatherSystem::Instance()->GetFogColor();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "perlinWorleyMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "worleyMap");
	glUniform1i(location, 2);
	location = glGetUniformLocation(shaderProgram, "curlMap");
	glUniform1i(location, 3);
	location = glGetUniformLocation(shaderProgram, "weatherMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(shaderProgram, "posMap");
	glUniform1i(location, 5);
	location = glGetUniformLocation(shaderProgram, "stencilMap");
	glUniform1i(location, 6);
	location = glGetUniformLocation(shaderProgram, "smMapLv0");
	glUniform1i(location, 7);
	location = glGetUniformLocation(shaderProgram, "smMapLv1");
	glUniform1i(location, 8);

	location = glGetUniformLocation(shaderProgram, "sunColor");
	glUniform3f(location, sunColor.x, sunColor.y, sunColor.z);
	location = glGetUniformLocation(shaderProgram, "zenithColor");
	glUniform3f(location, zenithColor.x, zenithColor.y, zenithColor.z);

	location = glGetUniformLocation(shaderProgram, "wsCamPos");
	glUniform3f(location, camPos.x, camPos.y, camPos.z);
	location = glGetUniformLocation(shaderProgram, "wsSunPos");
	glUniform3f(location, sunPos.x, sunPos.y, sunPos.z);

	float cloudMaxAltitude = WeatherSystem::Instance()->GetCloudMaxAltitude();
	float cloudMinAltitude = WeatherSystem::Instance()->GetCloudMinAltitude();
	location = glGetUniformLocation(shaderProgram, "cloudMaxAltitude");
	glUniform1f(location, cloudMaxAltitude);
	location = glGetUniformLocation(shaderProgram, "cloudMinAltitude");
	glUniform1f(location, cloudMinAltitude);
	location = glGetUniformLocation(shaderProgram, "cloudHeightInv");
	glUniform1f(location, 1.0f / (cloudMaxAltitude - cloudMinAltitude));
	location = glGetUniformLocation(shaderProgram, "cloudBias");
	glUniform3f(location, cloudBias.x, cloudBias.y, cloudBias.z);
	location = glGetUniformLocation(shaderProgram, "cloudCoverageScale");
	glUniform1f(location, WeatherSystem::Instance()->GetCloudCoverage());
	location = glGetUniformLocation(shaderProgram, "cloudPrecipitationScale");
	glUniform1f(location, WeatherSystem::Instance()->GetCloudPrecipitation());
	
	location = glGetUniformLocation(shaderProgram, "fogDensity");
	glUniform1f(location, WeatherSystem::Instance()->GetFogDensity());
	location = glGetUniformLocation(shaderProgram, "fogColorScale");
	glUniform3f(location, fogColor.x, fogColor.y, fogColor.z);
	location = glGetUniformLocation(shaderProgram, "fogPrecipitaion");
	glUniform1f(location, WeatherSystem::Instance()->GetFogPrecipitation());
	location = glGetUniformLocation(shaderProgram, "fogMaxAltitude");
	glUniform1f(location, WeatherSystem::Instance()->GetFogMaxAltitude());

	location = glGetUniformLocation(shaderProgram, "smMat");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[0].matrix);
	location = glGetUniformLocation(shaderProgram, "smProMatLv0");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[1].matrix);
	location = glGetUniformLocation(shaderProgram, "smProMatLv1");
	glUniformMatrix4fv(location, 1, GL_FALSE, input->mats[2].matrix);

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
}