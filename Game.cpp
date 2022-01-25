#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
using namespace DirectX;
void Game::Init()
{	//WindowsAPI‰Šú‰»ˆ—
	Win = WinAPI::GetInstance();
	Win->Init(WINDOW_WIDTH, WINDOW_HEIGHT);


	myDirectX = MyDirectX::GetInstance();
	myDirectX->Init();
	isEnd = false;
	Input *input = Input::GetInstance();

	Sound::StaticInitialize();
	int alarmIndex = Sound::SoundLoadWave("Resources/Alarm01.wav");
	alarm.Init(alarmIndex);

	input->Init(WinAPI::GetInstance()->w, WinAPI::GetInstance()->hwnd);

	cam.Init(XMFLOAT3(0, 0, -100), XMFLOAT3(0, 0, 0));
	angle = 0.0f;

	domeObj.scale = { 0.1f, 0.1f, 0.1f };
	domeObj.position = { -10,0,0 };
	domeObj.Init(&cam);

	boxObj.Init(&cam);
	boxObj.scale = { 100.0f, 100.0f, 100.0f };
	boxObj.position = { 10, 0,0 };

	dome.CreateModel("skydome");
	triangle.CreateModel("box");
	ParticleManager::StaticInitialize(&cam);

	part = ParticleManager::Create();
	debugText = DebugText::Create();

	spriteTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	sprite.Init(spriteTex);
}

void Game::Finalize()
{
	delete part;
	delete debugText;
	//xAudio2‰ð•ú
	Sound::xAudioDelete();
	//‰¹ºƒf[ƒ^‰ð•ú
	Sound::SoundUnload();
	Win->end();
}

void Game::Update()
{
	Win->msgCheck();
	Input::GetInstance()->Update();

	if (Input::GetInstance()->Key(DIK_K))
	{
		isEnd = true;
	}
	if (Input::GetInstance()->KeyTrigger(DIK_SPACE))
	{
		alarm.Play();
	}
	if (Input::GetInstance()->KeyTrigger(DIK_8))
	{
		alarm.Stop();
	}
	if (Input::GetInstance()->Key(DIK_RIGHT) || Input::GetInstance()->Key(DIK_LEFT) || Input::GetInstance()->Key(DIK_UP) || Input::GetInstance()->Key(DIK_DOWN))
	{
		if (Input::GetInstance()->Key(DIK_RIGHT))
		{
			domeObj.rotation.y++;
		}
		if (Input::GetInstance()->Key(DIK_LEFT))
		{
			domeObj.rotation.y--;
		}
		if (Input::GetInstance()->Key(DIK_UP))
		{
			domeObj.rotation.x++;
		}
		if (Input::GetInstance()->Key(DIK_DOWN))
		{
			domeObj.rotation.x--;
		}
	}
	boxObj.color.x += 0.01;
	domeObj.Update();

	boxObj.Update();

	if (Input::GetInstance()->Key(DIK_D) || Input::GetInstance()->Key(DIK_A))
	{
		if (Input::GetInstance()->Key(DIK_D))
		{
			angle += XMConvertToRadians(1.0f);
		}
		else if (Input::GetInstance()->Key(DIK_A))
		{
			angle -= XMConvertToRadians(1.0f);
		}
		cam.eye.x = -100 * sinf(angle);
		cam.eye.z = -100 * cosf(angle);
	}

	cam.Update();

	part->Add(60, Vector3(), Vector3(((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10), Vector3(), 1, 10);
	part->Update();
	debugText->Print("banana", 100, 100, 10);
	sprite.position.x++;
	sprite.Update();
}

void Game::PreDraw()
{
	myDirectX->PreDraw();

}

void Game::Draw()
{
	domeObj.modelDraw(dome.GetModel(), ModelPipeline::GetInstance()->GetPipeLine());
	boxObj.modelDraw(triangle.GetModel(), ModelPipeline::GetInstance()->GetPipeLine());
	part->Draw(spriteTex);
	sprite.Draw();
	debugText->DrawAll();

}

void Game::PostDraw()
{
	myDirectX->PostDraw();
}
