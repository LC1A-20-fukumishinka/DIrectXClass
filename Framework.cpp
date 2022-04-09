#include "Framework.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "FbxLoader.h"
#include "TextureMgr.h"
#include "SafeDelete.h"
#include "PostEffectTestPipeline.h"
#include "PostMosaicPipeline.h"
#include "PostMonochromePipeline.h"
#include "PostGBPipeline.h"
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
	//�|�X�g�G�t�F�N�g�̏�����
	postEffect->Init();


	postEffect2 = new PostEffect();
	postEffect2->Init();
	//postEffect->size = { 100, 100 };
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

	//postEffect->Update();
}

void Framework::Finalize()
{
	sceneMgr->Finalize();
	//xAudio2���
	Sound::xAudioDelete();
	//�����f�[�^���
	Sound::SoundUnload();

	SafeDelete(postEffect);
	SafeDelete(postEffect2);
	FbxLoader::GetInstance()->Finalize();
	myDirectX->Finalize();
	//myDirectX->CheckAliveObject();
	Win->end();
}

void Framework::Draw()
{
	postEffect->PreDrawScene();
	sceneMgr->Draw();
	postEffect->PostDrawScene();

	postEffect2->PreDrawScene();
	postEffect->Draw(PostMosaicPipeline::Instance()->GetPipeLine());
	postEffect2->PostDrawScene();

	myDirectX->PreDraw();
	//postEffect->Draw(PostEffectTestPipeline::Instance()->GetPipeLine());

	postEffect2->Draw(PostGBPipeline::Instance()->GetPipeLine());
	myDirectX->PostDraw();
}
