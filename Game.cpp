#include "Game.h"
#include "BaseData.h"
#include "DirectInput.h"
void Game::Init()
{	//WindowsAPI‰Šú‰»ˆ—
	Win = WinAPI::GetInstance();
	Win->Init(WINDOW_WIDTH, WINDOW_HEIGHT);


	myDirectX = MyDirectX::GetInstance();
	myDirectX->Init();
	isEnd = false;
	Input *input = Input::GetInstance();

	input->Init(WinAPI::GetInstance()->w, WinAPI::GetInstance()->hwnd);
}

void Game::Finalize()
{
	Win->end();
}

void Game::Update()
{
	Win->msgCheck();
	Input::GetInstance()->Update();

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
