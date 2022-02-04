#include "GameScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "ModelPhongPipeline.h"
void GameScene::Init()
{
	cam = new Camera();
	cam->Init(Vector3(0, 0, -100), Vector3(0, 0, 0));
	light = Light::Create();
	light->SetLightColor({1, 1, 1});
	light->Update();
	domeObj = new Object3D();
	domeObj->scale = { 0.1f, 0.1f, 0.1f };
	domeObj->position = { -10,0,0 };
	domeObj->Init(cam, light);
	dome = new Model();
	dome->CreateModel("skydome");
}

void GameScene::Update()
{
	if (Input::Instance()->Key(DIK_RIGHT) || Input::Instance()->Key(DIK_LEFT) || Input::Instance()->Key(DIK_UP) || Input::Instance()->Key(DIK_DOWN))
	{
		if (Input::Instance()->Key(DIK_RIGHT))
		{
			domeObj->rotation.y++;
		}
		if (Input::Instance()->Key(DIK_LEFT))
		{
			domeObj->rotation.y--;
		}
		if (Input::Instance()->Key(DIK_UP))
		{
			domeObj->rotation.x++;
		}
		if (Input::Instance()->Key(DIK_DOWN))
		{
			domeObj->rotation.x--;
		}
	}
	domeObj->Update();
}

void GameScene::Draw()
{
	domeObj->modelDraw(dome->GetModel(), ModelPhongPipeline::Instance()->GetPipeLine());
}

void GameScene::Finalize()
{
	delete dome;
	delete domeObj;
	delete light;

}
