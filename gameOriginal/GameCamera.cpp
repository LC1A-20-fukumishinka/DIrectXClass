#include "GameCamera.h"
#include "Vector3.h"
#include "FukuMath.h"
#include "gameConstData.h"
#include "GameInput.h"
#include <algorithm>
#include "Planet.h"
#include "DirectInput.h"
using namespace FukuMath;
using namespace GameDatas;
using namespace DirectX;

const float GameCamera::sTitleCameraDistance = 200.0f;
void GameCamera::Init()
{
	cam_.Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
	CameraAnimationEase_.Init(60);

	nextEyePos_ = cam_.GetEye();
	nextCamUpRot_ = XMQuaternionIdentity();
}

void GameCamera::Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	if (Input::Instance()->KeyTrigger(DIK_0))
	{
		TitleAnimationStart();
	}

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
		//cam_.up = XMVector3Rotate(XMLoadFloat3(&playerYVec), nextCamUpRot_);
		cam_.up = XMVector3Rotate(YVec, nextCamUpRot_);

		//次のターゲット座標に移動
		cam_.SetTarget(nextTargetPos_);
	}


	cam_.Update();
}

void GameCamera::Draw()
{
}

void GameCamera::Finalize()
{
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
	XMVECTOR frontVec = XMLoadFloat3(&(cam_.GetTarget() - cam_.GetEye()));
	XMVECTOR upVec = XMLoadFloat3(&cam_.up);
	oldCamUpRot_ = XMQuaternionRotationMatrix(FukuMath::GetMatRot( upVec, frontVec));
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
	XMMATRIX rot = GetMatRot(YVec, XMLoadFloat3( &eyeDir));

	//ターゲットの位置を修正
	nextTargetPos_ = Vector3(0, 0, 0);

	//基準点から動かしてワールド座標を作成
	nextEyePos_ = (eyeDir * -sTitleCameraDistance) + nextTargetPos_;

	//上ベクトルをしゅせう性
	nextCamUpRot_ = XMQuaternionRotationMatrix(rot);
	//nextCamUpRot = XMQuaternionIdentity();
	StartCameraAnimation(true, 60);
}

void GameCamera::ClearToIngme()
{
	isClearMode_ = false;
	StartCameraAnimation(true, 30);
}

void GameCamera::TitleToIngame(const Vector3 &playerPos, const Vector3 &playerZVec)
{
	isTitleMode_ = false;
	nextEyePos_ = playerPos - (playerZVec * 10);
	StartCameraAnimation(true, 30);
}

void GameCamera::NormalUpdate(const Vector3 &playerPos)
{
	nextTargetPos_ = playerPos;
	nextTargetPos_.y += 1.0f;
	cam_.SetTarget(nextTargetPos_);
	Vector3 camPos = nextEyePos_ - nextTargetPos_;
	float length = camPos.length();


	//プレイヤーとの距離をクランプ
	length = std::clamp(length, camMinRength, camMaxRength);
	//プレイヤー->カメラ(向き)
	Vector3 camAngle = camPos.normalize();
	//距離を補正
	camPos = camAngle * length;
	//ターゲットの位置を加算してカメラの座標完成
	camPos += nextTargetPos_;

	//float camUpLength = 5;
	//Vector3 basePlanetToCameraAngle = { 0, 1, 0 };

	//if (!planet_.expired())
	//{
	//	//惑星→カメラ
	//	basePlanetToCameraAngle = camPos - planet_.lock()->GetPos();
	//	basePlanetToCameraAngle.normalize();

	//	camUpLength += planet_.lock()->GetScale();

	//	basePlanetToCameraAngle *= camUpLength;

	//	camPos = planet_.lock()->GetPos();

	//	camPos += basePlanetToCameraAngle;
	//}

	////上記の処理でできたベクトルで回転行列を作成
	//XMMATRIX camUpMat = FukuMath::GetMatRot(XMLoadFloat3(&camAngle), XMLoadFloat3(&basePlanetToCameraAngle.normalize()));
	//quaternion化
	//nextCamUpRot_ = XMQuaternionRotationMatrix(camUpMat);
	nextEyePos_ = camPos;



	camRot(GameInput::Instance()->RStick());
}

void GameCamera::SetNextPlantPos(const Vector3 &pos)
{
	nextPlanetPos_ = pos;
}

bool GameCamera::GetIsAnimationEnd()
{
	return CameraAnimationEase_.IsEnd();
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

	XMVECTOR frontV = XMLoadFloat3(&(nextTargetPos_ - nextEyePos_));
	nextCamUpRot_ = XMQuaternionRotationMatrix(FukuMath::GetMatRot(XMLoadFloat3(&playerYVec), XMLoadFloat3(&playerZVec)));
}

void GameCamera::GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec)
{
}

void GameCamera::TitleUpdate()
{
	XMVECTOR angle = XMVector3Rotate(ZVec, nextCamUpRot_);
	//追加する回転
	XMVECTOR addRot = XMQuaternionRotationAxis(YVec, degree / 4);
	angle = XMVector3Rotate(angle, addRot);
	//基準点から動かしてワールド座標を作成
	nextEyePos_ = (angle * -sTitleCameraDistance) + nextTargetPos_;

	nextCamUpRot_ = XMQuaternionMultiply(nextCamUpRot_, addRot);
}

void GameCamera::ClearCameraUpdate()
{
	//float t = (1.0f - CameraAnimationEase.Read());

	//float rotRate = 0.15f + (t * 4.0f);
	//camRot(XMFLOAT2(-rotRate, 0));
}

void GameCamera::IngameCameraUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	if (GameInput::Instance()->LockOnTrigger() || GameInput::Instance()->LockOnRelease())
	{
		StartCameraAnimation(true, 15);
	}

	if (/*GameInput::Instance()->GrabInput()*/ false)
	{
		LockonUpdate(playerPos, playerZVec, playerYVec);
	}
	else if (GameInput::Instance()->LockOnInput())
	{
		LockonUpdate(playerPos, playerZVec, playerYVec);
	}
	else
	{
		if (GameInput::Instance()->CameraReflesh())
		{
			nextCamUpRot_ = XMQuaternionRotationMatrix(GetMatRot(XMLoadFloat3(&playerYVec), XMLoadFloat3(&cam_.GetAngle())));
			StartCameraAnimation(false, 15);
		}
		NormalUpdate(playerPos);

	}



	if (GameInput::Instance()->LockOnRelease())
	{
		nextEyePos_ = (playerPos - playerZVec * 100);
	}
}

void GameCamera::camRot(const DirectX::XMFLOAT2 &rot)
{
	XMVECTOR rotQ = XMQuaternionIdentity();
	XMVECTOR rotX = XMVector3Rotate(XMLoadFloat3(&cam_.GetRight()), XMQuaternionIdentity());
	XMVECTOR rotY = XMVector3Rotate(XMLoadFloat3(&cam_.up), XMQuaternionIdentity());
	rotQ = XMQuaternionMultiply(nextCamUpRot_, XMQuaternionRotationAxis(rotX, rot.y * RotRate));

	rotQ = XMQuaternionMultiply(rotQ, XMQuaternionRotationAxis(rotY, rot.x * RotRate));

	nextCamUpRot_ = rotQ;
	//targetからnextPosの視点座標までのベクトルを計算
	//XMVECTOR nextVec = XMLoadFloat3(&(nextEyePos_ - nextTargetPos_));

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
