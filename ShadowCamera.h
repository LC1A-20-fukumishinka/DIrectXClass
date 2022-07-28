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
	void SetAngle(const Vector3 &angle);
private:
	std::unique_ptr<Camera> cam_;
	std::weak_ptr<GravityPlayer> player_;

	Vector3 camAngle_;

	float dist_;
};

