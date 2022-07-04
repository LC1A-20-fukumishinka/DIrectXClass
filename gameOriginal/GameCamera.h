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
	/// 惑星を変更した
	/// </summary>
	void IsAnimationOn();

	/// <summary>
	/// 惑星変更アニメーション
	/// </summary>
	void StartCameraAnimation(bool isTargetEase, int EaseTimer);

private:
	void NormalUpdate(const Vector3 &playerPos);
	void CameraAnimationUpdate();
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
private:
	void camRot();
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
private://挙動のイージング
	bool isChangeBasePlanetAnimation;
	Easing CameraAnimationEase;
	bool isTargetEase = false;
};

