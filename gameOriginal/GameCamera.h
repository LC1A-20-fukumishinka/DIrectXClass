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
private:
	void NormalUpdate(const Vector3 &playerPos);
	void CameraAnimationUpdate();
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
	void ClearCameraUpdate();
	void IngameCameraUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
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
private://�����̃C�[�W���O
	bool isChangeBasePlanetAnimation;
	Easing CameraAnimationEase;
	bool isTargetEase = false;

	bool isClearMode_ = false;
};

