#include "GravityPlayer.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "../Planet.h"
using namespace GameInput;
using namespace DirectX;
using namespace FukuMath;
static float testRot = 0;
static Easing RotX;
static Easing RotY;
bool isX = false;
bool isY = false;
void GravityPlayer::Init(Model *model)
{
	drawObject.Init();
	drawObject.SetModel(model);
	drawObject.SetRotation(XMFLOAT3(45 * degree, 0, 0));
	RotX.Init(60);
	RotY.Init(120);
}

void GravityPlayer::Update()
{
	float x = static_cast<float>(RotX.Do(Easing::InOut, Easing::Sine)) - 0.5f;
	float y = static_cast<float>(RotY.Do(Easing::InOut, Easing::Sine)) - 0.5f;

	if (isX)
	{
		x = -x;
	}
	if (isY)
	{
		y = -y;
	}

	x = 45 * degree;

	y = y * 90 * degree;
	if (RotX.IsEnd())
	{
		RotX.Init(120);
		isX = !isX;
	}
	if (RotY.IsEnd())
	{
		RotY.Reset();
		isY = !isY;
	}
	XMFLOAT2 stick = LStick();
	Vector3 move = { stick.x,0, stick.y };
	Vector3 nowPos(drawObject.GetPosition());
	drawObject.SetPosition(nowPos + move);

	drawObject.AddRotation(XMFLOAT3{ 0, degree,0 });
}

void GravityPlayer::Finalize()
{
}

void GravityPlayer::Draw()
{
	drawObject.Update();
	drawObject.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GravityPlayer::SetPos(const DirectX::XMFLOAT3 &pos)
{
	drawObject.SetPosition(pos);
}

void GravityPlayer::SetRotation(const DirectX::XMFLOAT3 &rot)
{
	drawObject.SetRotation(rot);
}

void GravityPlayer::SetModel(Model *model)
{
	drawObject.SetModel(model);
}

void GravityPlayer::SetCamera(Camera *cam)
{
	drawObject.SetCamera(cam);
}

void GravityPlayer::SetLight(LightGroup *lights)
{
	drawObject.SetLightGroup(lights);
}

const DirectX::XMFLOAT3 GravityPlayer::GetPos()
{
	return drawObject.GetPosition();
}

void GravityPlayer::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	this->grabPlanet = planet;
	grabPlanet.lock()->Grab();
}

void GravityPlayer::ReleasePlanet()
{
	bool isNotGrab = grabPlanet.expired();
	if (isNotGrab)return;
	grabPlanet.lock()->ReleaseGrab();
	grabPlanet.reset();
}

void GravityPlayer::GrabUpdate()
{
	bool isNotGrab = grabPlanet.expired();
	if (isNotGrab)return;

}
