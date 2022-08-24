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
	cam.Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
	CameraAnimationEase.Init(60);

	nextEyePos = cam.GetEye();
	nextCamUpRot = XMQuaternionIdentity();
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

	if (isChangeBasePlanetAnimation)
	{
		CameraAnimationUpdate();
	}
	else if (!isChangeBasePlanet)
	{
		//次の対象の場所に即移動する
		cam.SetEye(nextEyePos);
		//基本の上ベクトルを回転させて確認
		XMStoreFloat3(&cam.up, XMVector3Rotate(YVec, nextCamUpRot));
		//次のターゲット座標に移動
		cam.SetTarget(nextTargetPos);
	}

	cam.Update();
}

void GameCamera::Draw()
{
}

void GameCamera::Finalize()
{
}

Camera *GameCamera::GetCamera()
{
	return &cam;
}

Vector3 GameCamera::GetCameraPos()
{
	return cam.GetEye();
}

bool GameCamera::GetIsChangeBasePlanet()
{
	return isChangeBasePlanet;
}

void GameCamera::SetBasePlanet(const std::weak_ptr<Planet> planet)
{
	this->planet = planet;
}

void GameCamera::StartCameraAnimation(bool isTargetEase, int EaseTimer)
{
	isChangeBasePlanet = false;
	isChangeBasePlanetAnimation = true;
	this->isTargetEase = isTargetEase;
	CameraAnimationEase.Init(EaseTimer);
	oldEyePos = cam.GetEye();
	oldTargetPos = cam.GetTarget();
	XMVECTOR frontVec = XMLoadFloat3(&(cam.GetTarget() - cam.GetEye()));
	XMVECTOR upVec = XMLoadFloat3(&cam.up);
	oldCamUpRot = XMQuaternionRotationMatrix(FukuMath::GetMatRot(frontVec, upVec));
}

void GameCamera::ClearAnimationStart(const Vector3 &playerPos)
{
	//nextTargetPos = planet.lock()->GetPos();
	nextTargetPos = nextPlanetPos_;

	Vector3 eyeDir = cam.GetEye() - planet.lock()->GetPos();

	//向きが存在しなかった場合の修正
	if (eyeDir.length() == 0.0f)
	{
		eyeDir = Vector3{ 0, 0, 1 };
	}

	//距離をとる
	eyeDir.normalize();
	eyeDir *= 300.0f;

	//基準点から動かしてワールド座標を作成
	nextEyePos = eyeDir + planet.lock()->GetPos();

	//上ベクトルをしゅせう性
	nextCamUpRot = XMQuaternionIdentity();

	//状態の設定
	isClearMode_ = true;

	//アニメーション開始処理
	IsAnimationOn();
	StartCameraAnimation(true, 90);
}

void GameCamera::TitleAnimationStart()
{
	//カメラの向きを初期化
	Vector3 eyeDir = Vector3{ 0, -0.4f, 1 };

	//正規化
	eyeDir.normalize();
	XMMATRIX rot = GetMatRot(XMLoadFloat3(&eyeDir));

	//ターゲットの位置を修正
	nextTargetPos = Vector3(0, 0, 0);

	//基準点から動かしてワールド座標を作成
	nextEyePos = (eyeDir * -sTitleCameraDistance) + nextTargetPos;

	//上ベクトルをしゅせう性
	nextCamUpRot = XMQuaternionRotationMatrix(rot);
	//nextCamUpRot = XMQuaternionIdentity();
	IsAnimationOn();
	StartCameraAnimation(true, 60);
}

void GameCamera::ClearToIngme()
{
	isClearMode_ = false;
	IsAnimationOn();
	StartCameraAnimation(true, 30);
}

void GameCamera::TitleToIngame(const Vector3 &playerPos, const Vector3 &playerZVec)
{
	isTitleMode_ = false;
	IsAnimationOn();
	nextEyePos = playerPos - (playerZVec * 10);
	StartCameraAnimation(true, 30);
}

void GameCamera::NormalUpdate(const Vector3 &playerPos)
{
	nextTargetPos = playerPos;
	nextTargetPos.y += 1.0f;
	cam.SetTarget(nextTargetPos);
	Vector3 camPos = nextEyePos - nextTargetPos;
	float length = camPos.length();


	//プレイヤーとの距離をクランプ
	length = std::clamp(length, camMinRength, camMaxRength);
	//プレイヤー->カメラ(向き)
	Vector3 camAngle = camPos.normalize();
	//距離を補正
	camPos = camAngle * length;
	//ターゲットの位置を加算してカメラの座標完成
	camPos += nextTargetPos;

	float camUpLength = 5;
	Vector3 basePlanetToCameraAngle = { 0, 1, 0 };

	if (!planet.expired())
	{
		//惑星→カメラ
		basePlanetToCameraAngle = camPos - planet.lock()->GetPos();
		basePlanetToCameraAngle.normalize();

		camUpLength += planet.lock()->GetScale();

		basePlanetToCameraAngle *= camUpLength;

		camPos = planet.lock()->GetPos();

		camPos += basePlanetToCameraAngle;
	}

	//上記の処理でできたベクトルで回転行列を作成
	XMMATRIX camUpMat = FukuMath::GetMatRot(XMLoadFloat3(&camAngle), XMLoadFloat3(&basePlanetToCameraAngle.normalize()));
	//quaternion化
	nextCamUpRot = XMQuaternionRotationMatrix(camUpMat);
	nextEyePos = camPos;

	camRot(GameInput::Instance()->RStick());
}

void GameCamera::SetNextPlantPos(const Vector3 pos)
{
	nextPlanetPos_ = pos;
}

bool GameCamera::GetIsAnimationEnd()
{
	return CameraAnimationEase.IsEnd();
}

void GameCamera::CameraAnimationUpdate()
{
	//easing
	float t = CameraAnimationEase.Do(Easing::Out, Easing::Cubic);
	//移動処理

	//距離をとって
	Vector3 eyeDist = nextEyePos - oldEyePos;
	//距離をイージングの値で乗算
	eyeDist *= t;
	//元の視点にさっきの距離を加算
	cam.SetEye(oldEyePos + eyeDist);

	if (isTargetEase)
	{
		//距離をとって
		Vector3 targetDist = nextTargetPos - oldTargetPos;
		//距離をイージングの値で乗算
		targetDist *= t;
		//元の視点にさっきの距離を加算
		cam.SetTarget(oldTargetPos + targetDist);
	}
	else
	{
		cam.SetTarget(nextTargetPos);
	}

	//回転処理
	XMVECTOR animationRot = XMQuaternionSlerp(oldCamUpRot, nextCamUpRot, t);
	XMStoreFloat3(&cam.up, XMVector3Rotate(YVec, animationRot));
	//処理終わり
	if (CameraAnimationEase.IsEnd())
	{
		isChangeBasePlanetAnimation = false;
	}
}

void GameCamera::LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	//たいしょうのざひょうをプレイヤーに
	nextTargetPos = playerPos;
	//少しずらす
	XMVECTOR cameraAngleRot = XMQuaternionRotationMatrix(cam.GetMatBillboard());
	Vector3 shift(XMVector3Rotate(LockOnShift, cameraAngleRot));
	nextTargetPos += shift;
	cam.SetTarget(nextTargetPos);
	Vector3 camPos = -playerZVec;
	float length = 3.0f;

	camPos.normalize();
	camPos *= length;
	camPos += nextTargetPos;
	nextEyePos = camPos;

	XMVECTOR frontV = XMLoadFloat3(&(nextTargetPos - nextEyePos));
	nextCamUpRot = XMQuaternionRotationMatrix(FukuMath::GetMatRot(frontV, XMLoadFloat3(&playerYVec)));
}

void GameCamera::GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec)
{
}

void GameCamera::TitleUpdate()
{
	XMVECTOR angle = XMVector3Rotate(ZVec, nextCamUpRot);
	//追加する回転
	XMVECTOR addRot = XMQuaternionRotationAxis(YVec, degree/4);
	angle = XMVector3Rotate(angle, addRot);
	//基準点から動かしてワールド座標を作成
	nextEyePos = (angle * -sTitleCameraDistance) + nextTargetPos;

	nextCamUpRot = XMQuaternionMultiply(nextCamUpRot, addRot);
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
		IsAnimationOn();
		StartCameraAnimation(true, 15);
	}

	if (GameInput::Instance()->GrabInput())
	{
		LockonUpdate(playerPos, playerZVec, playerYVec);
	}
	else if (GameInput::Instance()->LockOnInput())
	{
		LockonUpdate(playerPos, playerZVec, playerYVec);
	}
	else
	{
		NormalUpdate(playerPos);
	}



	if (GameInput::Instance()->LockOnRelease())
	{
		nextEyePos = (playerPos - playerZVec * 100);
	}
}

void GameCamera::IsAnimationOn()
{
	isChangeBasePlanet = true;
	isChangeBasePlanetAnimation = false;
}

void GameCamera::camRot(DirectX::XMFLOAT2 rot)
{
	XMVECTOR rotQ = XMQuaternionIdentity();

	rotQ = XMQuaternionRotationAxis(XMVector3Rotate(YVec, nextCamUpRot), rot.x * RotRate);
	//targetからnextPosの視点座標までのベクトルを計算
	XMVECTOR nextVec = XMLoadFloat3(&(nextEyePos - nextTargetPos));
	//回転させる
	nextVec = XMVector3Rotate(nextVec, rotQ);

	Vector3 camPos;
	XMStoreFloat3(&camPos, nextVec);
	//targetの座標＋生成したベクトルを合わせて回転完了
	camPos += nextTargetPos;
	nextEyePos = camPos;
}
