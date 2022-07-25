#include "ShadowCamera.h"
#include "Camera.h"
#include "gameOriginal/GravityPlayer.h"
using namespace std;
using namespace DirectX;
ShadowCamera::ShadowCamera()
{
	cam = make_unique<Camera>();
	camAngle = Vector3{ 1, -1, 1 };
	dist = 5.0f;
	cam->Init((Vector3() - (camAngle * dist)), Vector3());
}

ShadowCamera::~ShadowCamera()
{
}

void ShadowCamera::Init(const XMFLOAT3 &playerPos)
{


	cam->Init((playerPos - (camAngle * dist)), playerPos);
}

void ShadowCamera::Update(const XMFLOAT3 &playerPos)
{
	cam->SetEye((playerPos - (camAngle * dist)));
	cam->SetTarget(playerPos);
	cam->Update();
}

void ShadowCamera::Draw()
{
}

void ShadowCamera::Finalize()
{
}



Camera *ShadowCamera::GetCamera()
{
	return cam.get();
}
