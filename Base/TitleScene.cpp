#include "TitleScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "Vector3.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "GameScene.h"
using namespace DirectX;
void TitleScene::Init()
{
	int alarmIndex = Sound::SoundLoadWave("Resources/Alarm01.wav");
	alarm = new Sound();
	alarm->Init(alarmIndex);

	cam = new Camera();
	cam->Init(Vector3(0, 0, -100), Vector3(0, 0, 0));
	startAngle = 0.0f;

	light = Light::Create();

	light->Initialize();
	light->SetLightColor({1, 1, 1});
	light->SetLightDir({-1, 0, 1});
	light->Update();
	lightGroup = LightGroup::Create();
	lightGroup->SetLight(light);
	boxObj = new Object3D;
	boxObj->Init();
	boxObj->SetCamera(cam);
	boxObj->SetLightGroup(lightGroup);
	boxObj->SetScale({ 100.0f, 100.0f, 100.0f });
	boxObj->SetPosition({ 10, 0,0 });


	triangle = new Model();
	triangle->CreateModel("box");
	boxObj->SetModel(triangle);

	part = ParticleManager::Create();
	part->SetCamera(cam);

	spriteTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	sprite = new Sprite();
	sprite->Init(spriteTex);

	debug = DebugText::Create();
}

void TitleScene::Update()
{

	if (Input::Instance()->KeyTrigger(DIK_SPACE))
	{
		alarm->Play();
	}
	if (Input::Instance()->KeyTrigger(DIK_8))
	{
		alarm->Stop();
	}

	if (Input::Instance()->KeyTrigger(DIK_RETURN))
	{
		IScene *scene = new GameScene();
		SceneMgr::Instance()->SetNextScene(scene);
	}
	boxObj->Update();

	if (Input::Instance()->Key(DIK_D) || Input::Instance()->Key(DIK_A))
	{
		if (Input::Instance()->Key(DIK_D))
		{
			startAngle += XMConvertToRadians(1.0f);
		}
		else if (Input::Instance()->Key(DIK_A))
		{
			startAngle -= XMConvertToRadians(1.0f);
		}

		XMFLOAT3 eye = cam->GetEye();

		eye.x = -100 * sinf(startAngle);
		eye.z = -100 * cosf(startAngle);
		cam->SetEye(eye);
	}

	cam->Update();
	lightGroup->Update();
	part->Add(60, Vector3(), Vector3(((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10), Vector3(), 1, 10);
	part->Update();
	debug->Print("banana", 100, 100, 10);
	sprite->position.x++;
	sprite->Update();
}

void TitleScene::Draw()
{
	boxObj->modelDraw( ModelPipeline::Instance()->GetPipeLine());
	part->Draw(spriteTex);
	sprite->Draw();
	debug->DrawAll();
}

void TitleScene::Finalize()
{
	delete triangle;
	delete alarm;
	delete boxObj;
	delete sprite;
	delete part;
	delete debug;
	delete cam;
	delete light;
	delete lightGroup;
}
