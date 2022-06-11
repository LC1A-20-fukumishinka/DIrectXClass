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
using namespace FukuMath;
using namespace DirectX;
using namespace std;
using namespace GameInput;
void GameScene::Init()
{
	star = make_unique<Planet>();

	cam = make_unique<Camera>();
	cam->Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
	light = unique_ptr<Light>(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightActive(false);
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

	groundModel = make_unique<Model>();
	groundModel->CreateModel("ground");
	playerModel = make_unique<Model>();
	playerModel->CreateModel("chr_sword");
	objGround = make_unique<Object3D>();
	objGround->Init();
	objGround->SetModel(groundModel.get());
	objGround->SetCamera(cam.get());
	objGround->SetLight(light.get());
	objGround->SetLightGroup(lightGroup.get());

	Planet::SetCamera(cam.get());
	Planet::SetLight(lightGroup.get());

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
	player->Init(playerModel.get());
	player->SetCamera(cam.get());
	player->SetLight(lightGroup.get());
}

void GameScene::Update()
{

	cam->Update();
	lightGroup->Update();
	if (B())
	{
		player->SetGrabPlanet(star);
	}
	else
	{
		player->ReleasePlanet();
	}
	player->Update();
	if (Input::Instance()->Key(DIK_W))
	{
		star->SetPos(Vector3(0, 1, 0) + star->GetPos());
	}
	if (Input::Instance()->Key(DIK_S))
	{
		star->SetPos(Vector3(0, -1, 0) + star->GetPos());
	}
	objGround->Update();
	star->Update();
	//temple->Update();


}

void GameScene::Draw()
{
	star->Draw();
	player->Draw();
	//temple->Draw();
	objGround->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GameScene::Finalize()
{

}