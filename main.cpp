#include "2D/DebugText.h"
#include "EaseClass.h"
#include "Base/GraphicsPipeline3D.h"
#include "Game.h"
using namespace DirectX;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Game game;
	game.Init();
#pragma region DebugText
	//デバッグテキストのインスタンス
	DebugText debugText;
#pragma endregion

	////デバッグテキスト用のテクスチャ番号を指定

	int test = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	int grid = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/gridWall.png");



	//Camera cam;
	//cam.Init(XMFLOAT3(0, 0, -100), XMFLOAT3(0, 0, 0));



	XMVECTOR boxQuaternion, quaternion2;

	boxQuaternion = XMQuaternionIdentity();//単位クオータニオンの生成
	quaternion2 = XMQuaternionIdentity();//任意軸(1,0,0)方向に90度回転


	bool isTexture = false;




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

