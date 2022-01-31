#pragma once
#include "IScene.h"
class SceneMgr
{
private:
	SceneMgr();
	~SceneMgr();
public:
	SceneMgr(SceneMgr &obj) = delete;
	SceneMgr operator=(SceneMgr &obj) = delete;

	static SceneMgr *Instance();
	void SetNextScene(IScene *nextScene) { nextScene_ = nextScene; }

public:
	void Update();
	void Draw();
private:
	IScene *scene_ = nullptr;
	IScene *nextScene_ = nullptr;
};