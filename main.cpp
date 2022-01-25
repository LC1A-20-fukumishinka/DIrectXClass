#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Game game;
	game.Init();


	while (!game.isEnd) //ゲームループ
	{
		game.Update();
		//描画コマンド
		game.PreDraw();
		game.Draw();
		//debugText.DrawAll();
		//描画コマンドここまで
		game.PostDraw();
	}
	game.Finalize();
	return 0;
}
