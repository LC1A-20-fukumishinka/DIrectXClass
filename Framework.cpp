#include "Framework.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "FbxLoader.h"
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
	Win = WinAPI::Instance();
	Win->Init(WINDOW_WIDTH, WINDOW_HEIGHT);


	myDirectX = MyDirectX::Instance();
	myDirectX->Init();

	FbxLoader::GetInstance()->Initialize();

	Input *input = Input::Instance();
	input->Init(WinAPI::Instance()->w, WinAPI::Instance()->hwnd);

	Sound::StaticInitialize();
	ParticleManager::StaticInitialize();

	sceneMgr = SceneMgr::Instance();
	isEnd = false;
}

void Framework::Update()
{
	if (Win->loopBreak() || Input::Instance()->Key(DIK_ESCAPE))
	{
		isEnd = true;
		return;
	}
	Win->msgCheck();
	Input::Instance()->Update();

	sceneMgr->Update();
}

void Framework::Finalize()
{
	sceneMgr->Finalize();
	//xAudio2解放
	Sound::xAudioDelete();
	//音声データ解放
	Sound::SoundUnload();

	FbxLoader::GetInstance()->Finalize();
	myDirectX->Finalize();
	//myDirectX->CheckAliveObject();
	Win->end();
}

void Framework::Draw()
{
	myDirectX->PreDraw();

	sceneMgr->Draw();


	myDirectX->PostDraw();
}
