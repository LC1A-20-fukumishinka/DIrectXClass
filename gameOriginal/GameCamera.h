#pragma once
#include "../Camera.h"
#include <memory>
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

	void SetBasePlanet(const std::weak_ptr<Planet> planet);
private:
	void NormalUpdate(const Vector3 &playerPos);
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
private:
	void camRot();
private:
	Camera cam;

	Vector3 nextPos = {};
	DirectX::XMVECTOR nextRot = {};
	std::weak_ptr<Planet> planet;
};

