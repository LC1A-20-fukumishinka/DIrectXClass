#include "Framework.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "FbxLoader.h"
#include "TextureMgr.h"
#include "SafeDelete.h"
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

	postEffect = new PostEffect();
	int white = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/white1x1.png");
	//�|�X�g�G�t�F�N�g�̏�����
	postEffect->Init(white);
	postEffect->size = { 100, 100 };
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

	postEffect->Update();
}

void Framework::Finalize()
{
	sceneMgr->Finalize();
	//xAudio2���
	Sound::xAudioDelete();
	//�����f�[�^���
	Sound::SoundUnload();

	SafeDelete(postEffect);
	FbxLoader::GetInstance()->Finalize();
	myDirectX->Finalize();
	//myDirectX->CheckAliveObject();
	Win->end();
}

void Framework::Draw()
{
	myDirectX->PreDraw();

	postEffect->Draw();
	//sceneMgr->Draw();


	myDirectX->PostDraw();
}
