#include "Framework.h"
#include "BaseData.h"
#include "DirectInput.h"

void Framework::Run()
{
	Init();


	while (true) //�Q�[�����[�v
	{
		Update();

		if (IsEnd())
		{
			break;
		}
		//�`��R�}���h
		Draw();
		//debugText.DrawAll();
		//�`��R�}���h�����܂�
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
	//xAudio2���
	Sound::xAudioDelete();
	//�����f�[�^���
	Sound::SoundUnload();

//myDirectX->CheckAliveObject();
Win->end();
}
