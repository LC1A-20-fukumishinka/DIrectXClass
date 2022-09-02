#include "GameScene.h"
#include "Vector3.h"
#include "DirectInput.h"
#include "ModelPipeline.h"
#include "ModelPhongPipeline.h"
#include "TextureMgr.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include "FbxLoader.h"
#include "FbxObject3D.h"
#include "SafeDelete.h"
#include "FukuMath.h"
#include "../gameOriginal/GameInput.h"
#include "../Collision/Collision.h"
#include "../gameOriginal/PlanetManager.h"
#include "PostMonochromePipeline.h"
#include "PostGBPipeline.h"
#include "postMosaicPipeline.h"
#include "PostMonochromePipeline.h"
#include "../imgui/ImguiManager.h"
#include "BaseData.h"
using namespace FukuMath;
using namespace DirectX;
using namespace std;
void GameScene::Init()
{


	shadowRenderTarget_ = make_unique<MultiRenderTarget>();
	shadowRenderTarget_->Init(1);

	GBTarget_ = make_unique<MultiRenderTarget>();
	GBTarget_->Init(1);

	MonoTarget_ = make_unique<MultiRenderTarget>();
	MonoTarget_->Init(1);

	MosaicTarget_ = make_unique<MultiRenderTarget>();
	MosaicTarget_->Init(1);

	StartTarget_ = make_unique<MultiRenderTarget>();
	StartTarget_->Init(1);

	cam_ = make_unique<GameCamera>();
	cam_->Init();
	cam_->TitleAnimationStart();
	light_ = unique_ptr<Light>(Light::Create());


	pointLight_ = make_unique<PointLight>();


	spotLight_ = unique_ptr<SpotLight>(SpotLight::Create());


	lightGroup_ = unique_ptr<LightGroup>(LightGroup::Create());
	lightGroup_->SetLight(light_.get());
	lightGroup_->SetPoinntLight(pointLight_.get());
	lightGroup_->SetSpotLight(spotLight_.get());

	lightGroup_->SetAmbientColor(XMFLOAT3{ 0.1f,0.1f, 0.1f });


	int clearTextHandle = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/clearText.png");

	clearText_ = make_unique<Sprite>();
	clearText_->Init(clearTextHandle);


	groundModel_ = make_unique<Model>();
	groundModel_->CreateModel("ground");
	playerModel_ = make_unique<Model>();
	playerModel_->CreateModel("chr_sword");
	domeModel_ = make_unique<Model>();
	domeModel_->CreateModel("skydome");

	objDome_ = make_unique<Object3D>();
	objDome_->Init();
	objDome_->SetModel(domeModel_.get());
	objDome_->SetCamera(cam_->GetCamera());
	objDome_->SetScale(XMFLOAT3(3.0f, 3.0f, 3.0f));
	objDome_->SetLightGroup(lightGroup_.get());

	objGround_ = make_unique<Object3D>();
	objGround_->Init();
	objGround_->SetModel(groundModel_.get());
	objGround_->SetCamera(cam_->GetCamera());
	objGround_->SetLight(light_.get());
	objGround_->SetLightGroup(lightGroup_.get());

	shadowCam_ = make_unique<ShadowCamera>();
	Planet::SetCamera(cam_->GetCamera());
	Planet::SetLight(lightGroup_.get());
	Planet::SetShadowCamera(shadowCam_->GetCamera());
	Planet::SetShadowTexture(shadowRenderTarget_->GetTextureNum(0));

	PlanetManager::Instance()->Init();

	//for (int i = 0; i < 6; i++)
	//{
	//	PlanetManager::Instance()->AddPlanet(XMFLOAT3{ static_cast<float>(30 * i) + 60, 40.0f, 0 }, 10.0f, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), 1, false);
	//}

	//PlanetManager::Instance()->AddPlanet(XMFLOAT3{ static_cast<float>(30 * 6) + 60, 40.0f, 30.0f }, 10.0f, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), 1, false);

	//PlanetManager::Instance()->AddPlanet(XMFLOAT3{ 300, 70.0f, 0 }, 50.0f, DirectX::XMFLOAT4(1.0f, 0.6f, 0.4f, 1.0f), 2, false, PlanetType::BASE);

	int stageCount = 0;
	while (true)
	{
	stageCount++;
	if (!PlanetManager::Instance()->LoadStage(stageCount))
	{
		break;
	}
	}


	player_ = make_unique<GravityPlayer>();
	player_->Init(playerModel_.get(), PlanetManager::Instance()->GetBasePlanet(0));
	player_->SetCamera(cam_->GetCamera());
	player_->SetLight(lightGroup_.get());

	shadowCam_->Init(player_->GetPos());


	player_->SetShadowCamera(shadowCam_->GetCamera());

	postTest_ = std::make_unique<PostEffect>();
	postTest_->Init();

	Flag::SetCamera(cam_->GetCamera());
	Flag::SetLights(lightGroup_.get());
	Flag::SetShadowCamera(shadowCam_->GetCamera());

	weak_ptr<Planet> FlagOnPlanet = PlanetManager::Instance()->GetBasePlanet(1);
	cam_->SetNextPlantPos(FlagOnPlanet.lock()->GetPos());

	MakeFlag(PlanetManager::Instance()->GetPlanet(0), Vector3(0, -1, 0), 2.0f);
	MakeFlag(FlagOnPlanet, Vector3(-1, 0, 0), 2.0f);
	//MakeFlag(a, Vector3(0, 1, 0), 2.0f);
	MakeFlag(FlagOnPlanet, Vector3(1, 0, 0), 2.0f);
	MakeFlag(FlagOnPlanet, Vector3(0, -1, 0), 2.0f);

	for (int i = 1; i < stageCount; i++)
	{
		FlagOnPlanet = PlanetManager::Instance()->GetBasePlanet(i);
		MakeFlag(FlagOnPlanet, Vector3(0, 1, 0), 2.0f);
	}

	Block::SetCamera(cam_->GetCamera());
	Block::SetLights(lightGroup_.get());
	Block tmpBlock;
	tmpBlock.Init(PlanetManager::Instance()->GetPlanet(0), Vector3(0.0f, 1.0f, 1.0f), 1.0f);

	testBlock_.emplace_back(tmpBlock);

	//box.Init(player_->GetPos(), ZVec, YVec);
	//box.SetCamera(cam_->GetCamera());
	titlePostEffect_.Init();

	InformationBoard::SetCamera(cam_->GetCamera());
	InformationBoard::SetLightGroup(lightGroup_.get());
	testBoard_.Init(L"Resources/welcome.png", Vector3(0, 50, 0), Vector3(0, 3, 0), Vector3(3, 1, 0));
	Restart();

}

void GameScene::Update()
{
	
	if (Input::Instance()->KeyTrigger(DIK_1))
	{
		isGB_ = !isGB_;
	}
	if (Input::Instance()->KeyTrigger(DIK_2))
	{
		isMono_ = !isMono_;
	}
	if (Input::Instance()->KeyTrigger(DIK_3))
	{
		isMosaic_ = !isMosaic_;
	}
	//スクショ用停止ポーズ
	if (Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_START))
	{
		isPause_ = !isPause_;
	}
	if (isPause_) return;
	GameInput::Instance()->Update();
	lightGroup_->Update();

	if (isGameTitle_ && GameInput::Instance()->B())
	{
		isGameTitle_ = false;
		cam_->TitleToIngame(player_->GetPos(), player_->GetAngle());
	}

	//掴む
	if (GameInput::Instance()->LockOnInput())
	{

		if (GameInput::Instance()->GrabInput())
		{
			XMFLOAT3 tmp = cam_->GetCameraPos();
			bool isCollision;
			shared_ptr<Planet> grabPlanet;

			isCollision = PlanetManager::Instance()->GetGrabPlanet(grabPlanet, tmp, cam_->GetCamera()->GetAngle());
			if (isCollision)
			{
				player_->SetGrabPlanet(grabPlanet);
			}
		}
	}
	//離す

	//タイトルじゃなかったらプレイヤーを動かせる
	if (!isGameTitle_)
	{
		if (!GameInput::Instance()->GrabInput())
		{
			player_->ReleasePlanet();
		}

		MovePlanet();
		player_->Update();
	}

	for (auto e : testBlock_)
	{
		player_->BlockCollision(e.GetCollisionPlanes());
	}

	objDome_->SetPosition(cam_->GetCameraPos());
	objDome_->Update();

	objGround_->Update();

	if (Input::Instance()->KeyTrigger(DIK_9))
	{
		PlanetManager::Instance()->IDSpawn(1);
	}

	if (Input::Instance()->KeyTrigger(DIK_8))
	{
		PlanetManager::Instance()->IDSpawn(2);
	}
	PlanetManager::Instance()->Update();
	//star->Update();
	//temple_->Update();
	cam_->SetBasePlanet(player_->GetBasePlanet());
	cam_->Update(player_->GetPos(), player_->GetAngle(), player_->GetUpVec());


	bool isGetNow = false;

	for (auto &flag : testFlag_)
	{
		flag.Update();

		if (flag.CollisionPlayer(1.0f, player_->GetPos()))
		{
			isGetNow = true;
		}
	}
	for (auto &e : testBlock_)
	{
		e.Update();
	}
	if (GetFlagCount() <= 0 && !isGameClear_ && isGetNow)
	{
		isGameClear_ = true;
		stageNum++;
		cam_->ClearAnimationStart(player_->GetPos());
	}


	ImguiUpdate();

	shadowCam_->Update(player_->GetPos());
	lightGroup_->Update();


	bool SpeedReset = false;
	ImGui::Begin("Debug");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	SpeedReset = ImGui::Button("Reset");
	ImGui::End();
	if (GameInput::Instance()->ATrigger() && isGameClear_ || SpeedReset)
	{
		Restart();
		cam_->ClearToIngme();
	}
	//box.Update();
	AnimationTestUpdate();
	titlePostEffect_.Update();

	testBoard_.CollisionPlayer(player_->GetPos());
	testBoard_.Update();
}

void GameScene::PreDraw()
{
	shadowRenderTarget_->PreDrawScene();
	player_->ShadowDraw();
	shadowRenderTarget_->PostDrawScene();


	//オブジェクトの描画
	StartTarget_->PreDrawScene();

	objDome_->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	StartTarget_->DepthReset();
	PlanetManager::Instance()->Draw();
	player_->Draw();
	for (auto &flag : testFlag_)
	{
		flag.Draw();
	}

	for (auto &e : testBlock_)
	{
		e.Draw();
	}

	testBoard_.Draw();


	//box.Draw();
	StartTarget_->PostDrawScene();


	DrawTexture_ = StartTarget_->GetTextureNum(0);


	if (isMosaic_)
	{
		vector<int> textureNum;
		textureNum.emplace_back(DrawTexture_);
		MosaicTarget_->PreDrawScene();
		//postTest_->Draw(PostMosaicPipeline::Instance()->GetPipeLine(), textureNum);
		titlePostEffect_.Draw(DrawTexture_);
		MosaicTarget_->PostDrawScene();
		DrawTexture_ = MosaicTarget_->GetTextureNum(0);
	}

	if (isGB_)
	{
		vector<int> textureNum;
		textureNum.emplace_back(DrawTexture_);
		GBTarget_->PreDrawScene();
		postTest_->Draw(PostGBPipeline::Instance()->GetPipeLine(), textureNum);
		GBTarget_->PostDrawScene();
		DrawTexture_ = GBTarget_->GetTextureNum(0);
	}

	if (isMono_)
	{
		vector<int> textureNum;
		textureNum.emplace_back(DrawTexture_);
		MonoTarget_->PreDrawScene();
		postTest_->Draw(PostMonochromePipeline::Instance()->GetPipeLine(), textureNum);
		MonoTarget_->PostDrawScene();
		DrawTexture_ = MonoTarget_->GetTextureNum(0);
	}
}

void GameScene::MainDraw()
{
	vector<int> textureNum;
	textureNum.emplace_back(DrawTexture_);

	postTest_->Draw(NormalDrawPipeline_.GetPipeLine(), textureNum);

	if (isGameClear_)
	{
		clearText_->Draw();
	}
}

void GameScene::Finalize()
{

}

void GameScene::Restart()
{
	lightsRestart();
	ObjectRestart();
	isGameClear_ = false;
}

void GameScene::lightsRestart()
{
	light_->SetLightColor({ 0.6f, 0.6f, 0.6f });
	light_->SetLightDir({ 1.0f,-1.0f ,1.0f ,0.0f });
	light_->SetLightActive(true);
	light_->Update();

	pointLight_->SetActive(true);
	pointLight_->SetLightPos(XMFLOAT3{ 0, 1.5, 0 });
	pointLight_->SetLightAtten(XMFLOAT3(0.2f, 0.2f, 0.2f));

	spotLight_->SetLightDir({ -0.0f, -1.0f, 0.0f });
	spotLight_->SetLightPos({ 0.0f, 1.0f, 0.0f });
	spotLight_->SetLightAtten({ 0.01f, 0.01f, 0.01f });
	spotLight_->SetActive(true);
}

void GameScene::ObjectRestart()
{
	clearText_->size = clearText_->texSize;
	clearText_->position = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
	clearText_->Update();

	PlanetManager::Instance()->Reset();
	player_->Init(playerModel_.get(), PlanetManager::Instance()->GetBasePlanet(0));
	for (auto &e : testFlag_)
	{
		e.Reset();
	}

	for (auto &e : testBlock_)
	{
		e.Update();
	}
}

void GameScene::MovePlanet()
{
	if (player_->GetIsJump())
	{
		shared_ptr<Planet> basePlanet;
		bool isMove = false;
		isMove = PlanetManager::Instance()->MovePlanet(basePlanet, player_->GetPos());

		if (isMove)
		{
			player_->SetBasePlanet(basePlanet);
			cam_->IsAnimationOn();
		}
	}
	else if (cam_->GetIsChangeBasePlanet())
	{
		cam_->StartCameraAnimation(false, 60);
	}
}

void GameScene::ImguiUpdate()
{
#pragma region directionalLight

	ImGui::Begin("light");
	ImGui::SetWindowSize(ImVec2(500, 200), ImGuiCond_::ImGuiCond_FirstUseEver);

	Vector3 angle = light_->GetLightDir();
	float directionalLightAngle[3] = { angle.x,angle.y ,angle.z };
	ImGui::SliderFloat3("Directional", directionalLightAngle, -1.0f, 1.0f);

	angle = Vector3(directionalLightAngle[0], directionalLightAngle[1], directionalLightAngle[2]);

	light_->SetLightDir(XMLoadFloat3(&angle));
	ImGui::End();
	shadowCam_->SetAngle(angle);

#pragma endregion


#pragma region PostEffect
	ImGui::Begin("SimpleEffect");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Checkbox("GB", &isGB_);
	ImGui::Checkbox("Monocrome", &isMono_);
	ImGui::Checkbox("Mosaic", &isMosaic_);
	ImGui::End();
#pragma endregion

}

void GameScene::AnimationTestUpdate()
{
	//クリア状態じゃなかったら抜ける
	if (!isGameClear_) { return; }
	switch (clearStatus_)
	{
	case GameScene::STANDBY:
		//アニメーションが開始します（状態0）操作不能状態に
		GameInput::Instance()->SetIsControll(false);
		clearStatus_ = STOP;
		break;
	case GameScene::STOP:
		//一時停止 タイマー + フラグ（状態１）カメラ移動状態に
		if (true)
		{
			clearStatus_ = MOVECAM;
			cam_->ClearAnimationStart(XMFLOAT3());
		}
		break;
	case GameScene::MOVECAM:
		//カメラが移動＋次の惑星に向けて回転 イージング（状態２）惑星出現状態に
		if (cam_->GetIsAnimationEnd())
		{
			clearStatus_ = SPAWNPLANET;
			PlanetManager::Instance()->IDSpawn(stageNum);
		}
		break;
	case GameScene::SPAWNPLANET:
		//惑星の出現 内部にイージングがあるんでフラグ処理（状態３）カメラ回収状態に
		if (PlanetManager::Instance()->SpawnAnimationEnd(stageNum))
		{
			cam_->ClearToIngme();
			clearStatus_ = RETURNCAM;
		}
		break;
	case GameScene::RETURNCAM:
		//カメラが戻る（状態４）終了処理に
		if (cam_->GetIsAnimationEnd())
		{
			//戻りきったらアニメーション終了（状態４）待機状態に
			isGameClear_ = false;
			weak_ptr<Planet> nextPlanet = PlanetManager::Instance()->GetBasePlanet(stageNum + 1);
			cam_->SetNextPlantPos(nextPlanet.lock()->GetPos());
			clearStatus_ = STANDBY;
			GameInput::Instance()->SetIsControll(true);
		}
		break;
	default:
		isGameClear_ = false;
		clearStatus_ = STANDBY;
		GameInput::Instance()->SetIsControll(true);
		break;
	}
}

void GameScene::MakeFlag(std::weak_ptr<Planet> base, Vector3 angle, float scale)
{
	Flag tmpFlag;
	tmpFlag.Init(base, angle, scale);

	testFlag_.emplace_back(tmpFlag);

}

int GameScene::GetFlagCount()
{
	int aliveFlagCount = 0;

	for (auto &flag : testFlag_)
	{
		aliveFlagCount += static_cast<int>(flag.GetIsDraw());
	}
	return aliveFlagCount;
}
