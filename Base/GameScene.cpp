#include "GameScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
void GameScene::Init()
{
	cam = new Camera();
	cam->Init(Vector3(0, 0, -100), Vector3(0, 0, 0));
	domeObj = new Object3D();
	domeObj->scale = { 0.1f, 0.1f, 0.1f };
	domeObj->position = { -10,0,0 };
	domeObj->Init(cam);
	dome = new Model();
	dome->CreateModel("skydome");
}

void GameScene::Update()
{
	if (Input::GetInstance()->Key(DIK_RIGHT) || Input::GetInstance()->Key(DIK_LEFT) || Input::GetInstance()->Key(DIK_UP) || Input::GetInstance()->Key(DIK_DOWN))
	{
		if (Input::GetInstance()->Key(DIK_RIGHT))
		{
			domeObj->rotation.y++;
		}
		if (Input::GetInstance()->Key(DIK_LEFT))
		{
			domeObj->rotation.y--;
		}
		if (Input::GetInstance()->Key(DIK_UP))
		{
			domeObj->rotation.x++;
		}
		if (Input::GetInstance()->Key(DIK_DOWN))
		{
			domeObj->rotation.x--;
		}
	}
	domeObj->Update();
}

void GameScene::Draw()
{
	domeObj->modelDraw(dome->GetModel(), ModelPipeline::GetInstance()->GetPipeLine());
}

void GameScene::Finalize()
{
	delete dome;
	delete domeObj;
}
