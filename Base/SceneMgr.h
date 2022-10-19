#pragma once
#include "IScene.h"
#include <memory>
class SceneMgr
{
public:
	SceneMgr();
	~SceneMgr();
	SceneMgr(const SceneMgr &obj) = delete;
	SceneMgr operator=(const SceneMgr &obj) = delete;

	static SceneMgr *Instance();
	static void Finalize();
	void SetNextScene(std::unique_ptr<IScene>nextScene) { nextScene_.swap(nextScene); }

public:
	void Update();
	void PreDraw();
	void Draw();
private:
	std::unique_ptr<IScene> scene_;
	std::unique_ptr<IScene> nextScene_ ;
	static std::unique_ptr<SceneMgr> instance;
};