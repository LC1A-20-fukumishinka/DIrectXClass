#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "TitleScene.h"
#include "GameScene.h"
using namespace DirectX;
void Game::Init()
{	//WindowsAPI‰Šú‰»ˆ—

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
	myDirectX->PreDraw();
	sceneMgr->Draw();
	myDirectX->PostDraw();
}

