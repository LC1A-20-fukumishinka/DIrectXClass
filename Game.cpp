#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "TitleScene.h"
#include "GameScene.h"
using namespace DirectX;
void Game::Init()
{	//WindowsAPI初期化処理

	Framework::Init();

	IScene *scene_ = new GameScene();
	SceneMgr::Instance()->SetNextScene(scene_);
	postTest = std::make_unique<PostEffect>();
	postTest->Init();
}

void Game::Finalize()
{
	postTest.reset();
	Framework::Finalize();
}

void Game::Update()
{
	Framework::Update();
	sceneMgr->Update();
}


void Game::Draw()
{
	postTest->PreDrawScene();
	sceneMgr->Draw();
	postTest->PostDrawScene();
	myDirectX->PreDraw();
	postTest->Draw(PostEffectTestPipeline::Instance()->GetPipeLine());
	myDirectX->PostDraw();
}

