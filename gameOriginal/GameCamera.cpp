#include "GameCamera.h"
#include "Vector3.h"
#include "FukuMath.h"
#include "gameConstData.h"
#include "GameInput.h"
#include <algorithm>
#include "Planet.h"
#include "DirectInput.h"
#include "PlanetManager.h"
#include"../imgui/ImguiManager.h"
using namespace FukuMath;
using namespace GameDatas;
using namespace DirectX;

void GameCamera::Init()
{
	cam_.Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
	CameraAnimationEase_.Init(60);

	Reset();
}

void GameCamera::Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec, GameDatas::PlayerStatus playerStatus)
{
	//playerPos_ = playerPos;
	playerStatus_ = playerStatus;
	if (isTitleMode_)
	{
		TitleUpdate();
	}
	else if (isClearMode_)
	{
		ClearCameraUpdate();
	}
	else
	{
		IngameCameraUpdate(playerPos, playerZVec, playerYVec);
	}

	if (isChangeBasePlanetAnimation_ && !isCameraStop_)
	{
		CameraAnimationUpdate();
	}
	else if (!isCameraStop_)
	{
		//次の対象の場所に即移動する
		cam_.SetEye(nextEyePos_);
		//基本の上ベクトルを回転させて確認
		//cam_.up = XMVector3Rotate(F3toV(playerYVec), nextCamUpRot_);
		cam_.up = XMVector3Rotate(YVec, nextCamUpRot_);

		//次のターゲット座標に移動
		cam_.SetTarget(nextTargetPos_);
	}

	oldIsOneWayGravity_ = gravity_.isOneWayGravity;

	cam_.Update();

	ImguiUpdate();
}

void GameCamera::Draw()
{
}

void GameCamera::Finalize()
{
}

void GameCamera::SetGravityData(GameDatas::GravityData gravity)
{
	gravity_ = gravity;
}

Camera *GameCamera::GetCamera()
{
	return &cam_;
}

Vector3 GameCamera::GetCameraPos()
{
	return cam_.GetEye();
}

bool GameCamera::GetIsCameraStop()
{
	return isCameraStop_;
}

void GameCamera::CameraStop()
{
	isCameraStop_ = true;
}

void GameCamera::SetBasePlanet(const std::weak_ptr<Planet> planet)
{
	this->planet_ = planet;
}

void GameCamera::StartCameraAnimation(bool isTargetEase, int EaseTimer)
{
	isCameraStop_ = false;
	isChangeBasePlanetAnimation_ = true;
	this->isTargetEase_ = isTargetEase;
	CameraAnimationEase_.Init(EaseTimer);
	oldEyePos_ = cam_.GetEye();
	oldTargetPos_ = cam_.GetTarget();
	XMVECTOR frontVec = F3toV((cam_.GetTarget() - cam_.GetEye()));
	XMVECTOR upVec = F3toV(cam_.up);
	oldCamUpRot_ = XMQuaternionRotationMatrix(FukuMath::GetMatRot(upVec, frontVec));
}

void GameCamera::ClearAnimationStart(const Vector3 &playerPos)
{
	//nextTargetPos = planet.lock()->GetPos();
	nextTargetPos_ = nextPlanetPos_;

	Vector3 eyeDir = cam_.GetEye() - planet_.lock()->GetPos();

	//向きが存在しなかった場合の修正
	if (eyeDir.length() == 0.0f)
	{
		eyeDir = Vector3{ 0, 0, 1 };
	}

	//距離をとる
	eyeDir = eyeDir.normalize();
	eyeDir *= 300.0f;

	//基準点から動かしてワールド座標を作成
	nextEyePos_ = eyeDir + planet_.lock()->GetPos();

	//上ベクトルをしゅせう性
	nextCamUpRot_ = XMQuaternionIdentity();

	//状態の設定
	isClearMode_ = true;

	//アニメーション開始処理
	StartCameraAnimation(true, 90);
}

void GameCamera::TitleAnimationStart()
{
	//カメラの向きを初期化
	Vector3 eyeDir = Vector3{ 0, -0.4f, 1 };

	//正規化
	eyeDir = eyeDir.normalize();
	XMMATRIX rot = GetMatRot(YVec, F3toV(eyeDir));

	//ターゲットの位置を修正
	nextTargetPos_ = Vector3(0, 0, 0);

	//基準点から動かしてワールド座標を作成
	nextEyePos_ = (eyeDir * -sTitleCameraDistance) + nextTargetPos_;

	//上ベクトルをしゅせう性
	nextCamUpRot_ = XMQuaternionRotationMatrix(rot);
	StartCameraAnimation(true, 60);
}

void GameCamera::ClearToIngme(const Vector3& playerUp)
{
	nextCamUpRot_ = XMQuaternionRotationMatrix(GetMatRot(F3toV(playerUp), F3toV(cam_.GetAngle())));
	isClearMode_ = false;
	StartCameraAnimation(true, 30);
}

void GameCamera::TitleToIngame(const Vector3 &playerPos, const Vector3 &playerZVec)
{
	isTitleMode_ = false;
	nextEyePos_ = playerPos - (playerZVec * 10);
	nextCamUpRot_ = XMQuaternionIdentity();
	StartCameraAnimation(true, 30);
}

void GameCamera::NormalUpdate(const Vector3 &playerPos, const Vector3 &playerYVec)
{
	//注視点をプレイヤの頭に合わせる
	nextTargetPos_ = playerPos;
	nextTargetPos_ += playerYVec.normalize();

	//カメラの座標計算（距離を出す）
	Vector3 camPos;

	//重力解除時
	if (!gravity_.isOneWayGravity && oldIsOneWayGravity_)
	{//落下方向にカメラを向かせる
		Vector3 cameraToPlayerDistance = planet_.lock()->GetPos() - nextTargetPos_;
		camPos += nextTargetPos_ + (-cameraToPlayerDistance.normalize() * nowDistance);
		nextCamUpRot_ = XMQuaternionRotationMatrix(GetMatRot(F3toV(cam_.up), F3toV(cameraToPlayerDistance.normalize())));

		StartCameraAnimation(false, 120);
	}
	else
	{
		//カメラからプレイヤーへの距離計算
		Vector3 cameraToPlayerDistance = nextEyePos_ - nextTargetPos_;

		//長さを測ってプレイヤーとの距離をクランプして距離調整
		float length = nowDistance;

		//向きに直して
		Vector3 camAngle = cameraToPlayerDistance.normalize();
		//向き＊距離で相対座標を計算
		camPos = camAngle * length;
		//ターゲットの位置を加算してカメラのワールド座標完成
		camPos += nextTargetPos_;
	}

	nextEyePos_ = camPos;

	if (playerStatus_ == PlayerStatus::STAND)
	{
		//現在の回転
		nextCamUpRot_;

		//立ち姿勢における回転
		XMVECTOR playerStandRot;
		playerStandRot = XMQuaternionRotationMatrix(GetMatRot(F3toV(playerYVec), F3toV(cam_.GetAngle())));

		//上記二つの回転をSlerp
		nextCamUpRot_ = XMQuaternionSlerp(nextCamUpRot_, playerStandRot, 0.03f);
	}

	camRot(GameInput::Instance()->RStick(), playerYVec);
}

void GameCamera::SetNextPlantPos(const Vector3 &pos)
{
	nextPlanetPos_ = pos;
}

bool GameCamera::GetIsAnimationEnd()
{
	return CameraAnimationEase_.IsEnd();
}

void GameCamera::CameraReflesh(Vector3 UpVec, int timer)
{
	nextCamUpRot_ = XMQuaternionRotationMatrix(GetMatRot(F3toV(UpVec), F3toV(cam_.GetAngle())));
	StartCameraAnimation(false, timer);
}

void GameCamera::LandingCameraReflesh(Vector3 UpVec)
{
	if (!imCamLanding_) return;
	nextCamUpRot_ = XMQuaternionRotationMatrix(GetMatRot(F3toV(UpVec), F3toV(cam_.GetAngle())));
	StartCameraAnimation(false, 60);
}

void GameCamera::Reset()
{
	nextEyePos_ = Vector3(0, 10, -15);
	nextTargetPos_ = Vector3(0, 3, 0);
	nextCamUpRot_ = XMQuaternionRotationMatrix(GetMatRot(YVec, F3toV((nextTargetPos_ - nextEyePos_))));
	gravity_.angle = -YVec;
	isTitleMode_ = true;
}

void GameCamera::CameraAnimationUpdate()
{
	//easing
	float t = CameraAnimationEase_.Do(Easing::Out, Easing::Cubic);
	//移動処理

	//距離をとって
	Vector3 eyeDist = nextEyePos_ - oldEyePos_;
	//距離をイージングの値で乗算
	eyeDist *= t;
	//元の視点にさっきの距離を加算
	cam_.SetEye(oldEyePos_ + eyeDist);

	if (isTargetEase_)
	{
		//距離をとって
		Vector3 targetDist = nextTargetPos_ - oldTargetPos_;
		//距離をイージングの値で乗算
		targetDist *= t;
		//元の視点にさっきの距離を加算
		cam_.SetTarget(oldTargetPos_ + targetDist);
	}
	else
	{
		cam_.SetTarget(nextTargetPos_);
	}

	//回転処理
	XMVECTOR animationRot = XMQuaternionSlerp(oldCamUpRot_, nextCamUpRot_, t);
	XMStoreFloat3(&cam_.up, XMVector3Rotate(YVec, animationRot));
	//処理終わり
	if (CameraAnimationEase_.IsEnd())
	{
		isChangeBasePlanetAnimation_ = false;
	}
}

void GameCamera::LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	//たいしょうのざひょうをプレイヤーに
	nextTargetPos_ = playerPos;
	//少しずらす
	XMVECTOR cameraAngleRot = XMQuaternionRotationMatrix(cam_.GetMatBillboard());
	Vector3 shift(XMVector3Rotate(LockOnShift, cameraAngleRot));
	nextTargetPos_ += shift;
	cam_.SetTarget(nextTargetPos_);

	Vector3 camPos = -playerZVec;
	float length = 3.0f;

	camPos = camPos.normalize();
	camPos *= length;
	camPos += nextTargetPos_;
	nextEyePos_ = camPos;

	XMVECTOR frontV = F3toV((nextTargetPos_ - nextEyePos_));
	nextCamUpRot_ = XMQuaternionRotationMatrix(FukuMath::GetMatRot(F3toV(playerYVec), F3toV(playerZVec)));
}

void GameCamera::GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec)
{
}

void GameCamera::TitleUpdate()
{
	XMVECTOR angle = XMVector3Rotate(ZVec, nextCamUpRot_);
	//追加する回転
	XMVECTOR addRot = XMQuaternionRotationAxis(YVec, degree / 4.0f);
	angle = XMVector3Rotate(angle, addRot);
	//基準点から動かしてワールド座標を作成
	nextEyePos_ = (angle * -sTitleCameraDistance) + nextTargetPos_;

	nextCamUpRot_ = XMQuaternionMultiply(nextCamUpRot_, addRot);
}

void GameCamera::ClearCameraUpdate()
{

}

void GameCamera::IngameCameraUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	//注視点状態切り替えアニメーション
	if (GameInput::Instance()->LockOnTrigger() || GameInput::Instance()->LockOnRelease())
	{
		StartCameraAnimation(true, 15);
	}

	//状態毎のカメラのアップデート
	if (GameInput::Instance()->LockOnInput())
	{//ロックオン中
		//ロックオン時アップデート
		LockonUpdate(playerPos, playerZVec, playerYVec);
	}
	else
	{//通常時

		//右スティック押し込みによるカメラの姿勢リフレッシュ
		if (GameInput::Instance()->CameraReflesh())
		{
			CameraReflesh(playerYVec, 15);
		}

		//通常時アップデート
		NormalUpdate(playerPos, playerYVec);

		if (imCamCollision_)
		{
			PlanetCollisionUpdate();
		}
	}



	if (GameInput::Instance()->LockOnRelease())
	{
		nextEyePos_ = (playerPos - playerZVec * 100);
	}
}

void GameCamera::PlanetCollisionUpdate()
{
	//カメラの対象座標から視点方向へのレイ
	Ray cameraRay;

	//カメラレイの情報用意
	Vector3 cameraDir = Vector3(nextEyePos_ - nextTargetPos_);
	cameraRay.start = F3toV(nextTargetPos_);
	cameraRay.dir = F3toV(cameraDir.normalize());

	//カメラの距離の最大値より少し大きく
	float colDistance = GameDatas::camMaxLength + 1.0f;

	Sphere planetCol;

	//衝突判定
	float NearPlanetDistance = (Vector3(cameraRay.start) - planet_.lock()->GetPos()).length();
	NearPlanetDistance -= planet_.lock()->GetScale();
	if (colDistance > NearPlanetDistance)
	{
		planetCol.center = F3toV(planet_.lock()->GetPos());
		planetCol.radius = planet_.lock()->GetScale();
		Collision::CheckRay2Sphere(cameraRay, planetCol, &colDistance);
	}

	//めり込んだ際などに距離がゼロになってバグが発生するので距離がゼロにならないようにする
	if (colDistance > 0.0f && colDistance <= rateDistance)
	{
		nowDistance = colDistance;
		CToPCollAnimationRate = 0.0f;
	}

	//接触した際の距離と現在の距離を比較
	if (nowDistance < colDistance)
	{//接触地点の方が長かった場合カメラの視点座標を最大値にする
		CToPCollAnimationRate += (1.0f / 120.0f);
	}

	CToPCollAnimationRate = std::clamp(CToPCollAnimationRate, 0.0f, 1.0f);

	float easeRate = Easing::easeInOutQuint(CToPCollAnimationRate);

	if (!imCamBackFlag_)
	{
		easeRate = 1.0f;
	}
	rateDistance = (nowDistance * (1 - easeRate)) + (colDistance * easeRate);

	nextEyePos_ = nextTargetPos_ + (cameraDir.normalize() * rateDistance);
}

void GameCamera::camRot(const DirectX::XMFLOAT2 &rot, Vector3 playerUp)
{
	//回転姿勢のクォータニオン
	XMVECTOR rotQ = XMQuaternionIdentity();
	//回転用X軸
	XMVECTOR rotX = XMVector3Rotate(F3toV(cam_.GetRight()), XMQuaternionIdentity());
	//回転用Y軸
	XMVECTOR rotY = XMVector3Rotate(F3toV(cam_.up), XMQuaternionIdentity());
	if (playerStatus_ == PlayerStatus::STAND)
	{
		rotY = XMVector3Rotate(F3toV(playerUp), XMQuaternionIdentity());
	}

	rotQ = XMQuaternionMultiply(nextCamUpRot_, XMQuaternionRotationAxis(rotX, rot.y * RotRate));

	rotQ = XMQuaternionMultiply(rotQ, XMQuaternionRotationAxis(rotY, rot.x * RotRate));

	nextCamUpRot_ = rotQ;
	//targetからnextPosの視点座標までのベクトルを計算
	//XMVECTOR nextVec = F3toV((nextEyePos_ - nextTargetPos_));

	float length = (nextEyePos_ - nextTargetPos_).length();
	XMVECTOR nextVec = -ZVec;

	//回転させる
	nextVec = XMVector3Rotate(nextVec, rotQ);

	nextVec *= length;
	Vector3 camPos;
	XMStoreFloat3(&camPos, nextVec);
	//targetの座標＋生成したベクトルを合わせて回転完了
	camPos += nextTargetPos_;
	nextEyePos_ = camPos;
}

void GameCamera::ImguiUpdate()
{
	ImGui::Begin("Cam");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Checkbox("Back", &imCamBackFlag_);
	ImGui::Checkbox("Collision", &imCamCollision_);
	ImGui::Checkbox("Landing", &imCamLanding_);
	ImGui::End();
}
