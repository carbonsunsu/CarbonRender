#include "..\Inc\CRRenderPassManager.h"

RenderPassManager* RenderPassManager::ins = nullptr;

RenderPassManager::RenderPassManager()
{
}

RenderPassManager::~RenderPassManager()
{
}

RenderPassManager * RenderPassManager::Instance()
{
	if (ins == nullptr)
		ins = new RenderPassManager();

	return ins;
}

void RenderPassManager::Init()
{
	skyPass.Init();
	finalPass.Init();
	gPass.Init();
	lightPass.Init();
	smPass.Init();
	shadowPass.Init();
	shadowBlurPass.Init();
}

void RenderPassManager::Draw()
{
	PassOutput* sm = smPass.Draw(NULL);//albedo and depth, vpl pos
	PassOutput* sky = skyPass.Draw(NULL);//sky
	PassOutput* g = gPass.Draw(NULL);//albedo, normal, position

	PassOutput sInput;
	sInput.cout = 2;
	sInput.RTS = new GLuint[sInput.cout];
	sInput.RTS[0] = g->RTS[2];
	sInput.RTS[1] = sm->RTS[0];
	sInput.mats = new Matrix4x4[1];
	sInput.mats[0] = sm->mats[0];
	PassOutput* shadow = shadowPass.Draw(&sInput);

	//add ao later


	PassOutput* shadowBlured = shadowBlurPass.Draw(shadow);


	PassOutput lInput;
	lInput.cout = g->cout + 2;
	lInput.RTS = new GLuint[lInput.cout];
	for (int i = 0; i < lInput.cout - 1; i++)
		lInput.RTS[i] = g->RTS[i];
	lInput.RTS[lInput.cout - 2] = shadowBlured->RTS[0];
	lInput.RTS[lInput.cout - 1] = sky->RTS[1];
	PassOutput* light = lightPass.Draw(&lInput);

	PassOutput finalInput;
	finalInput.cout = sky->cout + light->cout;
	finalInput.RTS = new GLuint[finalInput.cout];
	finalInput.RTS[0] = sky->RTS[0];
	finalInput.RTS[1] = light->RTS[0];
	finalPass.Draw(&finalInput);
}
