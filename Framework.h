#pragma once
#include "WinAPI.h"
#include "MyDirectX.h"
#include "DebugText.h"
#include "WinAPI.h"
#include "MyDirectX.h"
#include "Sound.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
class Framework
{
public:
	~Framework();

	void Run();

	virtual void Init();

	virtual void Update();

	virtual void Finalize();

	virtual void Draw();

	bool IsEnd() { return isEnd; }
protected:
	WinAPI *Win;
	MyDirectX *myDirectX;
	SceneMgr *sceneMgr;

	bool isEnd = false;
};