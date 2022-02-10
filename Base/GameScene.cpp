#include "GameScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "ModelPhongPipeline.h"
#include "TextureMgr.h"
using namespace DirectX;
void GameScene::Init()
{
	cam = new Camera();
	cam->Init(Vector3(0, 1, -10), Vector3(0, 0, 0));
	light = Light::Create();
	light->SetLightColor({ 1, 1, 1 });
	light->Update();
	domeObj = new Object3D;/*
	domeObj->SetScale( XMFLOAT3(1f, 0.1f, 0.1f) );*/
	domeObj->SetPosition(XMFLOAT3(0, 0, 0));
	domeObj->Init(cam, light);
	objFighter = Player::Create(cam, light);
	objFighter->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	objFighter->SetPosition(XMFLOAT3(-1, 0, 0));
	objectSphere = new Object3D;
	objectSphere->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	objectSphere->SetPosition(XMFLOAT3(-3, 0, 0));
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
	rayPart = ParticleManager::Create();
	partTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	rayPart->SetCamera(cam);
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

	Ray ray;
	ray.start = { 10.0f, 0.5f, 0.0f, 1 };
	ray.dir = { -1, 0, 0, 0 };
	RaycastHit raycastHit;
	if (CollisionManager::GetInstance()->Raycast(ray, &raycastHit))
	{
		for (int i = 0; i < 1; ++i)
		{
			const float rnd_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			rayPart->Add(10, XMFLOAT3(raycastHit.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f);
		}
	}
	rayPart->Update();
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
	rayPart->Draw(partTex);
}

void GameScene::Finalize()
{
	delete dome;
	delete sphereModel;
	delete fighterModel;
	delete objFighter;
	delete objectSphere;
	delete domeObj;
	delete light;
	delete rayPart;

}
