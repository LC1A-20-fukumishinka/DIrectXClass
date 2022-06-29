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
void GravityPlayer::Init(Model *model, std::shared_ptr<Planet> planet)
{
	drawObject.Init();
	drawObject.SetModel(model);
	SetBasePlanet(planet);

	status = STAND;

	if (!basePlanet.expired())
	{
		Vector3 startPos;
		startPos = basePlanet.lock()->GetPos();
		startPos += YVec * basePlanet.lock()->GetScale();
		drawObject.SetPosition(startPos);
	}
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
	//switch (status)
	//{
	//case STAND:
	//	FloorMove(isSetAngle);
	//	break;
	//case JUMP:
	//	JumpMove(isSetAngle);
	//	break;
	//default:
	//	break;
	//}

	FloorMove(isSetAngle);

}

void GravityPlayer::FloorMove(bool isSetAngle)
{
	XMFLOAT2 stick = LStick();
	Vector3 stickInputVector3(stick.x, 0, stick.y);
	//スティックの入力をベクターに入れる
	Vector3 move = stickInputVector3;

	//カメラの行列から入力を画面の向きに合わせて矯正
	XMVECTOR moveV = XMLoadFloat3(&move);
	XMVECTOR camRot = XMQuaternionRotationMatrix(cam->GetMatBillboard());
	moveV = XMVector3Rotate(moveV, camRot);
	XMStoreFloat3(&move, moveV);
	move.normalize();

	move *= maxMoveSpeed;
	Vector3 nowPos(drawObject.GetPosition());

	Vector3 dist = (nowPos + move) - basePlanet.lock()->GetPos();

	//基本の惑星から距離を測って
	dist = dist.normalize() * basePlanet.lock()->GetScale();
	drawObject.SetPosition(basePlanet.lock()->GetPos() + dist);

	Vector3 up = {};

	up = drawObject.GetPosition() - basePlanet.lock()->GetPos();

	//法線方向
	XMVECTOR upV = XMLoadFloat3(&up.normalize());


	XMVECTOR rightV = {};
	//右取得

	if (!isSetAngle)
	{
		rightV = XMVector3Cross(upV, drawObject.GetFrontVec());
	}
	else
	{
		rightV = XMVector3Cross(upV, moveV);
	}

	XMVECTOR frontV = {};

	frontV = XMVector3Cross(rightV, upV);
	//Y軸移動を修正
	//move.y = 0;

	//通常移動時の向きの変化
	if (move.length() > 0.0f)
	{
		drawObject.SetRotationVector(frontV, upV);
	}
}

void GravityPlayer::JumpMove(bool isSetAngle)
{

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
	//XMVECTOR playerUp = drawObject.GetFrontVec();
	//playerUp.m128_f32[1] = 0.0f;
	//drawObject.SetRotationVector(playerUp);
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

const XMFLOAT3 GravityPlayer::GetUpVec()
{
	XMFLOAT3 playerUp;
	XMStoreFloat3(&playerUp, drawObject.GetUpVec());
	return playerUp;
}

const float GravityPlayer::GetBasePlanetScale()
{
	if (basePlanet.expired())
	{
		return 0.0f;
	}
	return basePlanet.lock()->GetScale();
}

const std::weak_ptr<Planet> &GravityPlayer::GetBasePlanet()
{
	return basePlanet;
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

void GravityPlayer::SetBasePlanet(std::shared_ptr<Planet> planet)
{
	this->basePlanet = planet;
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
