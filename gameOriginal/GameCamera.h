#pragma once
#include "../Camera.h"
#include <memory>
#include "../EaseClass.h"
#include "gameConstData.h"
class Planet;
class GameCamera
{
public:
	void Init();
	void Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec, GameDatas::PlayerStatus playerStatus);
	void Draw();
	void Finalize();

public:

	void SetGravityData(GameDatas::GravityData isOneWayGravity);
	Camera *GetCamera();
	Vector3 GetCameraPos();
	bool GetIsCameraStop();

	void CameraStop();
	void SetBasePlanet(const std::weak_ptr<Planet> planet);

	/// <summary>
	/// ���݂̃J�����̏�Ԃ�ۑ����A�j���[�V�������X�^�[�g
	/// </summary>
	/// <param name="isTargetEase">�ڕW���W��J�ڂ����邩</param>
	/// <param name="EaseTimer">�A�j���[�V��������</param>
	void StartCameraAnimation(bool isTargetEase, int EaseTimer);

	void ClearAnimationStart(const Vector3 &playerPos);


	void TitleAnimationStart();
	/// <summary>
	/// �N���A����Q�[���ɑJ��
	/// </summary>
	void ClearToIngme();

	/// <summary>
	/// �^�C�g������Q�[���ɑJ��
	/// </summary>
	void TitleToIngame(const Vector3 &playerPos, const Vector3 &playerZVec);

	/// <summary>
	/// �N���A���ɃJ�������������W
	/// </summary>
	void SetNextPlantPos(const Vector3 &pos);

	/// <summary>
	/// �A�j���[�V�������I���������ǂ���
	/// </summary>
	/// <returns></returns>
	bool GetIsAnimationEnd();

	void CameraReflesh(Vector3 UpVec, int timer);

	void LandingCameraReflesh(Vector3 UpVec);
private://�󋵕ʍX�V����

	/// <summary>
	/// �J�������J�ڌn�A�j���[�V�������̍X�V����
	/// </summary>
	void CameraAnimationUpdate();

	/// <summary>
	/// �Q�[���v���C���̏���
	/// </summary>
	void IngameCameraUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	//�N���A���J������������
	void ClearCameraUpdate();
	//�ʏ펞�̃J��������
	void NormalUpdate(const Vector3 &playerPos, const Vector3 &playerYVec);
	//���ڏ�Ԃ̃J��������
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	//�͂ݏ�Ԃ̃J��������
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
	//�^�C�g�����̃J��������
	void TitleUpdate();

	//�f���ƐڐG���Ă����ۂ̍X�V����
	void PlanetCollisionUpdate();
private:
	void camRot(const DirectX::XMFLOAT2 &rot, Vector3 playerUp);
private://�����o�ϐ�
	Camera cam_;

	//�v�Z���ďo�����_���W
	Vector3 nextEyePos_;
	//�Ώۍ��W
	Vector3 nextTargetPos_;
	//�J�����̎p��
	DirectX::XMVECTOR nextCamUpRot_;
	//�A�j���[�V�����p�ɐݒ肷��ߋ��̎��_
	Vector3 oldEyePos_;
	//�A�j���[�V�����p�ɐݒ肷��ߋ��̑Ώ�
	Vector3 oldTargetPos_;
	//�A�j���[�V�����p�ɐݒ肷��ߋ��̎p��
	DirectX::XMVECTOR oldCamUpRot_;
	std::weak_ptr<Planet> planet_;
	bool isCameraStop_ = false;
	//���̘f���̍��W
	Vector3 nextPlanetPos_;

	//Vector3 playerPos_;
	//XMVECTOR playerPosture_;
private://�����̃C�[�W���O
	bool isChangeBasePlanetAnimation_;
	Easing CameraAnimationEase_;
	bool isTargetEase_ = false;

	bool isClearMode_ = false;
	bool isTitleMode_ = true;

	GameDatas::GravityData gravity_;
	bool oldIsOneWayGravity_ = false;

	float nowDistance = GameDatas::camMaxLength;
	float rateDistance = GameDatas::camMaxLength;
	float CToPCollAnimationRate = 1.0f;
	GameDatas::PlayerStatus playerStatus_ = GameDatas::PlayerStatus::STAND;

private://Imgui
	bool imCamBackFlag_ = true;
	bool imCamCollision_ = true;
	bool imCamLanding_ = true;
	void ImguiUpdate();

};

