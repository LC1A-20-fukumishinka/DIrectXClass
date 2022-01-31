#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "GameScene.h"
using namespace DirectX;
void Game::Init()
{	//WindowsAPI‰Šú‰»ˆ—

	Framework::Init();

	scene_ = new GameScene();
	scene_->Init();
	isEnd = false;

	//int alarmIndex = Sound::SoundLoadWave("Resources/Alarm01.wav");
	//alarm = new Sound();
	//alarm->Init(alarmIndex);

	//cam = new Camera();
	//cam->Init(XMFLOAT3(0, 0, -100), XMFLOAT3(0, 0, 0));
	//angle = 0.0f;



	//boxObj = new Object3D();
	//boxObj->Init(cam);
	//boxObj->scale = { 100.0f, 100.0f, 100.0f };
	//boxObj->position = { 10, 0,0 };


	//triangle = new Model();
	//triangle->CreateModel("box");

	//part = ParticleManager::Create();
	//part->SetCamera(cam);

	//spriteTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	//sprite = new Sprite();
	//sprite->Init(spriteTex);
}

void Game::Finalize()
{
	//delete triangle;
	//delete alarm;
	//delete boxObj;
	//delete sprite;
	//delete part;
	scene_->Finalize();
	Framework::Finalize();
}

void Game::Update()
{
	Framework::Update();

	scene_->Update();
	//if (Input::GetInstance()->Key(DIK_K))
	//{
	//	isEnd = true;
	//}
	//if (Input::GetInstance()->KeyTrigger(DIK_SPACE))
	//{
	//	alarm->Play();
	//}
	//if (Input::GetInstance()->KeyTrigger(DIK_8))
	//{
	//	alarm->Stop();
	//}


	//boxObj->color.x += 0.01;

	//boxObj->Update();

	//if (Input::GetInstance()->Key(DIK_D) || Input::GetInstance()->Key(DIK_A))
	//{
	//	if (Input::GetInstance()->Key(DIK_D))
	//	{
	//		angle += XMConvertToRadians(1.0f);
	//	}
	//	else if (Input::GetInstance()->Key(DIK_A))
	//	{
	//		angle -= XMConvertToRadians(1.0f);
	//	}
	//	cam->eye.x = -100 * sinf(angle);
	//	cam->eye.z = -100 * cosf(angle);
	//}

	//cam->Update();

	//part->Add(60, Vector3(), Vector3(((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10), Vector3(), 1, 10);
	//part->Update();
	//debugText->Print("banana", 100, 100, 10);
	//sprite->position.x++;
	//sprite->Update();

}


void Game::Draw()
{
	myDirectX->PreDraw();

	scene_->Draw();
	//boxObj->modelDraw(triangle->GetModel(), ModelPipeline::GetInstance()->GetPipeLine());
	//part->Draw(spriteTex);
	//sprite->Draw();
	//debugText->DrawAll();

	myDirectX->PostDraw();
}

