#include "Flag.h"
#include "ModelPhongPipeline.h"
#include "Model.h"
#include "Planet.h"
#include "../ShadowPipeline.h"
#include "../Collision/Collision.h"
using namespace DirectX;
std::unique_ptr<Model> Flag::sFlagModel;
Camera *Flag::sShadowCamera = nullptr;
Flag::Flag()
{
	if (!sFlagModel)
	{
		sFlagModel = std::make_unique<Model>();
		sFlagModel->CreateModel("Flag");
	}
}

Flag::~Flag()
{
}

void Flag::Init(std::weak_ptr<Planet> basePlanet, Vector3 angle, float scale)
{
	ObjectOnSphere::Init(basePlanet, angle, scale);
	SetModel(sFlagModel.get());
}

void Flag::Update()
{
	ObjectOnSphere::Update();
}

void Flag::DrawShadow()
{
	//shadowObject_.Update();
	//shadowObject_.modelDraw(ShadowPipeline::Instance()->GetPipeLine());
}

void Flag::Finalize()
{
}

void Flag::Draw()
{
	if (isDraw_)
	{
		ObjectOnSphere::Draw();
	}
}

void Flag::SetShadowCamera(Camera *camera)
{
	sShadowCamera = camera;
}

bool Flag::CollisionPlayer(float radius, const Vector3 pos)
{
	Sphere playerSphere, flagSphere;
	playerSphere.center = XMLoadFloat3(&pos);
	playerSphere.radius = radius;
	flagSphere.center = XMLoadFloat3(&GetWorldPos());
	flagSphere.radius = GetWorldScale();

	bool isCollision = Collision::CheckSphere2Sphere(playerSphere, flagSphere);

	if (isCollision)
	{
		//è¡Ç¶ÇÈèàóù
		isDraw_ = false;
	}
	return isCollision;
}

bool Flag::GetIsDraw()
{
	return isDraw_ && ObjectOnSphere::GetIsPlanetSpawn();
}

void Flag::Reset()
{
	isDraw_ = true;
}
