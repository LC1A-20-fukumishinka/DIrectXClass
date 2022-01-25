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
	//�f�o�b�O�e�L�X�g�̃C���X�^���X
	DebugText debugText;
#pragma endregion

	////�f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��

	int test = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	int grid = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/gridWall.png");



	//Camera cam;
	//cam.Init(XMFLOAT3(0, 0, -100), XMFLOAT3(0, 0, 0));



	XMVECTOR boxQuaternion, quaternion2;

	boxQuaternion = XMQuaternionIdentity();//�P�ʃN�I�[�^�j�I���̐���
	quaternion2 = XMQuaternionIdentity();//�C�ӎ�(1,0,0)������90�x��]


	bool isTexture = false;




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

