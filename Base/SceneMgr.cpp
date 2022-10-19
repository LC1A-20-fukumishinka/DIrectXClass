#include "SceneMgr.h"

std::unique_ptr<SceneMgr> SceneMgr::instance = nullptr;
SceneMgr::SceneMgr()
{
}

SceneMgr::~SceneMgr()
{
	scene_->Finalize();
}

SceneMgr *SceneMgr::Instance()
{
	if (!instance)
	{
		instance = std::make_unique<SceneMgr>();
	}

	return instance.get();
}

void SceneMgr::Finalize()
{
}

void SceneMgr::Update()
{
	//シーンの切り替え
	if (nextScene_)
	{//シーンの変更をする
		if (scene_)
		{
			scene_->Finalize();
			scene_.reset();
		}
		scene_.swap(nextScene_);
		scene_->Init();
	}
	scene_->Update();
}

void SceneMgr::PreDraw()
{
	scene_->PreDraw();
}

void SceneMgr::Draw()
{
	scene_->MainDraw();
}
