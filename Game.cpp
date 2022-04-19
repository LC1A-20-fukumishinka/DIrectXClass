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
}

void Game::Finalize()
{
	Framework::Finalize();
}

void Game::Update()
{
	Framework::Update();
}


void Game::Draw()
{
	Framework::Draw();
}

