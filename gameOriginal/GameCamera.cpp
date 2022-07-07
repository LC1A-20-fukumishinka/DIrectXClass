#include "GameCamera.h"
#include "Vector3.h"
#include "FukuMath.h"
#include "gameConstData.h"
#include "GameInput.h"
#include <algorithm>
#include "Planet.h"
using namespace FukuMath;
using namespace GameDatas;
using namespace DirectX;
void GameCamera::Init()
{
	cam.Init(Vector3(0, 3, -15), Vector3(0, 3, 0));
	CameraAnimationEase.Init(60);

	nextEyePos = cam.GetEye();
}

void GameCamera::Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
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

	if (isChangeBasePlanetAnimation)
	{
		CameraAnimationUpdate();
	}
	else if(!isChangeBasePlanet)
	{
		//���̑Ώۂ̏ꏊ�ɑ��ړ�����
		cam.SetEye(nextEyePos);
		//��{�̏�x�N�g������]�����Ċm�F
		XMStoreFloat3(&cam.up, XMVector3Rotate(YVec, nextCamUpRot));
		//���̃^�[�Q�b�g���W�Ɉړ�
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

void GameCamera::StartCameraAnimation(bool isTargetEase , int EaseTimer)
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

void GameCamera::NormalUpdate(const Vector3 &playerPos)
{
	nextTargetPos = playerPos;
	nextTargetPos.y += 1.0f;
	cam.SetTarget(nextTargetPos);
	Vector3 camPos = nextEyePos - nextTargetPos;
	float length = camPos.length();


	//�v���C���[�Ƃ̋������N�����v
	length = std::clamp(length, camMinRength, camMaxRength);
	//�v���C���[->�J����(����)
	Vector3 camAngle = camPos.normalize();
	//������␳
	camPos = camAngle * length;
	//�^�[�Q�b�g�̈ʒu�����Z���ăJ�����̍��W����
	camPos += nextTargetPos;

	float camUpLength = 5;
	Vector3 basePlanetToCameraAngle = { 0, 1, 0 };

	if (!planet.expired())
	{
		//�f�����J����
		basePlanetToCameraAngle = camPos - planet.lock()->GetPos();
		basePlanetToCameraAngle.normalize();

		camUpLength += planet.lock()->GetScale();

		basePlanetToCameraAngle *= camUpLength;

		camPos = planet.lock()->GetPos();

		camPos += basePlanetToCameraAngle;
	}

	//��L�̏����łł����x�N�g���ŉ�]�s����쐬
	XMMATRIX camUpMat = FukuMath::GetMatRot(XMLoadFloat3(&camAngle), XMLoadFloat3(&basePlanetToCameraAngle.normalize()));
	//quaternion��
	nextCamUpRot = XMQuaternionRotationMatrix(camUpMat);
	nextEyePos = camPos;

	camRot();
}

void GameCamera::CameraAnimationUpdate()
{
	//easing
	float t = CameraAnimationEase.Do(Easing::Out, Easing::Cubic);
	//�ړ�����

	//�������Ƃ���
		Vector3 eyeDist = nextEyePos - oldEyePos;
	//�������C�[�W���O�̒l�ŏ�Z
		eyeDist *= t;
	//���̎��_�ɂ������̋��������Z
	cam.SetEye(oldEyePos + eyeDist);

	if (isTargetEase)
	{
		//�������Ƃ���
		Vector3 targetDist = nextTargetPos - oldTargetPos;
		//�������C�[�W���O�̒l�ŏ�Z
		targetDist *= t;
		//���̎��_�ɂ������̋��������Z
		cam.SetTarget(oldTargetPos + targetDist);
	}
	else
	{
		cam.SetTarget(nextTargetPos);
	}

	//��]����
	XMVECTOR animationRot = XMQuaternionSlerp(oldCamUpRot, nextCamUpRot, t);
	XMStoreFloat3(&cam.up, XMVector3Rotate(YVec, animationRot));
	//�����I���
	if (CameraAnimationEase.IsEnd())
	{
		isChangeBasePlanetAnimation = false;
	}
}

void GameCamera::LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	//�������傤�̂��Ђ傤���v���C���[��
	nextTargetPos = playerPos;
	//�������炷
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

void GameCamera::IsAnimationOn()
{
	isChangeBasePlanet = true;
	isChangeBasePlanetAnimation = false;
}

void GameCamera::camRot()
{
	XMVECTOR rotQ = XMQuaternionIdentity();

	rotQ = XMQuaternionRotationAxis(XMLoadFloat3(&cam.up), GameInput::Instance()->RStick().x * RotRate);
	//target����nextPos�̎��_���W�܂ł̃x�N�g�����v�Z
	XMVECTOR nextVec = XMLoadFloat3(&(nextEyePos - nextTargetPos));
	//��]������
	nextVec = XMVector3Rotate(nextVec, rotQ);

	Vector3 camPos;
	XMStoreFloat3(&camPos, nextVec);
	//target�̍��W�{���������x�N�g�������킹�ĉ�]����
	camPos += nextTargetPos;
	nextEyePos = camPos;
}
