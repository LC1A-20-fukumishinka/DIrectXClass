#include "SceneMgr.h"

SceneMgr::SceneMgr()
{
}

SceneMgr::~SceneMgr()
{
	scene_->Finalize();
	delete scene_;
}

SceneMgr *SceneMgr::Instance()
{
	static SceneMgr instance;

	return &instance;
}

void SceneMgr::Update()
{
	//シーンの切り替え
	if (nextScene_)
	{//シーンの変更をする
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		scene_ = nextScene_;
		nextScene_ = nullptr;
		scene_->Init();
	}
scene_->Update();
}

void SceneMgr::Draw()
{
	scene_->Draw();
}
