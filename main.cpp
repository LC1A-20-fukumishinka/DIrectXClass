#include "Base/WinAPI.h"
#include "Base/MyDirectX.h"
#include "Input/DirectInput.h"
#include "2D/Sprite.h"
#include "2D/DebugText.h"
#include "3D/Object3D.h"
#include "Camera.h"
#include "EaseClass.h"
#include "Sound.h"
#include "Base/GraphicsPipeline3D.h"
#include "IGraphicsPipeline.h"
#include "TextureMgr.h"
#include "Model.h"
#include "ModelPipeline.h"
#include "particleManager.h"
using namespace DirectX;

const int window_width = 1280;
const int window_height = 720;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//WindowsAPI初期化処理
#pragma region WindowsAPI

	WinAPI *Win = WinAPI::GetInstance();

	Win->Init(window_width, window_height);
#pragma endregion
#pragma region sound(xAudioInstance)
	//宣言
	Sound::StaticInitialize();
	int alarmIndex = Sound::SoundLoadWave("Resources/Alarm01.wav");

	Sound alarm;
	alarm.Init(alarmIndex);
#pragma endregion

	//DirectX初期化処理 ここまで
	MyDirectX *myDirectX = MyDirectX::GetInstance();
	//DirectInputの初期化処理ここから
#pragma region DirectInput
	Input *input = Input::GetInstance();

	input->Init(WinAPI::GetInstance()->w, WinAPI::GetInstance()->hwnd);
#pragma endregion

	Camera cam;
	cam.Init(XMFLOAT3(0, 0, -100), XMFLOAT3(0, 0, 0));
	float angle = 0.0f;	//カメラの回転角


	//グラフィックスパイプライン設定
#pragma region gpipeline
	int spriteTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	Sprite sprite;
	sprite.Init(spriteTex);


#pragma region DebugText
	//デバッグテキストのインスタンス
	DebugText debugText;
#pragma endregion

	////デバッグテキスト用のテクスチャ番号を指定

	int test = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	int grid = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/gridWall.png");

	Model triangle;
	triangle.CreateModel("box");
	Model dome;
	dome.CreateModel("skydome");
	Object3D box;

	box.scale = { 0.1f, 0.1f, 0.1f };
	box.position = { -10,0,0 };
	box.Init(&cam);
	box.type = Object3D::Box;


	Object3D DrawTriangle;
	DrawTriangle.Init(&cam);
	DrawTriangle.scale = { 100.0f, 100.0f, 100.0f };
	DrawTriangle.position = { 10, 0,0 };


	XMVECTOR boxQuaternion, quaternion2;

	boxQuaternion = XMQuaternionIdentity();//単位クオータニオンの生成
	quaternion2 = XMQuaternionIdentity();//任意軸(1,0,0)方向に90度回転


	bool isTexture = false;

	ParticleManager::StaticInitialize(&cam);

	ParticleManager part;

	part.Add(60, Vector3(), Vector3(((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10), Vector3(), 1, 10);
	while (Win->loopBreak()) //ゲームループ
	{
		//ウィンドウメッセージ処理
#pragma region WindowMessage
		Win->msgCheck();
#pragma endregion
		input->Update();

		//更新

		part.Add(60, Vector3(), Vector3(((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 10), Vector3(), 1, 10);
		if (input->KeyTrigger(DIK_SPACE))
		{
			alarm.Play();
		}
		if (input->KeyTrigger(DIK_8))
		{
			alarm.Stop();
		}
		if (input->Key(DIK_RIGHT) || input->Key(DIK_LEFT) || input->Key(DIK_UP) || input->Key(DIK_DOWN))
		{
			quaternion2 = XMQuaternionIdentity();
			if (input->Key(DIK_RIGHT))
			{
				box.rotation.y++;
			}
			if (input->Key(DIK_LEFT))
			{
				box.rotation.y--;
			}
			if (input->Key(DIK_UP))
			{
				box.rotation.x++;
			}
			if (input->Key(DIK_DOWN))
			{
				box.rotation.x--;
			}
			boxQuaternion = XMQuaternionMultiply(boxQuaternion, quaternion2);
		}

		box.Update();

		DrawTriangle.Update();


		//コマンドリストに追加

#pragma region moveCamera
		if (input->Key(DIK_D) || input->Key(DIK_A))
		{
			if (input->Key(DIK_D))
			{
				angle += XMConvertToRadians(1.0f);
			}
			else if (input->Key(DIK_A))
			{
				angle -= XMConvertToRadians(1.0f);
			}
			cam.eye.x = -100 * sinf(angle);
			cam.eye.z = -100 * cosf(angle);
		}

		cam.Update();
		part.Update();

#pragma endregion

		myDirectX->PreDraw();
		//描画コマンド

		box.modelDraw(dome.GetModel(), ModelPipeline::GetInstance()->GetPipeLine(), isTexture, test);
		DrawTriangle.modelDraw(triangle.GetModel(), ModelPipeline::GetInstance()->GetPipeLine());

		part.Draw(spriteTex);
		debugText.DrawAll();
		//描画コマンドここまで
	//⑤リソースバリアを戻す

		myDirectX->PostDraw();
		// DirectX毎フレーム処理 ここまで
	}
	//xAudio2解放
	Sound::xAudioDelete();
	//音声データ解放
	Sound::SoundUnload();
	//ウィンドウクラスを登録解除
#pragma region WindowsAPI
	Win->end();
#pragma endregion
	return 0;
}

