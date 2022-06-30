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
#include"../gameOriginal/GameInput.h"
#include "../Collision/Collision.h"
#include "../gameOriginal/PlanetManager.h"
using namespace FukuMath;
using namespace DirectX;
using namespace std;
using namespace GameInput;
void GameScene::Init()
{
	cam = make_unique<GameCamera>();
	cam->Init();
	light = unique_ptr<Light>(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
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

	lightGroup->SetAmbientColor(XMFLOAT3{0.3f,0.3f, 0.3f });
	groundModel = make_unique<Model>();
	groundModel->CreateModel("ground");
	playerModel = make_unique<Model>();
	playerModel->CreateModel("chr_sword");
	objGround = make_unique<Object3D>();
	objGround->Init();
	objGround->SetModel(groundModel.get());
	objGround->SetCamera(cam->GetCamera());
	objGround->SetLight(light.get());
	objGround->SetLightGroup(lightGroup.get());

	Planet::SetCamera(cam->GetCamera());
	Planet::SetLight(lightGroup.get());

	PlanetManager::Instance()->Init();
	for (int i = 0; i < 10; i++)
	{
		PlanetManager::Instance()->AddPlanet(XMFLOAT3{ static_cast<float>(2 * i), 55.0f, 0 }, 3.0f);
	}
	//templeModel = unique_ptr<FbxModel>(FbxLoader::GetInstance()->LoadModelFromFile("newPlayer"));
	//FbxObject3D::SetDevice();
	//FbxObject3D::SetCamera(cam.get());
	//FbxObject3D::CreateGraphicsPipeline();
	//temple = make_unique<FbxObject3D>();
	//temple->Init();
	//temple->SetModel(templeModel.get());
	//temple->PlayAnimation();
	//temple->SetRotation({ -PI/2.0f, PI , 0 });
	//temple->SetPosition(XMFLOAT3(0.0f, 3.0f, 0.0f));
	//temple->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
	player = make_unique<GravityPlayer>();
	player->Init(playerModel.get(), PlanetManager::Instance()->GetBasePlanet());
	player->SetCamera(cam->GetCamera());
	player->SetLight(lightGroup.get());
}

void GameScene::Update()
{

	lightGroup->Update();

	//’Í‚Þ
	if (LockOnInput())
	{

		if (GrabInput())
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
	//—£‚·
	if (!GrabInput())
	{
		player->ReleasePlanet();
	}
	player->Update();

	objGround->Update();
	PlanetManager::Instance()->Update();
	//star->Update();
	//temple->Update();
	cam->SetBasePlanet(player->GetBasePlanet());
	cam->Update(player->GetPos(), player->GetAngle(), player->GetUpVec());


}

void GameScene::Draw()
{
	//star->Draw();

	PlanetManager::Instance()->Draw();
	player->Draw();
	//temple->Draw();
	objGround->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GameScene::Finalize()
{

}