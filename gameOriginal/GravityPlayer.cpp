#include "GravityPlayer.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "Planet.h"
#include "gameConstData.h"
using namespace GameInput;
using namespace DirectX;
using namespace FukuMath;
using namespace GameDatas;
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
	//惑星を掴んでいないかどうか
	bool isNotGrab = grabPlanet.expired();

	if (!isNotGrab)
	{//掴んでいる
		GrabUpdate();
	}
	else if (LockOnInput())
	{//ロックオン用の移動
	LockOnUpdate();
	}
	else
	{//通常状態
		NormalUpdate();
	}
}

void GravityPlayer::Finalize()
{
}

void GravityPlayer::Draw()
{
	drawObject.Update();
	drawObject.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GravityPlayer::Move(bool isSetAngle)
{
	XMFLOAT2 stick = LStick();
	//スティックの入力をベクターに入れる
	Vector3 move = { stick.x,0, stick.y };

	//カメラの行列から入力を画面の向きに合わせて矯正
	XMVECTOR moveV = XMLoadFloat3(&move);
	XMVECTOR camRot = XMQuaternionRotationMatrix(cam->GetMatBillboardY()) ;
	moveV =XMVector3Rotate(moveV, camRot);
	XMStoreFloat3(&move, moveV);

	//Y軸移動を修正
	move.y = 0;
	Vector3 nowPos(drawObject.GetPosition());

	//向きの変化
	if (isSetAngle && move.length() > 0.0f)
	{
		XMVECTOR inputVec = XMLoadFloat3(&move);
		drawObject.SetRotationVector(inputVec);
	}
	drawObject.SetPosition(nowPos + move);
}

void GravityPlayer::PlayerRotation()
{
	//プレイヤーのローカルのX軸に対して回転
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), RStick().y * RotRate));
	//ToDo///////////////
	//世界のY軸でなく自分の立っている垂直方向に最終的に変更したいね
	////////////////////
	//世界のY軸に対して回転
	drawObject.AddRotation(XMFLOAT3{ 0.0f, RStick().x * RotRate, 0.0 });
}

void GravityPlayer::NormalUpdate()
{
	//フィールドが球体になったらそれに合わせて変化
	XMVECTOR angle = drawObject.GetFrontVec();
	angle.m128_f32[1] = 0.0f;
	drawObject.SetRotationVector(angle);
	Move(true);
}

void GravityPlayer::LockOnUpdate()
{
	Move(false);
	PlayerRotation();
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
	this->cam = cam;
	drawObject.SetCamera(cam);
}

void GravityPlayer::SetLight(LightGroup *lights)
{
	drawObject.SetLightGroup(lights);
}

const DirectX::XMFLOAT3 &GravityPlayer::GetPos()
{
	return drawObject.GetPosition();
}

const XMFLOAT3 GravityPlayer::GetAngle()
{
	XMFLOAT3 angle;
	XMStoreFloat3(&angle, drawObject.GetFrontVec());
	return angle;
}

void GravityPlayer::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	if (grabPlanet.expired())
	{
		this->grabPlanet = planet;
		Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
		baseLength = planetPlayerDistance.length();
	}
	grabPlanet.lock()->GrabInput();
	XMFLOAT3 dist = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
	XMVECTOR vec = XMLoadFloat3(&dist);
	//惑星に向き直る
	drawObject.SetRotationVector(vec);
	grabPlanet.lock()->SetGrabRotateAngle(YVec, drawObject.GetRightVec());
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
	Move(false);

	//星とプレイヤーの二点間の距離を計算
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();

	//二点間の長さを計算
	float length = baseLength;
	if (false)
	{
#pragma region Loap
		length = planetPlayerDistance.length();
		//二点間の距離が掴んだ瞬間より短いか
		if (length <= baseLength)
		{//短い場合
			drawObject.SetRotationVector(XMLoadFloat3(&planetPlayerDistance));
		}
		else
		{
			length = baseLength;
		}
#pragma endregion
	}

	PlayerRotation();

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
	grabPlanet.lock()->SetGrabRotateAngle(YVec, drawObject.GetRightVec());

}
