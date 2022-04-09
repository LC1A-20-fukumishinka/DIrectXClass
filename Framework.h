#pragma once
#include "WinAPI.h"
#include "MyDirectX.h"
#include "DebugText.h"
#include "WinAPI.h"
#include "MyDirectX.h"
#include "Sound.h"
#include "TextureMgr.h"
#include "particleManager.h"
#include "SceneMgr.h"
#include "PostEffect.h"
class Framework
{
public:
	void Run();

	virtual void Init();

	virtual void Update();

	virtual void Finalize();

	virtual void Draw();

	bool IsEnd() { return isEnd; }
protected:
	WinAPI *Win;
	MyDirectX *myDirectX;

	bool isEnd = false;
	SceneMgr *sceneMgr;
	PostEffect *postEffect;
	PostEffect *postEffect2;
};