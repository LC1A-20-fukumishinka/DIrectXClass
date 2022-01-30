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
	virtual void Init();
protected:
	WinAPI *Win;
	MyDirectX *myDirectX;
	DebugText *debugText;
};