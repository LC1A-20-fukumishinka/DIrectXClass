#pragma once
#include "FukuMath.h"
#include "Vector3.h"
namespace GameDatas
{

	//�d�͂̃f�[�^
	struct GravityData
	{
		Vector3 angle;
		bool isOneWayGravity = false;
	};

	//�Q�[�g��ʉ߂������ǂ�����
	struct CollisionGateData
	{
		//�ڐG������
		bool isCollision;
		//�ڐG�����Q�[�g�̐F
		Vector3 color;
	};
	//�v���C���[����肤����
	enum class PlayerStatus
	{
		STAND,
		JUMP,
	};

	//�L�����N�^�[�̊�̕\���
	enum class PlayerFaceTexture
	{
		NORMAL,
		BLINK,
		SMILE,
		STRIVE,
	};
	//�ϐ�
		//�X�e�B�b�N���͂ɂ��J�����̉�]���x
	const float RotRate = 2.0f * FukuMath::degree;
	//�J�������L�����N�^�[��肤��ő勗��
	const float camMaxLength = 30.0f;
	//�L�����N�^�[�̃X�e�B�b�N�ɂ��ړ��̍ō����x
	const float maxMoveSpeed = 0.5f;
	//��ώ��_�ɂȂ������ɃL�����N�^���炸�炷�ʒu
	const DirectX::XMVECTOR LockOnShift = {2.0f, 2.0f, 0.0f};
	//�d�͂̊��
	const float baseGravityPower = 0.025f;
	//�^�C�g�����Ɏ��J�����̋���
	const float sTitleCameraDistance = 200.0f;
}