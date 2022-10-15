#include "GravityPlayer.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "Planet.h"
#include "gameConstData.h"
#include "../ShadowPipeline.h"
#include "../Collision/Collision.h"
#include "DirectInput.h"
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
	drawObject.SetPosition(Vector3(YVec));

	shadowObject.Init();
	shadowObject.SetModel(model);
	shadowObject.SetPosition(Vector3(YVec));
	SetBasePlanet(planet);

	localHeight = 0;
	status = PlayerStatus::STAND;

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
	else if (GameInput::Instance()->LockOnInput())
	{//ロックオン用の移動
		LockOnUpdate();
	}
	else
	{//通常状態
		NormalUpdate();
	}

	shadowObject.SetPosition(drawObject.GetPosition());
	shadowObject.SetRotation(drawObject.GetRotQuaternion());
}

void GravityPlayer::Finalize()
{
}

void GravityPlayer::Draw()
{
	drawObject.Update();
	drawObject.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void GravityPlayer::ShadowDraw()
{
	shadowObject.Update();
	shadowObject.modelDraw(ShadowPipeline::Instance()->GetPipeLine());
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
	XMFLOAT2 stick = GameInput::Instance()->LStick();

#pragma region jump
	if (status == PlayerStatus::STAND && GameInput::Instance()->ATrigger())
	{
		jumpSpeed = jumpPower;
		status = PlayerStatus::JUMP;
	}

	if (status == PlayerStatus::JUMP)
	{
		localHeight += jumpSpeed;
		jumpSpeed += gravity;
		if (localHeight <= 0)
		{
			localHeight = 0;
			status = PlayerStatus::STAND;
		}
	}
#pragma endregion

	Vector3 stickInputVector3(stick.x, 0, stick.y);
	//スティックの入力をベクターに入れる
	Vector3 move = stickInputVector3;

	//カメラの行列から入力を画面の向きに合わせて矯正
	XMVECTOR moveV = XMLoadFloat3(&move);
	//XMVECTOR camRot = XMQuaternionRotationMatrix(cam->GetMatBillboard());

	XMVECTOR moveUp = drawObject.GetUpVec();
	XMVECTOR moveFront;
	GetMatRot(XMLoadFloat3(&cam->GetRight()), moveUp, moveFront);
	XMMATRIX moveMat = GetMatRot( moveUp , moveFront);
	XMVECTOR camRot = XMQuaternionRotationMatrix(moveMat);
	moveV = XMVector3Rotate(moveV, camRot);
	XMStoreFloat3(&move, moveV);
	move.normalize();

	move *= maxMoveSpeed;
	Vector3 nowPos(drawObject.GetPosition());

	Vector3 dist = (nowPos + move) - basePlanet.lock()->GetPos();

	float length = basePlanet.lock()->GetScale() + localHeight;
	//基本の惑星から距離を測って
	dist = dist.normalize() * length;
	drawObject.SetPosition(basePlanet.lock()->GetPos() + dist);
	Vector3 up = {0.0f, 1.0f, 0.0f};

	//球面上における上方向
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

	//通常移動時の向きの変化
	if (move.length() > 0.0f)
	{
		drawObject.SetRotationVector(frontV, upV);
	}
}

void GravityPlayer::JumpMove(bool isSetAngle)
{
	//最終的に地上の処理と分割するから取り合えkズ記述しとくよ
	//if (status == PlayerStatus::JUMP)
	//{
	//	localHeight += jumpSpeed;
	//	jumpSpeed += gravity;
	//	if (localHeight <= 0)
	//	{
	//		localHeight = 0;
	//		status = PlayerStatus::STAND;
	//	}
	//}
}

void GravityPlayer::PlayerRotation()
{
	//プレイヤーのローカルのX軸に対して回転
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), GameInput::Instance()->RStick().y * RotRate));
	//ToDo///////////////
	//世界のY軸でなく自分の立っている垂直方向に最終的に変更したいね
	////////////////////
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetUpVec(), GameInput::Instance()->RStick().x * RotRate));

}

void GravityPlayer::NormalUpdate()
{
	//フィールドが球体になったらそれに合わせて変化

	if (GameInput::Instance()->LockOnRelease())
	{
		PostureReset();
	}
	Move(true);
}

void GravityPlayer::PostureReset()
{
	Vector3 BasePlanetToPlayer = drawObject.GetPosition() - basePlanet.lock()->GetPos();
	XMVECTOR BasePlanetToPlayerAngleV = XMLoadFloat3(&BasePlanetToPlayer.normalize());
	XMVECTOR frontVec = XMVector3Cross(drawObject.GetRightVec(), BasePlanetToPlayerAngleV);

	drawObject.SetRotationVector(frontVec, BasePlanetToPlayerAngleV);
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
	shadowObject.SetModel(model);
}

void GravityPlayer::SetCamera(Camera *cam)
{
	this->cam = cam;
	drawObject.SetCamera(cam);
}

void GravityPlayer::SetShadowCamera(Camera *cam)
{
	shadowObject.SetCamera(cam);
}

void GravityPlayer::SetLight(LightGroup *lights)
{
	drawObject.SetLightGroup(lights);
	shadowObject.SetLightGroup(lights);
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

const bool GravityPlayer::GetIsJump()
{
	bool isJump = status == PlayerStatus::JUMP;
	return isJump;
}

void GravityPlayer::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	if (grabPlanet.expired())
	{
		this->grabPlanet = planet;
		Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
		baseLength = planetPlayerDistance.length();
	}

	if (!grabPlanet.expired())
	{
		grabPlanet.lock()->GrabOn();
		XMFLOAT3 dist = grabPlanet.lock()->GetPos() - drawObject.GetPosition();
		XMVECTOR vec = XMLoadFloat3(&dist);
		//惑星に向き直る
		drawObject.SetRotationVector(vec, drawObject.GetUpVec());
		grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec(), drawObject.GetRightVec());
	}
}

void GravityPlayer::SetBasePlanet(std::shared_ptr<Planet> planet)
{
	//元の惑星が
	if (!this->basePlanet.expired())
	{
		//高さの調整
		float basePlanetToPlayerLength = (drawObject.GetPosition() - planet->GetPos()).length();

		//惑星の半径を引いて高さ完成
		basePlanetToPlayerLength -= planet->GetScale();

		localHeight = basePlanetToPlayerLength;

		//ジャンプの速度修正
		jumpSpeed = 0;
		this->basePlanet.lock()->ReleaseBase();
	}

	this->basePlanet = planet;
	basePlanet.lock()->OnPlayer();
	PostureReset();

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
	//とりあえず動けないようにしますバグるからね
	//Move(false);

	//星とプレイヤーの二点間の距離を計算
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - drawObject.GetPosition();



	const float moveLengthSpeed = 0.1f;
	if (GameInput::Instance()->B())
	{
		baseLength -= moveLengthSpeed;
	}

	if (GameInput::Instance()->X())
	{
		baseLength += moveLengthSpeed;
	}
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
	grabPlanet.lock()->SetGrabRotateAngle(drawObject.GetUpVec(), drawObject.GetRightVec());

}

void GravityPlayer::BlockCollision(const std::vector<Triangle> &boxPlanes)
{
	//プレイヤーの落下のray
	XMVECTOR downVec = -drawObject.GetUpVec();
	Ray playerDownVec;
	playerDownVec.dir = downVec;
	playerDownVec.start = XMLoadFloat3(&(drawObject.GetPosition() + (drawObject.GetUpVec() * 0.5f)));

	for (auto &e : boxPlanes)
	{
		float distance = 1000;
		DirectX::XMVECTOR onPlayerPos;
		bool isOnPlayer = false;
		//乗る判定
		//法線方向がプレイヤーの重力方向と内積をとって１に近いかどうかを判定
		if (XMVector3Dot(downVec, -e.normal).m128_f32[0] >= 0.7f)
		{
		//例と足場の接触判定
			isOnPlayer = Collision::CheckRay2Triangle(playerDownVec, e, &distance, &onPlayerPos);
		}

		if (isOnPlayer)
		{
			int hoge = 1;
		}
		if (isOnPlayer && distance <= 1.1f)
		{
			XMVECTOR identity = {2.0f,2.0f ,2.0f ,0.0f };
			onPlayerPos += identity * drawObject.GetUpVec();
			drawObject.SetPosition(Vector3(onPlayerPos));
			jumpSpeed = 0.0f;
		}
		////押し返す判定
		//Collision::CheckSphere2Triangle();
	}
}
