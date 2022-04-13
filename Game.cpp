#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "TitleScene.h"
using namespace DirectX;
void Game::Init()
{	//WindowsAPI初期化処理

	Framework::Init();

	IScene *scene_ = new TitleScene();
	SceneMgr::Instance()->SetNextScene(scene_);
}

void Game::Finalize()
{
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

