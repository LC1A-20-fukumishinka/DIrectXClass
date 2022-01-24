#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
void Game::Init()
{	//WindowsAPI初期化処理
	Win = WinAPI::GetInstance();
	Win->Init(WINDOW_WIDTH, WINDOW_HEIGHT);


	myDirectX = MyDirectX::GetInstance();
	myDirectX->Init();
	isEnd = false;
}

void Game::Finalize()
{
	Win->end();

}

void Game::Update()
{
	Win->msgCheck();
	if (Input::GetInstance()->Key(DIK_K))
	{
		isEnd = true;
	}
}

void Game::PreDraw()
{
	myDirectX->PreDraw();

}

void Game::Draw()
{
}

void Game::PostDraw()
{
	myDirectX->PostDraw();
}
