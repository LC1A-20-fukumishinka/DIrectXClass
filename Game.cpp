#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"

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
}

void Game::Finalize()
{
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
}

void Game::PreDraw()
{
	myDirectX->PreDraw();

}

void Game::Draw()
{
}

void Game::PostDraw()
{
	myDirectX->PostDraw();
}
