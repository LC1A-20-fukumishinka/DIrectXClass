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
	if (Input::Instance()->KeyTrigger(DIK_Q))
	{
		isPostDraw = !isPostDraw;
	}
}


void Game::Draw()
{
	if (isPostDraw)
	{
		PostDraw();
	}
	else
	{
		BaseDraw();
	}
}

void Game::BaseDraw()
{
	myDirectX->PreDraw();
	sceneMgr->Draw();
	myDirectX->PostDraw();
}

void Game::PostDraw()
{
	postTest->PreDrawScene();
	sceneMgr->Draw();
	postTest->PostDrawScene();
	myDirectX->PreDraw();
	postTest->Draw(PostGBPipeline::Instance()->GetPipeLine());
	myDirectX->PostDraw();

}

