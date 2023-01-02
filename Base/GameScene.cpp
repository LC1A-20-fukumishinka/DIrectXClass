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
#include "../gameOriginal/gameConstData.h"
using namespace FukuMath;
using namespace DirectX;
using namespace std;
using namespace GameDatas;
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

	BloomTarget_ = make_unique<MultiRenderTarget>();
	BloomTarget_->Init(1);


	BloomDrawTarget_ = make_unique<MultiRenderTarget>();
	BloomDrawTarget_->Init(1);

	TitleTarget_ = make_unique<MultiRenderTarget>();
	TitleTarget_->Init(1);
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


	int AToStartTextHandle = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/A_To_Start_2.png");

	pressStartText_ = make_unique<Sprite>();
	pressStartText_->Init(AToStartTextHandle);
	pressStartText_->position = { WINDOW_WIDTH / 2, WINDOW_HEIGHT * (3.0f / 4.0f) };
	pressStartText_->size = pressStartText_->texSize;
	pressStartText_->size.x *= 0.5f;
	pressStartText_->size.y *= 0.5f;

	pressStartText_->Update();
	pressStartTextAnimationEase_.Init(120);

	models_.LoadModel("ground");
	models_.LoadModel("sphere", true);
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
	objGround_->SetModel(models_.GetModel("ground"));
	objGround_->SetCamera(cam_->GetCamera());
	objGround_->SetLight(light_.get());
	objGround_->SetLightGroup(lightGroup_.get());

	shadowCam_ = make_unique<ShadowCamera>();
	Planet::SetCamera(cam_->GetCamera());
	Planet::SetLight(lightGroup_.get());
	Planet::SetShadowCamera(shadowCam_->GetCamera());
	Planet::SetShadowTexture(shadowRenderTarget_->GetTextureNum(0));

	PlanetManager::Instance()->Init();

	int stageCount = 0;
	while (true)
	{
		stageCount++;
		if (!PlanetManager::Instance()->LoadStage(stageCount))
		{
			break;
		}
	}

	PlanetManager::Instance()->SetStagePlanets(1);

	player_ = make_unique<Player>();
	player_->LoadModel();

	player_->Init(PlanetManager::Instance()->GetBasePlanet(0));
	player_->SetCamera(cam_->GetCamera());
	player_->SetLight(lightGroup_.get());

	shadowCam_->Init(player_->GetPos());


	player_->SetShadowCamera(shadowCam_->GetCamera());

	postTest_ = std::make_unique<PostEffect>();
	postTest_->Init();

	Flag::SetCamera(cam_->GetCamera());
	Flag::SetLights(lightGroup_.get());
	Flag::SetShadowCamera(shadowCam_->GetCamera());

	GuidingStar::SetCamera(cam_->GetCamera());
	GuidingStar::SetLightGroup(lightGroup_.get());

	weak_ptr<Planet> FlagOnPlanet = PlanetManager::Instance()->GetBasePlanet(1);
	cam_->SetNextPlantPos(PlanetManager::Instance()->GetBasePlanet(1)->GetPos());

	MakeFlag(PlanetManager::Instance()->GetPlanet(0), Vector3(0, 1, -0.3f), 2.0f);

	titlePostEffect_.Init();

	InformationBoard::SetCamera(cam_->GetCamera());
	InformationBoard::SetLightGroup(lightGroup_.get());
	InformationBoard welcomeBoard, AtoJump, LTtoLockOn;
	welcomeBoard.Init(L"Resources/welcome.png", PlanetManager::Instance()->GetBasePlanet(0), Vector3(0, 1, 0.2f), Vector3(0, 3, 0), Vector3(3, 1, 0), 10.0f);
	AtoJump.Init(L"Resources/A_To_Jump.png", PlanetManager::Instance()->GetBasePlanet(0), Vector3(0.2f, 1, 0.2f), Vector3(0, 3, 0), Vector3(3, 1, 0), 10.0f);
	LTtoLockOn.Init(L"Resources/LT_To_LockOn.png", PlanetManager::Instance()->GetBasePlanet(0), Vector3(-0.2f, 1, 0.2f), Vector3(0, 3, 0), Vector3(3, 1, 0), 10.0f);
	testBoards_.emplace_back(welcomeBoard);
	testBoards_.emplace_back(AtoJump);
	testBoards_.emplace_back(LTtoLockOn);


	testStar.resize(1);

	for (auto &e : testStar)
	{
		e.Create();
	}

	particles_.Init(cam_->GetCamera());

	Gate::SetCamera(cam_->GetCamera());
	Gate::SetLightGroup(lightGroup_.get());
	Gate::SetModel(models_.GetModel("sphere"));
	gates_.Init();
	gates_.SetGateParticle(particles_.GetGateParticles());

	PlanetManager::Instance()->SetPlanetParticles(particles_.GetPlanetParticles());
	player_->SetRandingParticle(particles_.GetPlayerRandingParticles());
	Restart();

}

void GameScene::Update()
{
	//スクショ用停止ポーズ
	if (GameInput::Instance()->Pause())
	{
		isPause_ = !isPause_;
	}

	bool SpeedReset = false;

	if (isPause_)
	{
		ImGui::Begin("Debug");
		ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
		SpeedReset = ImGui::Button("Reset");
		ImGui::End();
	}
	if (isPause_ && SpeedReset)
	{
		isPause_ = false;
		Restart();
		cam_->ClearToIngme(player_->GetUpVec());
	}

	if (isPause_) return;

	GameInput::Instance()->Update();
	lightGroup_->Update();

	if (isGameTitle_)
	{
		TitleUpdate();
	}
	else
	{
		IngameUpdate();
	}


	objDome_->SetPosition(cam_->GetCameraPos());
	objDome_->Update();

	for (auto &e : testStar)
	{
		e.Update();
	}


	cam_->SetBasePlanet(player_->GetBasePlanet());
	cam_->SetGravityData(player_->GetGravityData());
	cam_->Update(player_->GetPos(), player_->GetAngle(), player_->GetUpVec(), player_->GetPlayerStatus());
	//プレイヤーの状態を受け取る
	gates_.ReceivePlayerStatus(player_->GetPlayerStatus());
	gates_.Update();
	//ゲートの通過処理
	if (gates_.Collision(player_->GetPos(), 3.0f))
	{
		//ゲート通過時にプレイヤーにキャラクターにおこる変化
		player_->passedGate();
	}

	
	//パーティクルの更新処理に必要なデータを受け取る
	GranetParticleManager::MakeParticleDatas particleData;
	particleData.cameraPos = cam_->GetCamera()->GetEye();
	XMMATRIX camRot = cam_->GetCamera()->GetMatBillboard();
	particleData.cameraRot = XMQuaternionRotationMatrix(camRot);
	particleData.playerGravity = player_->GetGravityData();
	particleData.playerPos = player_->GetPos();
	particleData.playerMoveVec = player_->GetMoveVec();
	particles_.SetMakeParticleDatas(particleData);
	//パーティクル更新
	particles_.Update();
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
	for (auto &e : testStar)
	{
		e.Draw();
	}
	StartTarget_->DepthReset();

	gates_.Draw();
	PlanetManager::Instance()->Draw();
	player_->Draw();
	for (auto &flag : testFlag_)
	{
		flag.Draw();
	}

	for (auto &e : testBoards_)
	{
		e.Draw();
	}
	if (isGameTitle_)
	{
		pressStartText_->Draw();
	}

	particles_.Draw();
	StartTarget_->PostDrawScene();


	BloomTarget_->PreDrawScene();
	PlanetManager::Instance()->BloomDraw();
	//fieldParticles_.Draw();
	particles_.Draw();
	BloomTarget_->PostDrawScene();

	DrawTexture_ = StartTarget_->GetTextureNum(0);


	bloom_.BrightUpdate(BloomTarget_->GetTextureNum(0));
	BloomTarget_->PreDrawScene();
	bloom_.Draw(DrawTexture_);
	BloomTarget_->PostDrawScene();
	DrawTexture_ = BloomTarget_->GetTextureNum(0);

	if (isGameTitle_)
	{
		vector<int> textureNum;
		textureNum.emplace_back(DrawTexture_);
		TitleTarget_->PreDrawScene();
		titlePostEffect_.Draw(DrawTexture_);
		TitleTarget_->PostDrawScene();
		DrawTexture_ = TitleTarget_->GetTextureNum(0);
	}

	if (isMosaic_)
	{
		vector<int> textureNum;
		textureNum.emplace_back(DrawTexture_);
		MosaicTarget_->PreDrawScene();
		postTest_->Draw(PostMosaicPipeline::Instance()->GetPipeLine(), textureNum);
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

void GameScene::IngameUpdate()
{

	//タイトルじゃなかったらプレイヤーを動かせる
	if (!isGameTitle_)
	{
		Planet::SetPlayerPos(player_->GetPos());

		if (!GameInput::Instance()->GrabInput())
		{
			player_->ReleasePlanet();
		}
		if (!isGameClear_)
		{
			MovePlanet();
		}
		player_->AddGravity(PlanetManager::Instance()->GetGravity(player_->GetPos()));
		player_->Update();
	}

	if (player_->LandingCamReset())
	{
		cam_->LandingCameraReflesh(player_->GetUpVec());
	}

	for (auto e : testBlock_)
	{
		player_->BlockCollision(e.GetCollisionPlanes());
	}

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

	bool isGetNow = false;

	for (auto &flag : testFlag_)
	{
		flag.Update();

		flag.CollisionPlayer(1.0f, player_->GetPos());
	}

	isOldClear = isClear;
	isClear = (GetFlagCount() <= 0);

	bool clearFlag = ((isClear && isOldClear != isClear) || PlanetManager::Instance()->StageClear());
	if (clearFlag && !isGameClear_)
	{
		isGameClear_ = true;
		stageNum++;
		cam_->ClearAnimationStart(player_->GetPos());
	}


	ImguiUpdate();

	shadowCam_->Update(player_->GetPos());
	lightGroup_->Update();

	StageClearAnimationUpdate();

	for (auto &e : testBoards_)
	{
		e.CollisionPlayer(player_->GetPos());
		e.Update();
	}
}

void GameScene::TitleUpdate()
{
	if (GameInput::Instance()->A())
	{
		isGameTitle_ = false;
		cam_->TitleToIngame(player_->GetPos(), player_->GetAngle());
	}

	float rate = pressStartTextAnimationEase_.Do(Easing::InOut, Easing::Cubic);
	if (isTitleAnimationFlip)
	{
		rate = 1 - rate;
	}
	if (pressStartTextAnimationEase_.IsEnd())
	{
		pressStartTextAnimationEase_.Reset();
		isTitleAnimationFlip = !isTitleAnimationFlip;
	}

	pressStartText_->color.w = rate;

	pressStartText_->Update();
	titlePostEffect_.Update();
}

void GameScene::Restart()
{
	lightsRestart();
	ObjectRestart();
	stageNum = 0;
	cam_->SetNextPlantPos(PlanetManager::Instance()->GetBasePlanet(1)->GetPos());
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

	cam_->Reset();
	isGameTitle_ = true;
	titlePostEffect_.Reset();
	PlanetManager::Instance()->Reset();
	player_->Init(PlanetManager::Instance()->GetBasePlanet(0));
	for (auto &e : testFlag_)
	{
		e.Reset();
	}

	gates_.Reset();
}

void GameScene::MovePlanet()
{
	//空中にいるとき
	if (player_->GetPlayerStatus() == PlayerStatus::JUMP)
	{
		//現在一番近い惑星を調べる
		shared_ptr<Planet> basePlanet;
		bool isMove = false;
		isMove = PlanetManager::Instance()->MovePlanet(basePlanet, player_->GetPos());

		//一番近い惑星が変更されたらプレイヤーの基準になる惑星を変更する
		if (isMove)
		{
			player_->SetBasePlanet(basePlanet);
		}
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

	light_->SetLightDir(F3toV(angle));
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

void GameScene::StageClearAnimationUpdate()
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
			//カメラが戻る処理
			cam_->ClearToIngme(player_->GetUpVec());
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
			PlanetManager::Instance()->SetStagePlanets(stageNum);
		}
		break;
	default:
		isGameClear_ = false;
		clearStatus_ = STANDBY;
		GameInput::Instance()->SetIsControll(true);
		break;
	}
}

void GameScene::makeGuide()
{
	for (auto &e : testStar)
	{
		if (e.GetIsDraw()) continue;
		//正面方向に適当に出す
		Vector3 makePos = Vector3(0.0f, (static_cast<float>(rand() % 10)), (rand() % 50 + 30.0f));

		//XMVECTOR型に
		XMVECTOR makePosV = F3toV(makePos);

		//カメラの姿勢を合わせる
		makePosV = XMVector3Transform(makePosV, cam_->GetCamera()->GetMatBillboard());


		Vector3 destination = PlanetManager::Instance()->GetBasePlanet(1)->GetPos();
		Vector3 destinationAngle = destination - player_->GetPos();
		makePos = makePosV;
		makePos += player_->GetPos();
		makePos += (-destinationAngle.normalize() * 100.0f);
		e.Init(destination, makePos);
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
