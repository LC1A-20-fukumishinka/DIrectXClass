#include "Player.h"
#include "ModelPhongPipeline.h"
#include "GameInput.h"
#include "FukuMath.h"
#include "../EaseClass.h"
#include "Planet.h"
#include "PlanetManager.h"
#include "../ShadowPipeline.h"
#include "../Collision/Collision.h"
#include "DirectInput.h"
#include "../Shake.h"
#include "TextureMgr.h"
#include "../imgui/ImguiManager.h"
using namespace DirectX;
using namespace FukuMath;
using namespace GameDatas;
static float testRot = 0;
static Easing RotX;
static Easing RotY;
bool isX = false;
bool isY = false;
void Player::Init(std::shared_ptr<Planet> planet)
{

	pos_ = Vector3(YVec);
	drawObject_.Init();
	drawObject_.SetModel(playerModel_.get());
	drawObject_.SetPosition(pos_);

	shadowObject_.Init();
	shadowObject_.SetModel(playerModel_.get());
	shadowObject_.SetPosition(Vector3(YVec));
	SetBasePlanet(planet);

	leftTrackObject_.Init();
	leftTrackObject_.SetModel(leftTrackModel_.get());
	leftTrackObject_.SetPosition(pos_);

	rightTrackObject_.Init();
	rightTrackObject_.SetModel(rightTrackModel_.get());
	rightTrackObject_.SetPosition(pos_);
	localHeight = 0;
	status_ = PlayerStatus::STAND;
	oldStatus_ = status_;
	if (!basePlanet.expired())
	{
		Vector3 startPos;
		startPos = basePlanet.lock()->GetPos();
		startPos += YVec * basePlanet.lock()->GetScale();
		pos_ = startPos;
		drawObject_.SetPosition(pos_);
	}

	gravity_.isOneWayGravity = false;

	//すべてのじゅうりょくを下向きで初期化						   下向き
	oneWayGravityAngle_ = gravity_.angle = worldGravity_ = -YVec;
}

void Player::Update()
{
	//惑星を掴んでいないかどうか
	bool isNotGrab = grabPlanet.expired();

	oldStatus_ = status_;

	if (!isNotGrab)
	{//掴んでいる
		GrabUpdate();
	}
	else if (GameInput::Instance()->LockOnInput())
	{//ロックオン用の移動

		if (GameInput::Instance()->LockOnInput())
		{
			drawObject_.SetRotationVector(XMLoadFloat3(&cam_->GetAngle()), drawObject_.GetUpVec());
		}
		LockOnUpdate();
	}
	else
	{//通常状態
		NormalUpdate();
	}


	if (status_ == PlayerStatus::STAND)
	{
		PlanetManager::Instance()->playerStand(basePlanet);
	}
	XMVECTOR rot = drawObject_.GetRotQuaternion();
	leftTrackObject_.SetRotation(rot);
	rightTrackObject_.SetRotation(rot);
	shadowObject_.SetRotation(rot);

	drawObject_.SetPosition(pos_);
	leftTrackObject_.SetPosition(pos_);
	rightTrackObject_.SetPosition(pos_);
	shadowObject_.SetPosition(pos_);

	FaceUpdate();
}

void Player::Finalize()
{
}

void Player::Draw()
{
	ImguiDraw();
	playerModel_->SetTexture(faceTextureHandles_[static_cast<int>(face_)]);
	drawObject_.Update();
	leftTrackObject_.Update();
	rightTrackObject_.Update();
	drawObject_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	leftTrackObject_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
	rightTrackObject_.modelDraw(ModelPhongPipeline::Instance()->GetPipeLine());
}

void Player::ShadowDraw()
{
	shadowObject_.Update();
	shadowObject_.modelDraw(ShadowPipeline::Instance()->GetPipeLine());
}

void Player::ImguiDraw()
{
	ImGui::Begin("PlayerStatus");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("speed : %f", moveVec_.length());
	if (gravity_.isOneWayGravity)
	{
		ImGui::Text("GravityPower : ON");
	}
	else
	{
		ImGui::Text("GravityPower : OFF");
	}
	ImGui::End();
}

void Player::Boost()
{
	moveVec_ += (moveVec_.normalize() * 2);
}

void Player::Move(bool isSetAngle)
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
		if (gravity_.isOneWayGravity)
		{//速度を遅くする
			moveVec_ *= 0.3f;
		}
		gravity_.isOneWayGravity = !gravity_.isOneWayGravity;
		if (GameInput::Instance()->LockOnInput())
		{
			oneWayGravityAngle_ = cam_->GetAngle();
		}
		else
		{
			oneWayGravityAngle_ = cam_->GetAngle();
		}
	}
	FloorMove(isSetAngle);

}

void Player::PosUpdate(const Vector3 &move)
{
	Vector3 shakeVec;

#pragma region jump
	//ジャンプ中の処理
	if (status_ == PlayerStatus::JUMP)
	{
		//球とプレイヤーの距離
		Vector3 dist = pos_ - basePlanet.lock()->GetPos();


		//垂直に落下する成分
		moveVec_ += (gravity_.angle * baseGravityPower);

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

		//最高速度を超えていたら速度を調整する
		if (nowSpeed >= maxSpeed)
		{//加速システム
			moveVec_ = (moveVec_.normalize() * (maxSpeed + (nowSpeed - maxSpeed) * 0.95f));
		}

		if (Input::Instance()->ButtonTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			gravity_.isOneWayGravity = false;
			Vector3 dist = pos_ - basePlanet.lock()->GetPos();

			moveVec_ = -dist.normalize() * maxSpeed;
		}
		moveVec_ += (warkVec_ * 0.02f);

		if (dist.length() <= basePlanet.lock()->GetScale())
		{
			if (nowSpeed >= shakeSpeed)
			{
				shakeTimer_ = 0;
				//重力変更をした
				isGravityChanged_ = true;
			}
			status_ = PlayerStatus::STAND;
		}
	}

	if (shakeTimer_ <= shakeTimerMax_)
	{
		float shakePower = 3.0f * (1.0f - (static_cast<float> (shakeTimer_) / shakeTimerMax_));
		shakeUpdate(shakePower);
		shakeTimer_++;
	}

	//ジャンプした瞬間
	if (status_ == PlayerStatus::STAND && GameInput::Instance()->ATrigger())
	{
		//球面に対しての法線方向
		Vector3 JumpAngle;

		//立っている面の垂直方向に跳躍
		Vector3 dist = pos_ - basePlanet.lock()->GetPos();
		JumpAngle = (dist.normalize());
		//垂直方向に加速
		moveVec_ = (JumpAngle * jumpPower);

		moveVec_ += warkVec_;
		//ジャンプ状態に移行
		status_ = PlayerStatus::JUMP;
	}


#pragma endregion

	//惑星に引き寄せられる状態か
	bool isPlanetAttracts = !(status_ == PlayerStatus::JUMP && gravity_.isOneWayGravity);

	//現在位置
	Vector3 nowPos(pos_);

	//上陸中の惑星との距離
	Vector3 dist = (nowPos + (move * maxMoveSpeed)) - basePlanet.lock()->GetPos();

	//移動後の座標を計算（現在位置に跳躍移動と水平移動の速度を加算）
	nowPos += (moveVec_);


	//地上に立っているとき球面に位置を補正する
	if (status_ == PlayerStatus::STAND)
	{
		float length = basePlanet.lock()->GetScale();

		//基本の惑星から距離を測って球面に沿って移動
		dist = dist.normalize() * length;

		//座標決定
		nowPos = (basePlanet.lock()->GetPos() + dist);
	}

	pos_ = nowPos;
}

void Player::PostureUpdate(const Vector3 &move)
{


	//球面上における上方向
	Vector3 up;

	if (status_ == PlayerStatus::STAND)
	{
		up = Vector3(pos_ - basePlanet.lock()->GetPos()).normalize();
	}
	else
	{
		up = -gravity_.angle;
	}
	//ベクター型にする
	XMVECTOR upV = XMLoadFloat3(&up.normalize());

	//右
	XMVECTOR rightV = {};

	rightV = XMVector3Cross(upV, drawObject_.GetFrontVec());

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
		rightV = XMVector3Cross(upV, drawObject_.GetFrontVec());

		frontV = XMVector3Cross(drawObject_.GetRightVec(), upV);
	}

	//通常移動時の向きの変化
	drawObject_.SetRotationVector(frontV, upV);

}

void Player::FloorMove(bool isSetAngle)
{
	XMFLOAT2 stick = GameInput::Instance()->LStick();

	Vector3 stickInputVector3(stick.x, 0, stick.y);
	//スティックの入力をベクターに入れる
	Vector3 move = stickInputVector3;

	//カメラの行列から入力を画面の向きに合わせて矯正

	XMVECTOR moveV = XMLoadFloat3(&move);

	//入力の姿勢を作る
	XMVECTOR moveUp = drawObject_.GetUpVec();
	XMVECTOR moveFront = XMLoadFloat3(&Vector3(cam_->GetRight()).cross(moveUp));
	XMMATRIX moveMat = GetMatRot(moveUp, moveFront);

	//姿勢に合わせてベクトルを配置
	XMVECTOR moveRot = XMQuaternionRotationMatrix(moveMat);
	moveV = XMVector3Rotate(moveV, moveRot);
	XMStoreFloat3(&move, moveV);

	//一応正規化
	move.normalize();

	//歩行移動
	warkVec_ = (move * maxMoveSpeed);



	if (gravity_.isOneWayGravity)
	{
		gravity_.angle = oneWayGravityAngle_;
	}
	else if (worldGravity_.length() > 0.0f)
	{

		gravity_.angle = worldGravity_;
	}
	else
	{
		Vector3 dist = pos_ - basePlanet.lock()->GetPos();
		gravity_.angle = -(dist.normalize());
	}
	PosUpdate(move);

	if (isSetAngle)
	{
		PostureUpdate(move);
	}
}

void Player::JumpMove(bool isSetAngle)
{

}

void Player::PlayerRotation()
{
	//プレイヤーのローカルのX軸に対して回転
	drawObject_.AddRotation(XMQuaternionRotationAxis(drawObject_.GetRightVec(), GameInput::Instance()->RStick().y * RotRate));

	//世界のY軸でなく自分の立っている垂直方向に最終的に変更したいね
	drawObject_.AddRotation(XMQuaternionRotationAxis(drawObject_.GetUpVec(), GameInput::Instance()->RStick().x * RotRate));

}

void Player::NormalUpdate()
{
	//フィールドが球体になったらそれに合わせて変化

	if (GameInput::Instance()->LockOnRelease())
	{
		PostureReset();
	}
	Move(true);
}

void Player::PostureReset()
{
	if (status_ == PlayerStatus::JUMP) return;
	Vector3 BasePlanetToPlayer = pos_ - basePlanet.lock()->GetPos();
	XMVECTOR BasePlanetToPlayerAngleV = XMLoadFloat3(&BasePlanetToPlayer.normalize());
	XMVECTOR frontVec = XMVector3Cross(drawObject_.GetRightVec(), BasePlanetToPlayerAngleV);

	drawObject_.SetRotationVector(frontVec, BasePlanetToPlayerAngleV);
}

void Player::AddGravity(Vector3 gravity)
{
	worldGravity_ = gravity;
}

void Player::shakeUpdate(float shakePower)
{
	cam_->SetShift(Shake::GetShake(shakePower));
}

void Player::LoadModel()
{
	playerModel_ = std::make_unique<Model>();
	playerModel_->CreateModel("oct", true, true);
	faceTextureHandles_.emplace_back(playerModel_->GetTexture());
	faceTextureHandles_.emplace_back(TextureMgr::Instance()->SpriteLoadTexture(L"Resources/oct/octBlink.1001.png"));
	faceTextureHandles_.emplace_back(TextureMgr::Instance()->SpriteLoadTexture(L"Resources/oct/octSmile.1001.png"));
	faceTextureHandles_.emplace_back(TextureMgr::Instance()->SpriteLoadTexture(L"Resources/oct/octStrive.1001.png"));
	leftTrackModel_ = std::make_unique<Model>();
	leftTrackModel_->CreateModel("leftTrack", true, true);
	rightTrackModel_ = std::make_unique<Model>();
	rightTrackModel_->CreateModel("rightTrack", true, true);
	ArrowModel_ = std::make_unique<Model>();
	ArrowModel_->CreateModel("oct", true, true);
}

bool Player::LandingCamReset()
{
	bool isReset = (status_ == PlayerStatus::STAND && oldStatus_ == PlayerStatus::JUMP && isGravityChanged_);

	if (isGravityChanged_)
	{
		isGravityChanged_ = false;
	}
	return isReset;
}

void Player::FaceUpdate()
{
	blinkTimer_--;

	if (blinkTimer_ <= 0)
	{
		if (face_ == PlayerFaceTexture::NORMAL)
		{
			face_ = PlayerFaceTexture::BLINK;
			blinkTimer_ = 20;
		}
		else if (face_ == PlayerFaceTexture::BLINK)
		{
			face_ = PlayerFaceTexture::NORMAL;
			blinkTimer_ = 240;
		}
	}
}

void Player::LockOnUpdate()
{
	Move(false);
	PlayerRotation();
}

void Player::SetPos(const DirectX::XMFLOAT3 &pos)
{
	pos_ = pos;
	drawObject_.SetPosition(pos_);
}

void Player::SetRotation(const DirectX::XMFLOAT3 &rot)
{
	drawObject_.SetRotation(rot);
}

void Player::SetModel(Model *model)
{
	drawObject_.SetModel(model);
	shadowObject_.SetModel(model);
}

void Player::SetCamera(Camera *cam)
{
	this->cam_ = cam;
	drawObject_.SetCamera(cam);
	leftTrackObject_.SetCamera(cam);
	rightTrackObject_.SetCamera(cam);
}

void Player::SetShadowCamera(Camera *cam)
{
	shadowObject_.SetCamera(cam);
}

void Player::SetLight(LightGroup *lights)
{
	drawObject_.SetLightGroup(lights);
	leftTrackObject_.SetLightGroup(lights);
	rightTrackObject_.SetLightGroup(lights);
	shadowObject_.SetLightGroup(lights);
}

const DirectX::XMFLOAT3 &Player::GetPos()
{
	return pos_;
}

const XMFLOAT3 Player::GetAngle()
{
	XMFLOAT3 angle;
	XMStoreFloat3(&angle, drawObject_.GetFrontVec());
	return angle;
}

const XMFLOAT3 Player::GetUpVec()
{
	XMFLOAT3 playerUp;
	XMStoreFloat3(&playerUp, drawObject_.GetUpVec());
	return playerUp;
}

const XMVECTOR Player::GetPosture()
{
	return drawObject_.GetRotQuaternion();
}

float Player::GetBasePlanetScale()
{
	if (basePlanet.expired())
	{
		return 0.0f;
	}
	return basePlanet.lock()->GetScale();
}

const std::weak_ptr<Planet> &Player::GetBasePlanet()
{
	return basePlanet;
}

GameDatas::GravityData Player::GetGravityData()
{
	return gravity_;
}

GameDatas::PlayerStatus Player::GetPlayerStatus()
{
	return status_;
}

bool Player::GetIsOneWayGravity()
{
	return gravity_.isOneWayGravity;
}

void Player::SetGrabPlanet(std::shared_ptr<Planet> planet)
{
	if (grabPlanet.expired())
	{
		this->grabPlanet = planet;
		Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - pos_;
		baseLength = planetPlayerDistance.length();
	}

	if (!grabPlanet.expired())
	{
		grabPlanet.lock()->GrabOn();
		XMFLOAT3 dist = grabPlanet.lock()->GetPos() - pos_;
		XMVECTOR vec = XMLoadFloat3(&dist);
		//惑星に向き直る
		drawObject_.SetRotationVector(vec, drawObject_.GetUpVec());
		grabPlanet.lock()->SetGrabRotateAngle(drawObject_.GetUpVec(), drawObject_.GetRightVec());
	}
}

void Player::SetBasePlanet(std::shared_ptr<Planet> planet)
{
	//元の惑星が
	if (!this->basePlanet.expired())
	{
		//高さの調整
		float basePlanetToPlayerLength = (pos_ - planet->GetPos()).length();

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

void Player::ReleasePlanet()
{
	bool isNotGrab = grabPlanet.expired();
	if (isNotGrab)return;
	grabPlanet.lock()->ReleaseGrab();
	grabPlanet.reset();
}

void Player::GrabUpdate()
{
	//とりあえず動けないようにしますバグるからね
	//Move(false);

	//星とプレイヤーの二点間の距離を計算
	Vector3 planetPlayerDistance = grabPlanet.lock()->GetPos() - pos_;



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
			drawObject_.SetRotationVector(XMLoadFloat3(&planetPlayerDistance));
		}
		else
		{
			length = baseLength;
		}
#pragma endregion
	}

	PlayerRotation();

	//プレイヤーの正面方向に
	XMVECTOR vec = drawObject_.GetFrontVec();

	//移動量を乗算する
	vec *= length;

	//それをプレイヤーの位置に加算して
	XMVECTOR playerPos = XMLoadFloat3(&pos_);
	vec += playerPos;

	//惑星の位置完成
	XMFLOAT3 planetPos = {};
	XMStoreFloat3(&planetPos, vec);
	grabPlanet.lock()->SetPos(planetPos);
	grabPlanet.lock()->SetGrabRotateAngle(drawObject_.GetUpVec(), drawObject_.GetRightVec());

}

void Player::BlockCollision(const std::vector<Triangle> &boxPlanes)
{
	//プレイヤーの落下のray
	XMVECTOR downVec = -drawObject_.GetUpVec();
	Ray playerDownVec;
	playerDownVec.dir = downVec;
	playerDownVec.start = XMLoadFloat3(&(pos_ + (drawObject_.GetUpVec() * 0.5f)));

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
			onPlayerPos += identity * drawObject_.GetUpVec();
			pos_ = Vector3(onPlayerPos);
		}
		////押し返す判定
		//Collision::CheckSphere2Triangle();
	}
}
