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
	ssaoPass.Init();
	giPass.Init();
	ssrPass.Init();
}

void RenderPassManager::Draw()
{
	PassOutput* sm = smPass.Draw(NULL);//vpl pos and depth, vpl albedo
	PassOutput* sky = skyPass.Draw(NULL);//sky
	PassOutput* g = gPass.Draw(NULL);//albedo, normal, position, stensil

	PassOutput sInput;
	sInput.cout = 2;
	sInput.RTS = new GLuint[sInput.cout];
	sInput.RTS[0] = g->RTS[2];
	sInput.RTS[1] = sm->RTS[0];
	sInput.mats = new Matrix4x4[1];
	sInput.mats[0] = sm->mats[0];
	PassOutput* shadow = shadowPass.Draw(&sInput);//shadow

	PassOutput aoInput;
	aoInput.cout = 3;
	aoInput.RTS = new GLuint[aoInput.cout];
	aoInput.RTS[0] = g->RTS[1];
	aoInput.RTS[1] = g->RTS[2];
	aoInput.RTS[2] = shadow->RTS[0];
	PassOutput* ao = ssaoPass.Draw(&aoInput);//shadow and ao
	PassOutput* shadowBlured = shadowBlurPass.Draw(ao);

	PassOutput giInput;
	giInput.cout = 5;
	giInput.RTS = new GLuint[giInput.cout];
	giInput.RTS[0] = g->RTS[2];
	giInput.RTS[1] = g->RTS[1];
	giInput.RTS[2] = sm->RTS[0];
	giInput.RTS[3] = sm->RTS[1];
	giInput.RTS[4] = sm->RTS[2];
	giInput.mats = new Matrix4x4[1];
	giInput.mats[0] = sm->mats[0];
	PassOutput* gi = giPass.Draw(&giInput);//GI

	PassOutput lInput;
	lInput.cout = 7;
	lInput.RTS = new GLuint[lInput.cout];
	lInput.RTS[0] = g->RTS[0];
	lInput.RTS[1] = g->RTS[1];
	lInput.RTS[2] = g->RTS[2];
	lInput.RTS[3] = g->RTS[3];
	lInput.RTS[4] = shadowBlured->RTS[0];
	lInput.RTS[5] = gi->RTS[0];
	lInput.RTS[6] = sky->RTS[1];
	PassOutput* light = lightPass.Draw(&lInput);//light, pureLight, indSpecPara

	PassOutput rInput;
	rInput.cout = 5;
	rInput.RTS = new GLuint[rInput.cout];
	rInput.RTS[0] = light->RTS[0];
	rInput.RTS[1] = g->RTS[1];
	rInput.RTS[2] = g->RTS[2];
	rInput.RTS[3] = light->RTS[2];
	rInput.RTS[4] = sky->RTS[1];
	PassOutput* ssr = ssrPass.Draw(&rInput);//SSR

	PassOutput finalInput;
	finalInput.cout = 4;
	finalInput.RTS = new GLuint[finalInput.cout];
	finalInput.RTS[0] = sky->RTS[0];
	finalInput.RTS[1] = light->RTS[1];
	finalInput.RTS[2] = ssr->RTS[0];
	finalInput.RTS[3] = g->RTS[3];
	finalPass.Draw(&finalInput);
}
