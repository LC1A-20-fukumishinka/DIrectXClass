#include "TitleScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "Vector3.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "GameScene.h"
using namespace DirectX;
void TitleScene::Init()
{
	debug = DebugText::Create();
}

void TitleScene::Update()
{
	if (Input::Instance()->KeyTrigger(DIK_RETURN) || Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_START))
	{
		SceneMgr::Instance()->SetNextScene(std::make_unique<GameScene>());
	}
	debug->Print("Granet", 100, 100, 10);
}

void TitleScene::PreDraw()
{
}

void TitleScene::MainDraw()
{
	debug->DrawAll();
}

void TitleScene::Finalize()
{
	delete debug;
}
