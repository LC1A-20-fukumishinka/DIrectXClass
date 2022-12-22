#include "PlayerRandingParticle.h"
#include "FukuMath.h"
PlayerRandingParticle::PlayerRandingParticle()
{
}

PlayerRandingParticle::~PlayerRandingParticle()
{
}

//void PlayerRandingParticle::Update()
//{
//}

void PlayerRandingParticle::AddRandingParticle(const Vector3& pos, const Vector3& up)
{
	particles_.emplace_front();

	auto& p = particles_.front();

	p.s_scale = 3.0f;
	p.e_scale = 10.0f;

	p.position = pos;

	Vector3 angle;
	angle.x = static_cast<float>(rand() % 201 - 100);
	angle.y = 100.0f;
	angle.z = static_cast<float>(rand() % 201 - 100);
	angle = angle.normalize();
	angle = DirectX::XMVector3Rotate( F3toV(angle), DirectX::XMQuaternionRotationMatrix(FukuMath::GetMatRot(F3toV(up), FukuMath::ZVec)));
	p.velocity = angle * 0.3f;

	p.accel = -p.velocity *= 0.003f;

	p.dead_frame = 120;
	p.s_color = { 1.0f,1.0f ,1.0f ,1.0f };
	p.e_color = { 1.0f,1.0f ,1.0f ,0.0f };
}
