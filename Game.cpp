#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "TitleScene.h"
using namespace DirectX;
void Game::Init()
{	//WindowsAPI‰Šú‰»ˆ—

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
	sceneMgr->PreDraw();

	myDirectX->PreDraw();
	sceneMgr->Draw();
	myDirectX->PostDraw();
}

