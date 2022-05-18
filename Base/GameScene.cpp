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
	cam->Init(Vector3(0, 10, -30), Vector3(0, 1, 0));
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
	fighterModel->CreateModel("sphere");
	domeObj->SetModel(dome.get());
	SphereA = make_unique<Object3D>();
	SphereA->Init();
	SphereA->SetModel(fighterModel.get());
	SphereA->SetCamera(cam.get());
	SphereA->SetLightGroup(lightGroup.get());

	SphereB = make_unique<Object3D>();
	SphereB->Init();
	SphereB->SetModel(fighterModel.get());
	SphereB->SetCamera(cam.get());
	SphereB->SetLightGroup(lightGroup.get());


	Vector3 startPos(-10, 0, 0);
	SphereB->SetPosition(startPos);
	partTex = TextureMgr::Instance()->SpriteLoadTexture(L"Resources/texture.png");
	groundModel = make_unique<Model>();
	groundModel->CreateModel("ground");
	objGround = make_unique<Object3D>();
	objGround->Init();
	Vector3 tmp;
	tmp = objGround->GetPosition();
	objGround->SetPosition(tmp + Vector3(0, -1, 0));
	objGround->SetModel(groundModel.get());
	objGround->SetCamera(cam.get());
	objGround->SetLightGroup(lightGroup.get());

	power = 2.0f;
	gravity = 9.8f / 60.0f;
	airResistance = 0.05f;
	friction = 0.0f;
	XMVECTOR angleV{ 3, 3, 0, 0 };
	angleV = XMVector3Normalize(angleV);
	XMStoreFloat3(&startAngle, angleV);
	jumpVector = Vector3();
}

void GameScene::Update()
{
	XMFLOAT3 pos = SphereB->GetPosition();
	if (Input::Instance()->KeyTrigger(DIK_SPACE))
	{
		isStart = true;
		jumpVector = startAngle * power;
	}
	if (isStart)
	{
		jumpVector.y += -gravity;

		Vector3 force = AirResistance(jumpVector, airResistance);
		jumpVector += force;
		pos = Vector3(pos) + jumpVector;
	}

	//着地したときの挙動
	if (pos.y <= 0.0f)
	{
			pos.y = -pos.y;
			jumpVector.y = -jumpVector.y;

			if (fabs(jumpVector.y) <= 0.5f)
			{
				jumpVector.y = 0;
				pos.y = 0.0f;
			}

	}

	SphereB->SetPosition(pos);
	lightGroup->Update();
	cam->Update();
	domeObj->Update();
	SphereB->Update();
	objGround->Update();


}

void GameScene::Draw()
{
	objGround->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	domeObj->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	SphereB->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GameScene::Finalize()
{
}

Vector3 GameScene::AirResistance(const Vector3 &velocity, const float k)
{
	Vector3 resistanceForce = k * -velocity;
	return resistanceForce;
}
