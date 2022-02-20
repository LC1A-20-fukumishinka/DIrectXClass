#include "GameScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "ModelPhongPipeline.h"
#include "TextureMgr.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
using namespace DirectX;
void GameScene::Init()
{
	cam = new Camera();
	cam->Init(Vector3(0, 1, -10), Vector3(0, 0, 0));
	light = Light::Create();
	light->SetLightColor({ 0, 0, 0 });
	light->SetLightActive(true);
	light->Update();

	pointLight = PointLight::Create();
	pointLight->SetActive(true);
	pointLight->SetLightPos(XMFLOAT3{0, 1.5, 0});
	pointLight->SetLightAtten(XMFLOAT3(0.2f, 0.2f, 0.2f));
	lightGroup = LightGroup::Create();
	lightGroup->SetLight(light);
	lightGroup->SetPoinntLight(pointLight);
	domeObj = new Object3D;
	domeObj->SetPosition(XMFLOAT3(0, 0, 0));
	domeObj->Init();
	domeObj->SetCamera(cam);
	domeObj->SetLight(light);
	domeObj->SetLightGroup(lightGroup);

	objFighter = Player::Create(cam, light);
	objFighter->SetCamera(cam);
	objFighter->SetLight(light);
	objFighter->SetLightGroup(lightGroup);
	objFighter->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	objFighter->SetPosition(XMFLOAT3(-1, 0, 0));

	objectSphere = new Object3D;
	objectSphere->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	objectSphere->SetPosition(XMFLOAT3(-3, 0, 0));
	objectSphere->Init();
	objectSphere->SetCamera(cam);
	objectSphere->SetLight(light);
	objectSphere->SetLightGroup(lightGroup);

	dome = new Model();
	sphereModel = new Model();
	fighterModel = new Model();
	dome->CreateModel("skydome");
	sphereModel->CreateModel("sphere");
	fighterModel->CreateModel("chr_sword");
	domeObj->SetModel(dome);
	objFighter->SetModel(fighterModel);
	objectSphere->SetModel(sphereModel);
	objectSphere->SetCollider(new SphereCollider);
	rayPart = ParticleManager::Create();
	partTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	rayPart->SetCamera(cam);
	groundModel = new Model();
	groundModel->CreateModel("ground");
	objGround = TouchableObject::create(groundModel);
	objGround->SetCamera(cam);
	objGround->SetLight(light);
	objGround->SetLightGroup(lightGroup);

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
	ray.dir = { 0, -1, 0, 0 };
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
	lightGroup->Update();
	rayPart->Update();
	cam->Update();
	domeObj->Update();
	objFighter->Update();
	objectSphere->Update();
	objGround->Update();
	CollisionManager::GetInstance()->CheckAllCollision();
}

void GameScene::Draw()
{
	objGround->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
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
	delete groundModel;
	delete objGround;
	delete pointLight;
	delete lightGroup;
}
