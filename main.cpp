#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Game game;
	game.Init();


	while (!game.isEnd) //�Q�[�����[�v
	{
		game.Update();
		//�`��R�}���h
		game.PreDraw();
		game.Draw();
		//debugText.DrawAll();
		//�`��R�}���h�����܂�
		game.PostDraw();
	}
	game.Finalize();
	return 0;
}
