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
}

void GravityPlayer::Update()
{
	XMFLOAT2 stick = LStick();
	Vector3 move = { stick.x,0, stick.y };
	Vector3 nowPos(drawObject.GetPosition());
	drawObject.SetPosition(nowPos + move);

	GrabUpdate();
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
	grabPlanet.lock()->GrabInput();
	XMFLOAT3 dist = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
	XMVECTOR vec = XMLoadFloat3(&dist);
	//惑星に向き直る
	drawObject.SetRotationVector(vec);
	grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec());
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

	//星とプレイヤーの二点間の距離を計算
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();

	//二点間の長さを計算
	float length = planetPlayerDistance.length();

	//プレイヤーのローカルのX軸に対して回転
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), RStick().y * degree));
	//ToDo///////////////
	//世界のY軸でなく自分の立っている垂直方向に最終的に変更したいね
	////////////////////
	//世界のY軸に対して回転
	drawObject.AddRotation(XMFLOAT3{ 0.0f, RStick().x * degree, 0.0});

	//プレイヤーの正面方向に
	XMVECTOR vec = drawObject.GetFrontVec();

	//移動量を乗算する
	vec *= length;

	//それをプレイヤーの位置に加算して
	XMVECTOR playerPos = XMLoadFloat3(&drawObject.GetPosition());
	vec += playerPos;

	//惑星の位置完成
	XMFLOAT3 planetPos = {};
	XMStoreFloat3(&planetPos, vec);
	grabPlanet.lock()->SetPos(planetPos);

}
