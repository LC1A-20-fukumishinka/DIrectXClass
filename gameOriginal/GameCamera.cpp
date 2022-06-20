#include "GameCamera.h"
#include "Vector3.h"
#include "FukuMath.h"
#include "gameConstData.h"
#include "GameInput.h"
#include <algorithm>
using namespace FukuMath;
using namespace GameDatas;
using namespace GameInput;
using namespace DirectX;
void GameCamera::Init()
{
	cam.Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
}

void GameCamera::Update(const Vector3 &playerPos, const Vector3 &playerZVec)
{
	if (GrabInput())
	{
		LockonUpdate(playerPos, playerZVec);
	}
	else if (LockOnInput())
	{
		LockonUpdate(playerPos, playerZVec);
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

const Vector3 &GameCamera::GetCameraPos()
{
XMFLOAT3 a = cam.GetEye();
	return cam.GetEye();
}

void GameCamera::NormalUpdate(const Vector3 &playerPos)
{
	cam.target = playerPos;
	cam.target.y += 1.0f;
	Vector3 camPos = cam.GetEye() - cam.target;
	float length = camPos.length();
	//if (length >= camMaxRength)
	//{
	//	length = camMaxRength;
	//}
	//else if ()
	//{

	//}

	//クランプの代用
	//何故か使えない
	length = std::clamp(length, camMinRength, camMaxRength);
	camPos.normalaize();
	camPos *= length;
	camPos += cam.target;
	camPos.y = 3;
	cam.SetEye(camPos);
	DirectX::XMStoreFloat3(&cam.up, YVec);
	camRot();
}

void GameCamera::LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec)
{
	//たいしょうのざひょうをプレイヤーに
	cam.target = playerPos;
	//少しずらす
	XMVECTOR cameraAngleRot = XMQuaternionRotationMatrix(cam.GetMatBillboard());
	Vector3 shift(XMVector3Rotate(LockOnShift, cameraAngleRot));
	cam.target += shift;
	Vector3 camPos = -playerZVec;
	float length = 3.0f;

	camPos.normalaize();
	camPos *= length;
	camPos += cam.target;
	cam.SetEye(camPos);
	DirectX::XMStoreFloat3(&cam.up, YVec);
}

void GameCamera::GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec)
{
}

void GameCamera::camRot()
{
	XMVECTOR rotQ = XMQuaternionIdentity();
	cam.UpVecReset();
	rotQ = XMQuaternionRotationAxis(YVec, RStick().x * RotRate);
	cam.CameraRot(rotQ);
}
