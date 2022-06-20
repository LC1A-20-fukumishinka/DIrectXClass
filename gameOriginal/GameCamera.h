#pragma once
#include "../Camera.h"

class GameCamera
{
public:
	void Init();
	void Update(const Vector3 &playerPos, const Vector3 &playerZVec);
	void Draw();
	void Finalize();
public:
	Camera *GetCamera();
	const Vector3 &GetCameraPos();
private:
	void NormalUpdate(const Vector3 &playerPos);
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
private:
	void camRot();
private:
	Camera cam;
};

