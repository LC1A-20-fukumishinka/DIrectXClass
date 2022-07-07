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
using namespace FukuMath;
using namespace DirectX;
using namespace std;
void GameScene::Init()
{
	cam = make_unique<GameCamera>();
	cam->Init();
	light = unique_ptr<Light>(Light::Create());
	light->SetLightColor({ 0.9f, 0.9f, 0.9f });
	light->SetLightDir({ 1.0f,-1.0f ,1.0f ,0.0f });
	light->SetLightActive(true);
	light->Update();

	pointLight = make_unique<PointLight>();
	pointLight->SetActive(true);
	pointLight->SetLightPos(XMFLOAT3{ 0, 1.5, 0 });
	pointLight->SetLightAtten(XMFLOAT3(0.2f, 0.2f, 0.2f));

	spotLight = unique_ptr<SpotLight>(SpotLight::Create());
	spotLight->SetLightDir({ -0.0f, -1.0f, 0.0f });
	spotLight->SetLightPos({ 0.0f, 1.0f, 0.0f });
	spotLight->SetLightAtten({ 0.01f, 0.01f, 0.01f });
	spotLight->SetActive(true);

	lightGroup = unique_ptr<LightGroup>(LightGroup::Create());
	lightGroup->SetLight(light.get());
	lightGroup->SetPoinntLight(pointLight.get());
	lightGroup->SetSpotLight(spotLight.get());

	lightGroup->SetAmbientColor(XMFLOAT3{ 0.1f,0.1f, 0.1f });
	groundModel = make_unique<Model>();
	groundModel->CreateModel("ground");
	playerModel = make_unique<Model>();
	playerModel->CreateModel("chr_sword");
	domeModel = make_unique<Model>();
	domeModel->CreateModel("skydome");

	objDome = make_unique<Object3D>();
	objDome->Init();
	objDome->SetModel(domeModel.get());
	objDome->SetCamera(cam->GetCamera());
	objDome->SetScale(XMFLOAT3(3.0f, 3.0f, 3.0f));
	//objDome->SetLight(light.get());
	objDome->SetLightGroup(lightGroup.get());
	objGround = make_unique<Object3D>();
	objGround->Init();
	objGround->SetModel(groundModel.get());
	objGround->SetCamera(cam->GetCamera());
	objGround->SetLight(light.get());
	objGround->SetLightGroup(lightGroup.get());

	Planet::SetCamera(cam->GetCamera());
	Planet::SetLight(lightGroup.get());

	PlanetManager::Instance()->Init();
	for (int i = 0; i < 6; i++)
	{
		PlanetManager::Instance()->AddPlanet(XMFLOAT3{ static_cast<float>(30 * i) + 60, 40.0f, 0 }, 10.0f, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
	}

	PlanetManager::Instance()->AddPlanet(XMFLOAT3{ static_cast<float>(30 * 6) + 60, 40.0f, 30.0f }, 10.0f, DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));

	PlanetManager::Instance()->AddPlanet(XMFLOAT3{ 300, 70.0f, 0 }, 50.0f, DirectX::XMFLOAT4(1.0f, 0.6f, 0.4f, 1.0f));
	player = make_unique<GravityPlayer>();
	player->Init(playerModel.get(), PlanetManager::Instance()->GetBasePlanet());
	player->SetCamera(cam->GetCamera());
	player->SetLight(lightGroup.get());
}

void GameScene::Update()
{
	//スクショ用停止ポーズ
	if (Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_START))
	{
		isPause = !isPause;
	}
	if (isPause) return;
	GameInput::Instance()->Update();
	lightGroup->Update();

	//掴む
	if (GameInput::Instance()->LockOnInput())
	{

		if (GameInput::Instance()->GrabInput())
		{
			XMFLOAT3 tmp = cam->GetCameraPos();
			bool isCollision;
			shared_ptr<Planet> grabPlanet;

			isCollision = PlanetManager::Instance()->GetGrabPlanet(grabPlanet, tmp, cam->GetCamera()->GetAngle());
			if (isCollision)
			{
				player->SetGrabPlanet(grabPlanet);
			}
		}
	}
	//離す
	if (!GameInput::Instance()->GrabInput())
	{
		player->ReleasePlanet();
	}

	MovePlanet();
	player->Update();
	objDome->SetPosition(player->GetPos());
	objDome->Update();

	objGround->Update();
	PlanetManager::Instance()->Update();
	//star->Update();
	//temple->Update();
	cam->SetBasePlanet(player->GetBasePlanet());
	cam->Update(player->GetPos(), player->GetAngle(), player->GetUpVec());


}

void GameScene::Draw()
{
	objDome->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	DepthReset();
	PlanetManager::Instance()->Draw();
	player->Draw();
	//objGround->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GameScene::Finalize()
{

}

void GameScene::MovePlanet()
{
	if (player->GetIsJump())
	{
		shared_ptr<Planet> basePlanet;
		bool isMove = false;
		isMove = PlanetManager::Instance()->MovePlanet(basePlanet, player->GetPos());

		if (isMove)
		{
			player->SetBasePlanet(basePlanet);
			cam->IsAnimationOn();
		}
	}
	else if (cam->GetIsChangeBasePlanet())
	{
		cam->StartCameraAnimation(false, 60);
	}
}
