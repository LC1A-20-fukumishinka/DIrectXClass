#pragma once
#include "Camera.h"
#include <memory>
class GravityPlayer;
class ShadowCamera
{
public:
	ShadowCamera();
	~ShadowCamera();

	void Init(const DirectX::XMFLOAT3 &playerPos);
	void Update(const DirectX::XMFLOAT3 &playerPos);
	void Draw();
	void Finalize();

	Camera *GetCamera();
private:
	std::unique_ptr<Camera> cam;
	std::weak_ptr<GravityPlayer> player;

	Vector3 camAngle;

	float dist;
};

