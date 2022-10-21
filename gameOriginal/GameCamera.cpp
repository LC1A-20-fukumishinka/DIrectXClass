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
		//���̑Ώۂ̏ꏊ�ɑ��ړ�����
		cam_.SetEye(nextEyePos_);
		//��{�̏�x�N�g������]�����Ċm�F
		//cam_.up = XMVector3Rotate(XMLoadFloat3(&playerYVec), nextCamUpRot_);
		cam_.up = XMVector3Rotate(YVec, nextCamUpRot_);

		//���̃^�[�Q�b�g���W�Ɉړ�
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

	//���������݂��Ȃ������ꍇ�̏C��
	if (eyeDir.length() == 0.0f)
	{
		eyeDir = Vector3{ 0, 0, 1 };
	}

	//�������Ƃ�
	eyeDir = eyeDir.normalize();
	eyeDir *= 300.0f;

	//��_���瓮�����ă��[���h���W���쐬
	nextEyePos_ = eyeDir + planet_.lock()->GetPos();

	//��x�N�g�������ス����
	nextCamUpRot_ = XMQuaternionIdentity();

	//��Ԃ̐ݒ�
	isClearMode_ = true;

	//�A�j���[�V�����J�n����
	StartCameraAnimation(true, 90);
}

void GameCamera::TitleAnimationStart()
{
	//�J�����̌�����������
	Vector3 eyeDir = Vector3{ 0, -0.4f, 1 };

	//���K��
	eyeDir = eyeDir.normalize();
	XMMATRIX rot = GetMatRot(YVec, XMLoadFloat3( &eyeDir));

	//�^�[�Q�b�g�̈ʒu���C��
	nextTargetPos_ = Vector3(0, 0, 0);

	//��_���瓮�����ă��[���h���W���쐬
	nextEyePos_ = (eyeDir * -sTitleCameraDistance) + nextTargetPos_;

	//��x�N�g�������ス����
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


	//�v���C���[�Ƃ̋������N�����v
	length = std::clamp(length, camMinRength, camMaxRength);
	//�v���C���[->�J����(����)
	Vector3 camAngle = camPos.normalize();
	//������␳
	camPos = camAngle * length;
	//�^�[�Q�b�g�̈ʒu�����Z���ăJ�����̍��W����
	camPos += nextTargetPos_;

	//float camUpLength = 5;
	//Vector3 basePlanetToCameraAngle = { 0, 1, 0 };

	//if (!planet_.expired())
	//{
	//	//�f�����J����
	//	basePlanetToCameraAngle = camPos - planet_.lock()->GetPos();
	//	basePlanetToCameraAngle.normalize();

	//	camUpLength += planet_.lock()->GetScale();

	//	basePlanetToCameraAngle *= camUpLength;

	//	camPos = planet_.lock()->GetPos();

	//	camPos += basePlanetToCameraAngle;
	//}

	////��L�̏����łł����x�N�g���ŉ�]�s����쐬
	//XMMATRIX camUpMat = FukuMath::GetMatRot(XMLoadFloat3(&camAngle), XMLoadFloat3(&basePlanetToCameraAngle.normalize()));
	//quaternion��
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
	//�ړ�����

	//�������Ƃ���
	Vector3 eyeDist = nextEyePos_ - oldEyePos_;
	//�������C�[�W���O�̒l�ŏ�Z
	eyeDist *= t;
	//���̎��_�ɂ������̋��������Z
	cam_.SetEye(oldEyePos_ + eyeDist);

	if (isTargetEase_)
	{
		//�������Ƃ���
		Vector3 targetDist = nextTargetPos_ - oldTargetPos_;
		//�������C�[�W���O�̒l�ŏ�Z
		targetDist *= t;
		//���̎��_�ɂ������̋��������Z
		cam_.SetTarget(oldTargetPos_ + targetDist);
	}
	else
	{
		cam_.SetTarget(nextTargetPos_);
	}

	//��]����
	XMVECTOR animationRot = XMQuaternionSlerp(oldCamUpRot_, nextCamUpRot_, t);
	XMStoreFloat3(&cam_.up, XMVector3Rotate(YVec, animationRot));
	//�����I���
	if (CameraAnimationEase_.IsEnd())
	{
		isChangeBasePlanetAnimation_ = false;
	}
}

void GameCamera::LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec)
{
	//�������傤�̂��Ђ傤���v���C���[��
	nextTargetPos_ = playerPos;
	//�������炷
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
	//�ǉ������]
	XMVECTOR addRot = XMQuaternionRotationAxis(YVec, degree / 4);
	angle = XMVector3Rotate(angle, addRot);
	//��_���瓮�����ă��[���h���W���쐬
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
	//target����nextPos�̎��_���W�܂ł̃x�N�g�����v�Z
	//XMVECTOR nextVec = XMLoadFloat3(&(nextEyePos_ - nextTargetPos_));

	float length = (nextEyePos_ - nextTargetPos_).length();
	XMVECTOR nextVec = -ZVec;

	//��]������
	nextVec = XMVector3Rotate(nextVec, rotQ);

	nextVec *= length;
	Vector3 camPos;
	XMStoreFloat3(&camPos, nextVec);
	//target�̍��W�{���������x�N�g�������킹�ĉ�]����
	camPos += nextTargetPos_;
	nextEyePos_ = camPos;
}
