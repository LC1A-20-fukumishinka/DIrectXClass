#include "GameCamera.h"
#include "Vector3.h"
#include "FukuMath.h"
#include "gameConstData.h"
#include "GameInput.h"
#include <algorithm>
#include "Planet.h"
using namespace FukuMath;
using namespace GameDatas;
using namespace GameInput;
using namespace DirectX;
void GameCamera::Init()
{
	cam.Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
}

void GameCamera::Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	if (GrabInput())
	{
		LockonUpdate(playerPos, playerZVec, playerYVec);
	}
	else if (LockOnInput())
	{
		LockonUpdate(playerPos, playerZVec, playerYVec);
	}
	else
	{
		NormalUpdate(playerPos);
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

void GameCamera::SetBasePlanet(const std::weak_ptr<Planet> planet)
{
	this->planet = planet;
}

void GameCamera::NormalUpdate(const Vector3 &playerPos)
{
	cam.target = playerPos;
	cam.target.y += 1.0f;
	Vector3 camPos = cam.GetEye() - cam.target;
	float length = camPos.length();


	//プレイヤーとの距離をクランプ
	length = std::clamp(length, camMinRength, camMaxRength);
	//プレイヤー->カメラ(向き)
	camPos.normalize();
	//距離を補正
	camPos *= length;
	//ターゲットの位置を加算してカメラの座標完成
	camPos += cam.target;

	float camUpLength = 3;
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

	//camPos.y = 3;
	cam.SetEye(camPos);
	cam.up = basePlanetToCameraAngle.normalize();
	//DirectX::XMStoreFloat3(&cam.up, YVec);
	camRot();
}

void GameCamera::LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	//たいしょうのざひょうをプレイヤーに
	cam.target = playerPos;
	//少しずらす
	XMVECTOR cameraAngleRot = XMQuaternionRotationMatrix(cam.GetMatBillboard());
	Vector3 shift(XMVector3Rotate(LockOnShift, cameraAngleRot));
	cam.target += shift;
	Vector3 camPos = -playerZVec;
	float length = 3.0f;

	camPos.normalize();
	camPos *= length;
	camPos += cam.target;
	cam.SetEye(camPos);
	cam.up = playerYVec;
}

void GameCamera::GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec)
{
}

void GameCamera::camRot()
{
	XMVECTOR rotQ = XMQuaternionIdentity();
	//cam.UpVecReset();
	rotQ = XMQuaternionRotationAxis(XMLoadFloat3(&cam.up), RStick().x * RotRate);
	cam.CameraRot(rotQ);
}
