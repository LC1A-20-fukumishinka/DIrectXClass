#pragma once
#include "../Camera.h"
#include <memory>
#include "../EaseClass.h"
class Planet;
class GameCamera
{
public:
	void Init();
	void Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	void Draw();
	void Finalize();

public:
	Camera *GetCamera();
	Vector3 GetCameraPos();
	bool GetIsChangeBasePlanet();

	void SetBasePlanet(const std::weak_ptr<Planet> planet);
	/// <summary>
	/// �f����ύX����
	/// </summary>
	void IsAnimationOn();

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
	void SetNextPlantPos(const Vector3 pos);

	/// <summary>
	/// �A�j���[�V�������I���������ǂ���
	/// </summary>
	/// <returns></returns>
	bool GetIsAnimationEnd();
private:
	static const float sTitleCameraDistance;
private:
	void CameraAnimationUpdate();
	void IngameCameraUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
private://�󋵕ʍX�V����
	//�N���A���J������������
	void ClearCameraUpdate();
	//�ʏ펞�̃J��������
	void NormalUpdate(const Vector3 &playerPos);
	//���ڏ�Ԃ̃J��������
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	//�͂ݏ�Ԃ̃J��������
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
	//�^�C�g�����̃J��������
	void TitleUpdate();

private:
	void camRot(DirectX::XMFLOAT2 rot);
private:
	Camera cam;

	Vector3 nextEyePos = {};
	Vector3 nextTargetPos = {};
	DirectX::XMVECTOR nextCamUpRot = {};
	Vector3 oldEyePos = {};
	Vector3 oldTargetPos = {};
	DirectX::XMVECTOR oldCamUpRot = {};
	std::weak_ptr<Planet> planet;
	bool isChangeBasePlanet = false;
	//���̘f���̍��W
	Vector3 nextPlanetPos_ = {};
private://�����̃C�[�W���O
	bool isChangeBasePlanetAnimation;
	Easing CameraAnimationEase;
	bool isTargetEase = false;

	bool isClearMode_ = false;
	bool isTitleMode_ = true;
};

