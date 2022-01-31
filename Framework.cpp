#include "Framework.h"
#include "BaseData.h"
#include "DirectInput.h"

void Framework::Run()
{
	Init();


	while (true) //ゲームループ
	{
		Update();

		if (IsEnd())
		{
			break;
		}
		//描画コマンド
		Draw();
		//debugText.DrawAll();
		//描画コマンドここまで
	}
	Finalize();
}

void Framework::Init()
{
	Win = WinAPI::GetInstance();
	Win->Init(WINDOW_WIDTH, WINDOW_HEIGHT);


	myDirectX = MyDirectX::GetInstance();
	myDirectX->Init();

	Input *input = Input::GetInstance();
	input->Init(WinAPI::GetInstance()->w, WinAPI::GetInstance()->hwnd);

	Sound::StaticInitialize();
	ParticleManager::StaticInitialize();

	debugText = DebugText::Create();

}

void Framework::Update()
{
	if (Win->loopBreak())
	{
		isEnd = true;
		return;
	}
	Win->msgCheck();
	Input::GetInstance()->Update();
}

void Framework::Finalize()
{
	delete debugText;
	//xAudio2解放
	Sound::xAudioDelete();
	//音声データ解放
	Sound::SoundUnload();

//myDirectX->CheckAliveObject();
Win->end();
}
