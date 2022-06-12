#include "Planet.h"
#include "Camera.h"
#include "LightGroup.h"
#include "ModelPhongPipeline.h"
#include "FukuMath.h"
#include "gameOriginal/GameInput.h"
using namespace std;
using namespace FukuMath;
using namespace DirectX;
using namespace GameInput;
Camera *Planet::cam = nullptr;
LightGroup *Planet::lights = nullptr;
Planet::Planet()
{
	model = make_unique<Model>();

	//model->CreateModel("sphere");
	model->CreateModel("chr_sword");//‰ñ“]Šm”F—p
	object = make_unique<Object3D>();
	object->Init();
	object->SetModel(model.get());
	GrabRotateAxis = {0.0f, 1.0f, 0.0f , 0.0f};
}

Planet::~Planet()
{
}

void Planet::Init()
{
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
	object->SetPosition(pos);
	Vector3 tmp;
	tmp = object->GetRotation();
	tmp.y += degree;
	object->SetRotation(tmp);
	object->SetCamera(cam);
	object->SetLightGroup(lights);
}

void Planet::GrabUpdate()
{
	XMVECTOR Rot = XMQuaternionRotationAxis(GrabRotateAxis, RStick().x * degree);
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

void Planet::SetGrabRotateAngle(const XMVECTOR Axis)
{
	GrabRotateAxis = Axis;
}
