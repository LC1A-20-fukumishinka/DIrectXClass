#pragma once
#include "WinAPI.h"
#include "MyDirectX.h"
#include "DebugText.h"
#include "WinAPI.h"
#include "MyDirectX.h"
#include "Sound.h"
#include "TextureMgr.h"
#include "particleManager.h"
class Framework
{
public:
	void Run();

	virtual void Init();

	virtual void Update();

	virtual void Finalize();

	virtual void Draw() = 0;

	bool IsEnd() { return isEnd; }
protected:
	WinAPI *Win;
	MyDirectX *myDirectX;
	DebugText *debugText;

	bool isEnd = false;
};