#include "ShadowCamera.h"
#include "Camera.h"
#include "gameOriginal/Player.h"
using namespace std;
using namespace DirectX;
ShadowCamera::ShadowCamera()
{
	cam_ = make_unique<Camera>();
	camAngle_ = Vector3{ 1, -1, 1 };
	dist_ = 30.0f;
	cam_->Init((Vector3() - (camAngle_ * dist_)), Vector3(),Vector3(0, 1, 0), false);
}

ShadowCamera::~ShadowCamera()
{
}

void ShadowCamera::Init(const XMFLOAT3 &playerPos)
{
	cam_->Init((playerPos - (camAngle_ * dist_)), playerPos);
}

void ShadowCamera::Update(const XMFLOAT3 &playerPos)
{
	cam_->SetEye((playerPos - (camAngle_ * dist_)));
	cam_->SetTarget(playerPos);
	cam_->Update();
}

void ShadowCamera::Draw()
{
}

void ShadowCamera::Finalize()
{
}



Camera *ShadowCamera::GetCamera()
{
	return cam_.get();
}

void ShadowCamera::SetAngle(const Vector3 &angle)
{
	camAngle_ = angle;
}
