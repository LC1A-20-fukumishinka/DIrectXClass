#include "GameScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "ModelPhongPipeline.h"
using namespace DirectX;
void GameScene::Init()
{
	cam = new Camera();
	cam->Init(Vector3(0, 20, -10), Vector3(0, 0, 0));
	light = Light::Create();
	light->SetLightColor({1, 1, 1});
	light->Update();
	domeObj = new Object3D;
	domeObj->SetScale( XMFLOAT3(0.1f, 0.1f, 0.1f) );
	domeObj->SetPosition(XMFLOAT3( -10,0,0 ));
	domeObj->Init(cam, light);
	objFighter = Player::Create(cam,light);
	objFighter->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	objFighter->SetPosition(XMFLOAT3(-10, 0, 0));
	objectSphere = new Object3D;
	objectSphere->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	objectSphere->SetPosition(XMFLOAT3(-10, 0, 0));
	objectSphere->Init(cam, light);

	dome = new Model();
	sphereModel = new Model();
	fighterModel = new Model();
	dome->CreateModel("skydome");
	sphereModel->CreateModel("sphere");
	fighterModel->CreateModel("chr_sword");
	domeObj->SetModel(dome->GetModel());
	objFighter->SetModel(fighterModel->GetModel());
	objectSphere->SetModel(sphereModel->GetModel());
	objectSphere->SetCollider(new SphereCollider);
}

void GameScene::Update()
{
	if (Input::Instance()->Key(DIK_RIGHT) || Input::Instance()->Key(DIK_LEFT) || Input::Instance()->Key(DIK_UP) || Input::Instance()->Key(DIK_DOWN))
	{
		Vector3 rotation;
		rotation = domeObj->GetRotation();
		if (Input::Instance()->Key(DIK_RIGHT))
		{
			rotation.y++;
		}
		if (Input::Instance()->Key(DIK_LEFT))
		{
			rotation.y--;
		}
		if (Input::Instance()->Key(DIK_UP))
		{
			rotation.x++;
		}
		if (Input::Instance()->Key(DIK_DOWN))
		{
			rotation.x--;
		}
		domeObj->SetRotation(rotation);
	}
	cam->Update();
	domeObj->Update();
	objFighter->Update();
	objectSphere->Update();
	CollisionManager::GetInstance()->CheckAllCollision();
}

void GameScene::Draw()
{
	domeObj->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	objFighter->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	objectSphere->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GameScene::Finalize()
{
	delete dome;
	delete objFighter;
	delete objectSphere;
	delete domeObj;
	delete light;

}
