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
	//�V�[���̐؂�ւ�
	if (nextScene_)
	{//�V�[���̕ύX������
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
