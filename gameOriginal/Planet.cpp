#include "Planet.h"
#include "../Camera.h"
#include "../LightGroup.h"
#include "ModelPhongPipeline.h"
#include "FukuMath.h"
#include "GameInput.h"
#include "gameConstData.h"
using namespace std;
using namespace FukuMath;
using namespace DirectX;
using namespace GameDatas;
Camera *Planet::cam = nullptr;
LightGroup *Planet::lights = nullptr;
Planet::Planet()
{
	model = make_unique<Model>();

	model->CreateModel("sphere");
	//model->CreateModel("chr_sword");//‰ñ“]Šm”F—p
	object = make_unique<Object3D>();
	object->Init();
	object->SetModel(model.get());
	object->SetCamera(Planet::cam);
	object->SetScale(XMFLOAT3(scale, scale, scale));

	GrabRotateAxisY = YVec;
	GrabRotateAxisX = XVec;
}

Planet::~Planet()
{
}

void Planet::Init(const DirectX::XMFLOAT3 &pos, float size, const DirectX::XMFLOAT4 &color)
{
	SetScale(size);
	this->pos = pos;
	object->SetPosition(pos);
	object->SetColor(color);
}

void Planet::Update()
{
	if (!isGrab)
	{
		NormalUpdate();
	}
	else
	{
		GrabUpdate();
	}
}

void Planet::NormalUpdate()
{
	//Vector3 tmp = {};
	//tmp.y += degree;
	//object->AddRotation(tmp);
	object->SetPosition(pos);
	object->SetCamera(cam);
	object->SetLightGroup(lights);
}

void Planet::GrabUpdate()
{
	XMVECTOR Rot = XMQuaternionRotationAxis(GrabRotateAxisY, GameInput::Instance()->RStick().x * RotRate);
	Rot = XMQuaternionMultiply( Rot, XMQuaternionRotationAxis(GrabRotateAxisX, GameInput::Instance()->RStick().y * RotRate));
	object->AddRotation(Rot);
	object->SetCamera(cam);
	object->SetLightGroup(lights);
	object->SetPosition(pos);
}

void Planet::Draw()
{
	object->Update();
	object->modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void Planet::Finalize()
{
}

void Planet::SetScale(float scale)
{
	this->scale = scale;
	object->SetScale(XMFLOAT3(scale, scale, scale));
}

float Planet::GetScale()
{
	return scale;
}

void Planet::SetLight(LightGroup *lights)
{
	Planet::lights = lights;
}

void Planet::SetCamera(Camera *cam)
{
	Planet::cam = cam;
}

void Planet::GrabInput()
{
	isGrab = true;
}

void Planet::ReleaseGrab()
{
	isGrab = false;
}

void Planet::OnPlayer()
{
	isBase = true;
}

void Planet::ReleaseBase()
{
	isBase = false;
}

void Planet::SetGrabRotateAngle(const DirectX::XMVECTOR AxisY, const DirectX::XMVECTOR AxisX)
{
	GrabRotateAxisY = AxisY;
	GrabRotateAxisX = AxisX;
}

bool Planet::GetBase()
{
	return isBase;
}
