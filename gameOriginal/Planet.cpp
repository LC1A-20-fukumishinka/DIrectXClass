#include "Planet.h"
#include "../Camera.h"
#include "../LightGroup.h"
#include "ModelPhongPipeline.h"
#include "../ShadowDrawPipeline.h"
#include "FukuMath.h"
#include "GameInput.h"
#include "gameConstData.h"
using namespace std;
using namespace FukuMath;
using namespace DirectX;
using namespace GameDatas;
Camera *Planet::cam = nullptr;
Camera *Planet::shadowCam = nullptr;
int Planet::shadowTextureNum;
LightGroup *Planet::lights = nullptr;
Planet::Planet()
{
	model = make_unique<Model>();

	model->CreateModel("sphere");
	//model->CreateModel("chr_sword");//‰ñ“]Šm”F—p
	object = make_unique<PlanetObject>();
	object->Init();
	object->SetModel(model.get());
	object->SetCamera(Planet::cam);
	object->SetScale(XMFLOAT3(scale, scale, scale));
	object->SetShadowCamera(Planet::shadowCam);
	object->SetShadowTextureNum(shadowTextureNum);
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
	object->SetShadowCamera(shadowCam);
	object->SetShadowTextureNum(shadowTextureNum);
}

void Planet::GrabUpdate()
{
	XMVECTOR Rot = XMQuaternionRotationAxis(GrabRotateAxisY, GameInput::Instance()->RStick().x * RotRate);
	Rot = XMQuaternionMultiply(Rot, XMQuaternionRotationAxis(GrabRotateAxisX, GameInput::Instance()->RStick().y * RotRate));
	object->AddRotation(Rot);
	object->SetCamera(cam);
	object->SetLightGroup(lights);
	object->SetPosition(pos);
	object->SetShadowCamera(shadowCam);
	object->SetShadowTextureNum(shadowTextureNum);
}

void Planet::Draw()
{
	object->Update();
	object->modelDraw(ShadowDrawPipeline::Instance()->GetPipeLine());
}

void Planet::Finalize()
{
}

void Planet::SetScale(float scale)
{
	this->scale = scale;
	object->SetScale(XMFLOAT3(scale, scale, scale));
}

Object3D *Planet::GetObject3d()
{
	return object.get();
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

void Planet::SetShadowCamera(Camera *shadowCam)
{
	Planet::shadowCam = shadowCam;
}

void Planet::SetShadowTexture(int shadowTextureNum)
{
	Planet::shadowTextureNum = shadowTextureNum;
}

void Planet::GrabOn()
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
