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

	light_ = unique_ptr<Light>(Light::Create());
	light_->SetLightColor({ 0.6f, 0.6f, 0.6f });
	light_->SetLightDir({ 1.0f,-1.0f ,1.0f ,0.0f });
	light_->SetLightActive(true);
	light_->Update();

	pointLight_ = make_unique<PointLight>();
	pointLight_->SetActive(true);
	pointLight_->SetLightPos(XMFLOAT3{ 0, 1.5, 0 });
	pointLight_->SetLightAtten(XMFLOAT3(0.2f, 0.2f, 0.2f));

	spotLight_ = unique_ptr<SpotLight>(SpotLight::Create());
	spotLight_->SetLightDir({ -0.0f, -1.0f, 0.0f });
	spotLight_->SetLightPos({ 0.0f, 1.0f, 0.0f });
	spotLight_->SetLightAtten({ 0.01f, 0.01f, 0.01f });
	spotLight_->SetActive(true);

	lightGroup_ = unique_ptr<LightGroup>(LightGroup::Create());
	lightGroup_->SetLight(light_.get());
	lightGroup_->SetPoinntLight(pointLight_.get());
	lightGroup_->SetSpotLight(spotLight_.get());

	lightGroup_->SetAmbientColor(XMFLOAT3{ 0.1f,0.1f, 0.1f });
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
	//objDome_->SetLight(light_.get());
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

	for (int i = 0; i < 6; i++)
	{
		PlanetManager::Instance()->AddPlanet(XMFLOAT3{ static_cast<float>(30 * i) + 60, 40.0f, 0 }, 10.0f, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
	}

	PlanetManager::Instance()->AddPlanet(XMFLOAT3{ static_cast<float>(30 * 6) + 60, 40.0f, 30.0f }, 10.0f, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));

	PlanetManager::Instance()->AddPlanet(XMFLOAT3{ 300, 70.0f, 0 }, 50.0f, DirectX::XMFLOAT4(1.0f, 0.6f, 0.4f, 1.0f));

	player_ = make_unique<GravityPlayer>();
	player_->Init(playerModel_.get(), PlanetManager::Instance()->GetBasePlanet());
	player_->SetCamera(cam_->GetCamera());
	player_->SetLight(lightGroup_.get());

	shadowCam_->Init(player_->GetPos());


	player_->SetShadowCamera(shadowCam_->GetCamera());

	postTest_ = std::make_unique<PostEffect>();
	postTest_->Init();

	Flag::SetCamera(cam_->GetCamera());
	Flag::SetLights(lightGroup_.get());
	Flag::SetShadowCamera(shadowCam_->GetCamera());

	weak_ptr<Planet> a = PlanetManager::Instance()->GetPlanet(8);
	MakeFlag(a, Vector3(-1, 0, 0), 2.0f);
	MakeFlag(a, Vector3(0, 1, 0), 2.0f);
	MakeFlag(a, Vector3(1, 0, 0), 2.0f);
	MakeFlag(a, Vector3(0, -1, 0), 2.0f);



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
	if (!GameInput::Instance()->GrabInput())
	{
		player_->ReleasePlanet();
	}

	MovePlanet();
	player_->Update();

	objDome_->SetPosition(cam_->GetCameraPos());
	objDome_->Update();

	objGround_->Update();
	PlanetManager::Instance()->Update();
	//star->Update();
	//temple_->Update();
	cam_->SetBasePlanet(player_->GetBasePlanet());
	cam_->Update(player_->GetPos(), player_->GetAngle(), player_->GetUpVec());


	for (auto &flag : testFlag_)
	{
		flag.Update();

		flag.CollisionPlayer(1.0f, player_->GetPos());
	}
	if (GetFlagCount() <= 0 && !isGameClear)
	{
		isGameClear = true;
		cam_->ClearAnimationStart(player_->GetPos());
	}

	ImGui::Begin("light");
	ImGui::SetWindowSize(ImVec2(500, 200), ImGuiCond_::ImGuiCond_FirstUseEver);

	Vector3 angle = light_->GetLightDir();
	float directionalLightAngle[3] = { angle.x,angle.y ,angle.z };
	ImGui::SliderFloat3("Directional", directionalLightAngle, -1.0f, 1.0f);

	angle = Vector3(directionalLightAngle[0], directionalLightAngle[1], directionalLightAngle[2]);

	light_->SetLightDir(XMLoadFloat3(&angle));
	ImGui::End();

	shadowCam_->SetAngle(angle);

	shadowCam_->Update(player_->GetPos());
	lightGroup_->Update();
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
	StartTarget_->PostDrawScene();


	DrawTexture_ = StartTarget_->GetTextureNum(0);

	ImGui::Begin("SimpleEffect");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Checkbox("GB", &isGB_);
	ImGui::Checkbox("Monocrome", &isMono_);
	ImGui::Checkbox("Mosaic", &isMosaic_);
	ImGui::End();
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

}

void GameScene::Finalize()
{

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
