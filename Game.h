#pragma once
#include "WinAPI.h"
#include "MyDirectX.h"
class Game
{
public:
	void Init();
	void Finalize();
	void Update();
	void PreDraw();
	void Draw();
	void PostDraw();
public:
bool isEnd;
private:
	WinAPI *Win;
		MyDirectX *myDirectX;
};