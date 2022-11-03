#include "GravityPlayer.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "Planet.h"
#include "PlanetManager.h"
#include "gameConstData.h"
#include "../ShadowPipeline.h"
#include "../Collision/Collision.h"
#include "DirectInput.h"
#include "../Shake.h"
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

	isOneWayGravity_ = false;

	//すべてのじゅうりょくを下向きで初期化						   下向き
	oneWayGravityAngle_ = gravityAngle_ = worldGravity_ = -YVec;
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


	if (status == PlayerStatus::STAND)
	{
		PlanetManager::Instance()->playerStand(basePlanet);
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

	//重力操作時
	if (GameInput::Instance()->GrabTrigger())
	{
		//じゅうりょくを解除する時
		if (isOneWayGravity_)
		{//速度をゼロにする
			moveVec_ *= 0.3f;
		}
		isOneWayGravity_ = !isOneWayGravity_;
		if (GameInput::Instance()->LockOnInput())
		{
			oneWayGravityAngle_ = cam->GetAngle();
		}
		else
		{
			oneWayGravityAngle_ = cam->GetAngle();
		}
	}
	FloorMove(isSetAngle);

}

void GravityPlayer::PosUpdate(const Vector3 &move)
{
	Vector3 shakeVec;

#pragma region jump
	//ジャンプ中の処理
	if (status == PlayerStatus::JUMP)
	{
		//球とプレイヤーの距離
		Vector3 dist = drawObject.GetPosition() - basePlanet.lock()->GetPos();


		//垂直に落下する成分
		moveVec_ += (gravityAngle_ * gravity);

		//出せる最高速度
		const float maxSpeed = 3.0f;
		//カメラがシェイクを始める速度
		const float shakeSpeed = 2.0f;

		//現在の速度
		float nowSpeed = moveVec_.length();
		if (nowSpeed >= shakeSpeed)
		{
			float shakePower = (0.1f * (nowSpeed - shakeSpeed));
			shakeUpdate(shakePower);
		}


		if (nowSpeed >= maxSpeed)
		{
			moveVec_ = (moveVec_.normalize() * maxSpeed);
		}
		if (dist.length() <= basePlanet.lock()->GetScale())
		{
			status = PlayerStatus::STAND;
		}
	}

	//ジャンプした瞬間
	if (status == PlayerStatus::STAND && GameInput::Instance()->ATrigger())
	{
		//球面に対しての法線方向
		Vector3 JumpAngle;

		//立っている面の垂直方向に跳躍
		Vector3 dist = drawObject.GetPosition() - basePlanet.lock()->GetPos();
		JumpAngle = (dist.normalize());
		//垂直方向に加速
		moveVec_ = (JumpAngle * jumpPower);

		//ジャンプ状態に移行
		status = PlayerStatus::JUMP;
	}


#pragma endregion

	//惑星に引き寄せられる状態か
	bool isPlanetAttracts = !(status == PlayerStatus::JUMP && isOneWayGravity_);

	//現在位置
	Vector3 nowPos(drawObject.GetPosition());

	//上陸中の惑星との距離
	Vector3 dist = (nowPos + (move * maxMoveSpeed)) - basePlanet.lock()->GetPos();

	//移動後の座標を計算（現在位置に跳躍移動と水平移動の速度を加算）
	nowPos += (moveVec_ + warkVec_);


	//地上に立っているとき
	if (status == PlayerStatus::STAND)
	{
		float length = basePlanet.lock()->GetScale();

		//基本の惑星から距離を測って球面に沿って移動
		dist = dist.normalize() * length;

		//座標決定
		nowPos = (basePlanet.lock()->GetPos() + dist);
	}
	drawObject.SetPosition(nowPos);

}

void GravityPlayer::PostureUpdate(const Vector3 &move)
{


	//球面上における上方向
	Vector3 up;

	if (status == PlayerStatus::STAND)
	{
		up = Vector3(drawObject.GetPosition() - basePlanet.lock()->GetPos()).normalize();
	}
	else
	{
		up = -gravityAngle_;
	}
	//ベクター型にする
	XMVECTOR upV = XMLoadFloat3(&up.normalize());

	//右
	XMVECTOR rightV = {};

	rightV = XMVector3Cross(upV, drawObject.GetFrontVec());

	//正面
	XMVECTOR frontV = {};
	//移動していない状態でも姿勢制御をするためのifだねえ

	if (move.length() > 0.0f)
	{
		rightV = XMVector3Cross(upV, XMLoadFloat3(&move));

		frontV = XMVector3Cross(rightV, upV);
	}
	else
	{
		rightV = XMVector3Cross(upV, drawObject.GetFrontVec());

		frontV = XMVector3Cross(drawObject.GetRightVec(), upV);
	}

	//通常移動時の向きの変化
	drawObject.SetRotationVector(frontV, upV);

}

void GravityPlayer::FloorMove(bool isSetAngle)
{
	XMFLOAT2 stick = GameInput::Instance()->LStick();

	Vector3 stickInputVector3(stick.x, 0, stick.y);
	//スティックの入力をベクターに入れる
	Vector3 move = stickInputVector3;

	//カメラの行列から入力を画面の向きに合わせて矯正

	XMVECTOR moveV = XMLoadFloat3(&move);

	//入力の姿勢を作る
	XMVECTOR moveUp = drawObject.GetUpVec();
	XMVECTOR moveFront = XMLoadFloat3(&Vector3(cam->GetRight()).cross(moveUp));
	XMMATRIX moveMat = GetMatRot(moveUp, moveFront);

	//姿勢に合わせてベクトルを配置
	XMVECTOR moveRot = XMQuaternionRotationMatrix(moveMat);
	moveV = XMVector3Rotate(moveV, moveRot);
	XMStoreFloat3(&move, moveV);

	//一応正規化
	move.normalize();

	//歩行移動
	warkVec_ = (move * maxMoveSpeed);



	if (isOneWayGravity_)
	{
		gravityAngle_ = oneWayGravityAngle_;
	}
	else if (worldGravity_.length() > 0.0f)
	{

		gravityAngle_ = worldGravity_;
	}
	else
	{
		Vector3 dist = drawObject.GetPosition() - basePlanet.lock()->GetPos();
		gravityAngle_ = -(dist.normalize());
	}
	PosUpdate(move);

	if (isSetAngle)
	{
		PostureUpdate(move);
	}
}

void GravityPlayer::JumpMove(bool isSetAngle)
{

}

void GravityPlayer::PlayerRotation()
{
	//プレイヤーのローカルのX軸に対して回転
	drawObject.AddRotation(XMQuaternionRotationAxis(drawObject.GetRightVec(), GameInput::Instance()->RStick().y * RotRate));

	//世界のY軸でなく自分の立っている垂直方向に最終的に変更したいね
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
	if (status == PlayerStatus::JUMP) return;
	Vector3 BasePlanetToPlayer = drawObject.GetPosition() - basePlanet.lock()->GetPos();
	XMVECTOR BasePlanetToPlayerAngleV = XMLoadFloat3(&BasePlanetToPlayer.normalize());
	XMVECTOR frontVec = XMVector3Cross(drawObject.GetRightVec(), BasePlanetToPlayerAngleV);

	drawObject.SetRotationVector(frontVec, BasePlanetToPlayerAngleV);
}

void GravityPlayer::AddGravity(Vector3 gravity)
{
	worldGravity_ = gravity;
}

void GravityPlayer::shakeUpdate(float shakePower)
{
	cam->SetShift(Shake::GetShake(shakePower));
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

const XMFLOAT3 &GravityPlayer::GetGravityAngle()
{
	return gravityAngle_;
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
			XMVECTOR identity = { 2.0f,2.0f ,2.0f ,0.0f };
			onPlayerPos += identity * drawObject.GetUpVec();
			drawObject.SetPosition(Vector3(onPlayerPos));
		}
		////押し返す判定
		//Collision::CheckSphere2Triangle();
	}
}
