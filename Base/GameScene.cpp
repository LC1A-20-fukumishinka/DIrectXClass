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

using namespace FukuMath;
using namespace DirectX;
using namespace std;
void GameScene::Init()
{
	cam = make_unique<Camera>();
	cam->Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
	light = unique_ptr<Light>(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({ 0, 1, 10 });
	light->SetLightActive(true);
	light->Update();


	lightGroup = unique_ptr<LightGroup>(LightGroup::Create());
	lightGroup->SetLight(light.get());
	domeObj = make_unique<Object3D>();
	domeObj->SetPosition(XMFLOAT3(0, 0, 0));
	domeObj->Init();
	domeObj->SetCamera(cam.get());
	domeObj->SetLightGroup(lightGroup.get());

	dome = make_unique<Model>();
	fighterModel = make_unique<Model>();

	dome->CreateModel("skydome");
	fighterModel->CreateModel("chr_sword");
	domeObj->SetModel(dome.get());
	objFighter = make_unique<Object3D>();
	objFighter->Init();
	objFighter->SetModel(fighterModel.get());
	objFighter->SetCamera(cam.get());
	objFighter->SetLightGroup(lightGroup.get());
	objFighter->SetRotation(XMFLOAT3{ 0, PI / 2, 0 });
	Vector3 startPos(-3, 0, 0);
	objFighter->SetPosition(startPos);
	partTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	groundModel = make_unique<Model>();
	groundModel->CreateModel("ground");
	objGround = make_unique<Object3D>();
	objGround->Init();
	objGround->SetModel(groundModel.get());
	objGround->SetCamera(cam.get());
	objGround->SetLightGroup(lightGroup.get());

	power = 2.0f;
	gravity = 9.8f / 60.0f;
	airResistance = 0.0f;
	friction = 0.0f;
	XMVECTOR angleV{ 1, 10, 0, 0 };
	angleV = XMVector3Normalize(angleV);
	XMStoreFloat3(&startAngle, angleV);
	jumpVector = {};
}

void GameScene::Update()
{
	XMFLOAT3 pos = objFighter->GetPosition();
	if (Input::Instance()->KeyTrigger(DIK_SPACE))
	{
		isStart = true;
		jumpVector = startAngle * power;
	}
	if (isStart)
	{
		jumpVector.y += -gravity;
		pos = Vector3(pos) + jumpVector;
	}

	if (pos.y <= 0.0f)
	{
		if (jumpVector.length() <= 0.3f)
		{
			jumpVector = {};
			pos.y = 0.0f;
		}
		else
		{
			pos.y = -pos.y;
			jumpVector.y = -jumpVector.y;
			jumpVector *= 0.3;
		}
	}

	objFighter->SetPosition(pos);
	lightGroup->Update();
	cam->Update();
	domeObj->Update();
	objFighter->Update();
	objGround->Update();


}

void GameScene::Draw()
{
	objGround->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	domeObj->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	objFighter->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GameScene::Finalize()
{
}
