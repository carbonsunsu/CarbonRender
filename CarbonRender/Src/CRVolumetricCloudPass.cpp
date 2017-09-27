#include "..\Inc\CRVolumetricCloudPass.h"

void VolumetricCloudPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint couldRt;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	couldRt = SetGLRenderTexture(size.w, size.h, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0);
	dBuffer = SetGLDepthBuffer(size.w, size.h);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 1;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = couldRt;
}

void VolumetricCloudPass::Render(PassOutput * input)
{
	Camera cam;
	Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
	cam.SetPerspectiveCamera(curCam->GetCameraPara().x, 1.0f, 50000.0f);
	cam.SetPosition(curCam->GetPosition());
	cam.SetRotation(curCam->GetRotation());
	cam.UpdateViewMatrix();
	CameraManager::Instance()->Push(cam);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, noises[0]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, noises[1]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, noises[2]);

	float3 camPos = CameraManager::Instance()->GetCurrentCamera()->GetPosition();
	float4 sunColor = WeatherSystem::Instance()->GetSunColor();
	float4 zenithColor = WeatherSystem::Instance()->GetSkyUpColor();
	ShaderManager::Instance()->UseShader(shaderProgram);
	GLint location = glGetUniformLocation(shaderProgram, "perlinWorleyMap");
	glUniform1i(location, 1);
	location = glGetUniformLocation(shaderProgram, "wsCamPos");
	glUniform3f(location, camPos.x, camPos.y, camPos.z);
	location = glGetUniformLocation(shaderProgram, "cloudBoxSize");
	glUniform4f(location, cloudBoxScale.x, cloudBoxScale.y, cloudBoxScale.z, cloudBoxScale.w);
	location = glGetUniformLocation(shaderProgram, "sunColor");
	glUniform3f(location, sunColor.x, sunColor.y, sunColor.z);
	location = glGetUniformLocation(shaderProgram, "zenithColor");
	glUniform3f(location, zenithColor.x, zenithColor.y, zenithColor.z);

	cloudBox.Render(shaderProgram, false);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	CameraManager::Instance()->Pop();
}

void VolumetricCloudPass::Init()
{
	cloudBoxScale = float4(20000.0f, 3750.0f, 20000.0f, 5250.0f);
	FbxImportManager::Instance()->ImportFbxModel("Box", &cloudBox);
	cloudBox.GetReady4Rending();
	cloudBox.SetPosition(float3(0.0f, cloudBoxScale.w, 0.0f));
	cloudBox.SetScale(float3(cloudBoxScale.x, cloudBoxScale.y, cloudBoxScale.z));
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
	pwNoiseFile.open("Resources\\Textures\\PerlinWorley.noise", ios::in);
	if (!pwNoiseFile)
	{
		fileExist = false;
		pwNoiseFile.open("Resources\\Textures\\PerlinWorley.noise", ios::out);
	}
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 128; j++)
			for (int k = 0; k < 128; k++)
			{
				float3 uv = float3(i / 128.0f, j / 128.0f, k / 128.0f);
				if (fileExist)
				{
					float noise;
					pwNoiseFile >> noise;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 0] = (GLubyte)noise;

					pwNoiseFile >> noise;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 1] = (GLubyte)noise;

					pwNoiseFile >> noise;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 2] = (GLubyte)noise;

					pwNoiseFile >> noise;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 3] = (GLubyte)noise;
				}
				else
				{
					double noise = (WorleyNoiseFast(uv * 10.0f) + Fbm(uv * 5.0f)) * 0.5f * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 0] = (GLubyte)noise;
					pwNoiseFile << noise << endl;

					noise = WorleyNoiseFast(uv * 10.0f) * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 1] = (GLubyte)noise;
					pwNoiseFile << noise << endl;

					noise = WorleyNoiseFast(uv * 20.0f) * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 2] = (GLubyte)noise;
					pwNoiseFile << noise << endl;

					noise = WorleyNoiseFast(uv * 40.0f) * 255;
					perlinWorley[i * 128 * 128 * 4 + j * 128 * 4 + k * 4 + 3] = (GLubyte)noise;
					pwNoiseFile << noise << endl;
				}
			}
	pwNoiseFile.close();

	fstream wNoiseFile;
	wNoiseFile.open("Resources\\Textures\\Worley.noise", ios::in);
	if (!wNoiseFile)
	{
		fileExist = false;
		wNoiseFile.open("Resources\\Textures\\Worley.noise", ios::out);
	}
	for (int i = 0; i < 32; i++)
		for (int j = 0; j < 32; j++)
			for (int k = 0; k < 32; k++)
			{
				float3 uv = float3(i / 32.0f, j / 32.0f, k / 32.0f);
				if (fileExist)
				{
					float noise;
					wNoiseFile >> noise;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 0] = (GLubyte)noise;

					wNoiseFile >> noise;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 1] = (GLubyte)noise;

					wNoiseFile >> noise;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 2] = (GLubyte)noise;
				}
				else
				{
					double noise = WorleyNoiseFast(uv * 10.0f) * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 0] = (GLubyte)noise;
					wNoiseFile << noise << endl;

					noise = WorleyNoiseFast(uv * 20.0f) * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 1] = (GLubyte)noise;
					wNoiseFile << noise << endl;

					noise = WorleyNoiseFast(uv * 40.0f) * 255;
					worley[i * 32 * 32 * 3 + j * 32 * 3 + k * 3 + 2] = (GLubyte)noise;
					wNoiseFile << noise << endl;
				}
			}
	wNoiseFile.close();

	fstream cNoiseFile;
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 128; j++)
		{
			float3 uv = float3(i / 32.0f, j / 32.0f, 0.0f);
		}

	glGenTextures(3, noises);
	glBindTexture(GL_TEXTURE_3D, noises[0]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, 128, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, perlinWorley);

	glBindTexture(GL_TEXTURE_3D, noises[1]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, 32, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, worley);


}

float3 VolumetricCloudPass::PerlinFade(float3 a)
{
	return a * a * a * (a * (a * 6.0f - 15.0f) + 10.0f);
}

float VolumetricCloudPass::PerlinGrab(int a, float3 b)
{
	return Dot(b, grab[a%16]);
}

float VolumetricCloudPass::PerlinPermute(int a)
{
	return perm[a%256];
}

float VolumetricCloudPass::PerlinNoise(float3 uv)
{
	float3 P = Mod(Floor(uv), 256.0f);
	uv = uv - Floor(uv);
	float3 f = PerlinFade(uv);

	float A = PerlinPermute(P.x) + P.y;
	float AA = PerlinPermute(A) + P.z;
	float AB = PerlinPermute(A + 1) + P.z;
	float B = PerlinPermute(P.x + 1) + P.y;
	float BA = PerlinPermute(B) + P.z;
	float BB = PerlinPermute(B + 1) + P.z;

	float n = Lerp(
		Lerp(Lerp(PerlinGrab(PerlinPermute(AA), uv),
			PerlinGrab(PerlinPermute(BA), uv + float3(-1.0f, 0.0f, 0.0f)), f.x),
			Lerp(PerlinGrab(PerlinPermute(AB), uv + float3(0.0f, -1.0f, 0.0f)),
				PerlinGrab(PerlinPermute(BB), uv + float3(-1.0f, -1.0f, 0.0f)), f.x), f.y),
		Lerp(Lerp(PerlinGrab(PerlinPermute(AA + 1), uv + float3(0.0f, 0.0f, -1.0f)),
			PerlinGrab(PerlinPermute(BA + 1), uv + float3(-1.0f, 0.0f, -1.0f)), f.x),
			Lerp(PerlinGrab(PerlinPermute(AB + 1), uv + float3(0.0f, -1.0f, -1.0f)),
				PerlinGrab(PerlinPermute(BB + 1), uv + float3(-1.0f, -1.0f, -1.0f)), f.x), f.y),
		f.z);

	return n * 0.5f + 0.5f;
}

float VolumetricCloudPass::Fbm(float3 uv)
{
	float fbm = PerlinNoise(uv * 5.0f) +
		0.5f * PerlinNoise(uv * 10.0f) +
		0.25f * PerlinNoise(uv * 20.0f) +
		0.125f * PerlinNoise(uv * 40.0f) +
		0.0625f * PerlinNoise(uv * 80.0f);
	return fbm * 0.5f;
}

float3 VolumetricCloudPass::WorleyPermute(float3 a)
{
	return Mod(a * a * 34.0f + a, 289.0f);
}

float4 VolumetricCloudPass::WorleyPermute(float4 a)
{
	return Mod(a * a * 34.0f + a, 289.0f);
}

float3 VolumetricCloudPass::Length(float3 a, float3 b, float3 c)
{
	return a * a + b * b + c * c;
}

float4 VolumetricCloudPass::Length(float4 a, float4 b, float4 c)
{
	return a * a + b * b + c * c;
}

float VolumetricCloudPass::WorleyNoise(float3 uv)
{
	const float jitter = 0.9f;
	const float K = 0.142857f; // 1/7
	const float Ko = 0.428571f; // 1/2-K/2
	const float K2 = 0.020408f; // 1/(7*7)
	const float Kz = 0.166667f; // 1/6
	const float Kzo = 0.416667f; // 1/2-1/6*2

	float3 Pi = Mod(Floor(uv), 289.0f);
	float3 Pf = Fract(uv) - 0.5;

	float3 Pfx = Pf.x + float3(1.0f, 0.0f, -1.0f);
	float3 Pfy = Pf.y + float3(1.0f, 0.0f, -1.0f);
	float3 Pfz = Pf.z + float3(1.0f, 0.0f, -1.0f);

	float3 p = WorleyPermute(Pi.x + float3(-1.0f, 0.0f, 1.0f));
	float3 p1 = WorleyPermute(p + Pi.y - 1.0f);
	float3 p2 = WorleyPermute(p + Pi.y);
	float3 p3 = WorleyPermute(p + Pi.y + 1.0f);

	float3 p11 = WorleyPermute(p1 + Pi.z - 1.0f);
	float3 p12 = WorleyPermute(p1 + Pi.z);
	float3 p13 = WorleyPermute(p1 + Pi.z + 1.0f);

	float3 p21 = WorleyPermute(p2 + Pi.z - 1.0f);
	float3 p22 = WorleyPermute(p2 + Pi.z);
	float3 p23 = WorleyPermute(p2 + Pi.z + 1.0f);

	float3 p31 = WorleyPermute(p3 + Pi.z - 1.0f);
	float3 p32 = WorleyPermute(p3 + Pi.z);
	float3 p33 = WorleyPermute(p3 + Pi.z + 1.0f);

	float3 ox11 = Fract(p11*K) - Ko;
	float3 oy11 = Mod(Floor(p11*K), 7.0f)*K - Ko;
	float3 oz11 = Floor(p11*K2)*Kz - Kzo;

	float3 ox12 = Fract(p12*K) - Ko;
	float3 oy12 = Mod(Floor(p12*K), 7.0f)*K - Ko;
	float3 oz12 = Floor(p12*K2)*Kz - Kzo;

	float3 ox13 = Fract(p13*K) - Ko;
	float3 oy13 = Mod(Floor(p13*K), 7.0f)*K - Ko;
	float3 oz13 = Floor(p13*K2)*Kz - Kzo;

	float3 ox21 = Fract(p21*K) - Ko;
	float3 oy21 = Mod(Floor(p21*K), 7.0f)*K - Ko;
	float3 oz21 = Floor(p21*K2)*Kz - Kzo;

	float3 ox22 = Fract(p22*K) - Ko;
	float3 oy22 = Mod(Floor(p22*K), 7.0f)*K - Ko;
	float3 oz22 = Floor(p22*K2)*Kz - Kzo;

	float3 ox23 = Fract(p23*K) - Ko;
	float3 oy23 = Mod(Floor(p23*K), 7.0f)*K - Ko;
	float3 oz23 = Floor(p23*K2)*Kz - Kzo;

	float3 ox31 = Fract(p31*K) - Ko;
	float3 oy31 = Mod(Floor(p31*K), 7.0f)*K - Ko;
	float3 oz31 = Floor(p31*K2)*Kz - Kzo;

	float3 ox32 = Fract(p32*K) - Ko;
	float3 oy32 = Mod(Floor(p32*K), 7.0f)*K - Ko;
	float3 oz32 = Floor(p32*K2)*Kz - Kzo;

	float3 ox33 = Fract(p33*K) - Ko;
	float3 oy33 = Mod(Floor(p33*K), 7.0f)*K - Ko;
	float3 oz33 = Floor(p33*K2)*Kz - Kzo;

	float3 dx11 = Pfx + jitter*ox11;
	float3 dy11 = Pfy.x + jitter*oy11;
	float3 dz11 = Pfz.x + jitter*oz11;

	float3 dx12 = Pfx + jitter*ox12;
	float3 dy12 = Pfy.x + jitter*oy12;
	float3 dz12 = Pfz.y + jitter*oz12;

	float3 dx13 = Pfx + jitter*ox13;
	float3 dy13 = Pfy.x + jitter*oy13;
	float3 dz13 = Pfz.z + jitter*oz13;

	float3 dx21 = Pfx + jitter*ox21;
	float3 dy21 = Pfy.y + jitter*oy21;
	float3 dz21 = Pfz.x + jitter*oz21;

	float3 dx22 = Pfx + jitter*ox22;
	float3 dy22 = Pfy.y + jitter*oy22;
	float3 dz22 = Pfz.y + jitter*oz22;

	float3 dx23 = Pfx + jitter*ox23;
	float3 dy23 = Pfy.y + jitter*oy23;
	float3 dz23 = Pfz.z + jitter*oz23;

	float3 dx31 = Pfx + jitter*ox31;
	float3 dy31 = Pfy.z + jitter*oy31;
	float3 dz31 = Pfz.x + jitter*oz31;

	float3 dx32 = Pfx + jitter*ox32;
	float3 dy32 = Pfy.z + jitter*oy32;
	float3 dz32 = Pfz.y + jitter*oz32;

	float3 dx33 = Pfx + jitter*ox33;
	float3 dy33 = Pfy.z + jitter*oy33;
	float3 dz33 = Pfz.z + jitter*oz33;

	float3 d11 = Length(dx11, dy11, dz11);
	float3 d12 = Length(dx12, dy12, dz12);
	float3 d13 = Length(dx13, dy13, dz13);
	float3 d21 = Length(dx21, dy21, dz21);
	float3 d22 = Length(dx22, dy22, dz22);
	float3 d23 = Length(dx23, dy23, dz23);
	float3 d31 = Length(dx31, dy31, dz31);
	float3 d32 = Length(dx32, dy32, dz32);
	float3 d33 = Length(dx33, dy33, dz33);

	float3 d1a = Min(d11, d12);
	d12 = Max(d11, d12);
	d11 = Min(d1a, d13);
	d13 = Max(d1a, d13);
	d12 = Min(d12, d13);
	float3 d2a = Min(d21, d22);
	d22 = Max(d21, d22);
	d21 = Min(d2a, d23);
	d23 = Max(d2a, d23);
	d22 = Min(d22, d23);
	float3 d3a = Min(d31, d32);
	d32 = Max(d31, d32);
	d31 = Min(d3a, d33);
	d33 = Max(d3a, d33);
	d32 = Min(d32, d33);
	float3 da = Min(d11, d21);
	d21 = Max(d11, d21);
	d11 = Min(da, d31);
	d31 = Max(da, d31);
	d11.x = (d11.x < d11.y) ? d11.x : d11.y;
	d11.y = (d11.x < d11.y) ? d11.y : d11.x;
	d11.x = (d11.x < d11.z) ? d11.x : d11.z;
	d11.z = (d11.x < d11.z) ? d11.z : d11.x;
	d12 = Min(d12, d21);
	d12 = Min(d12, d22);
	d12 = Min(d12, d31);
	d12 = Min(d12, d32);
	d11.y = fminf(d11.y, d12.x);
	d11.z = fminf(d11.z, d12.y);
	d11.y = fminf(d11.y, d12.z);
	d11.y = fminf(d11.y, d11.z);
	return 1.0f - sqrt(d11.x);
}

float VolumetricCloudPass::WorleyNoiseFast(float3 uv)
{
	const float jitter = 0.8f;
	const float K = 0.142857f; // 1/7
	const float Ko = 0.428571f; // 1/2-K/2
	const float K2 = 0.020408f; // 1/(7*7)
	const float Kz = 0.166667f; // 1/6
	const float Kzo = 0.416667f; // 1/2-1/6*2

	float3 Pi = Mod(Floor(uv), 289.0f);
	float3 Pf = Fract(uv);

	float4 Pfx = Pf.x + float4(0.0f, -1.0f, 0.0f, -1.0f);
	float4 Pfy = Pf.y + float4(0.0f, 0.0f, -1.0f, -1.0f);

	float4 p = WorleyPermute(float4(0.0f, 1.0f, 0.0f, 1.0f) + Pi.x);
	p = WorleyPermute(p + Pi.y + float4(0.0f, 0.0f, 1.0f, 1.0f));
	float4 p1 = WorleyPermute(p + Pi.z);
	float4 p2 = WorleyPermute(p + Pi.z + float4(1.0f));

	float4 ox1 = Fract(p1*K) - Ko;
	float4 oy1 = Mod(Floor(p1*K), 7.0f)*K - Ko;
	float4 oz1 = Floor(p1*K2)*Kz - Kzo;
	float4 ox2 = Fract(p2*K) - Ko;
	float4 oy2 = Mod(Floor(p2*K), 7.0f)*K - Ko;
	float4 oz2 = Floor(p2*K2)*Kz - Kzo;

	float4 dx1 = Pfx + jitter*ox1;
	float4 dy1 = Pfy + jitter*oy1;
	float4 dz1 = Pf.z + jitter*oz1;
	float4 dx2 = Pfx + jitter*ox2;
	float4 dy2 = Pfy + jitter*oy2;
	float4 dz2 = Pf.z - 1.0f + jitter*oz2;

	float4 d1 = Length(dx1, dy1, dz1);
	float4 d2 = Length(dx2, dy2, dz2);

	float4 d = Min(d1, d2);
	d2 = Max(d1, d2);

	d.x = (d.x < d.y) ? d.x : d.y;
	d.y = (d.x < d.y) ? d.y : d.x;
	d.x = (d.x < d.z) ? d.x : d.z;
	d.z = (d.x < d.z) ? d.z : d.x;
	d.x = (d.x < d.w) ? d.x : d.w;
	d.w = (d.x < d.w) ? d.w : d.x;
	d.y = fminf(d.y, d2.y);
	d.z = fminf(d.z, d2.z);
	d.w = fminf(d.w, d2.w);

	d.y = fminf(d.y, d.z);
	d.y = fminf(d.y, d.w);
	d.y = fminf(d.y, d2.x);

	return 1.0f - sqrt(d.x);
}

float3 VolumetricCloudPass::CurlNoise(float3 uv)
{
	return float3();
}
