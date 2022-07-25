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
	static void Finalize();
	void SetNextScene(IScene *nextScene) { nextScene_ = nextScene; }

public:
	void Update();
	void PreDraw();
	void Draw();
private:
	IScene *scene_ = nullptr;
	IScene *nextScene_ = nullptr;
	static SceneMgr *instance;
};