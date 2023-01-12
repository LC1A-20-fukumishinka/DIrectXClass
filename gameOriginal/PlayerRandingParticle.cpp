#include "PlayerRandingParticle.h"
#include "FukuMath.h"

using namespace DirectX;
PlayerRandingParticle::PlayerRandingParticle()
{
}

PlayerRandingParticle::~PlayerRandingParticle()
{
}

void PlayerRandingParticle::Update()
{
	//�������s�����p�[�e�B�N����S�폜
	particles_.remove_if(
		[](Particle& x)
		{
			return x.frame >= x.dead_frame;
		}
	);
	//�S�p�[�e�B�N���X�V

	for (std::forward_list<Particle>::iterator it = particles_.begin();
		it != particles_.end();
		it++)
	{
		//�o�߃t���[�������J�E���g
		it->frame++;
		//���x�ɉ����x�����Z
		it->velocity = it->velocity + it->accel;
		//���x�ɂ��ړ�
		it->position = it->position + it->velocity;

		//�i�s�x��0~1�͈̔͂Ɋ��Z
		float f = it->frame / (float)it->dead_frame;

		//�X�P�[���̐��`���
		float a = (it->e_scale - it->s_scale);
		it->scale = a * f;

		it->scale += it->s_scale;
		XMStoreFloat4(&it->draw_color, ((it->s_color * (1.0f - f)) + (it->e_color * f)));
	}
	SendBuffers();
}

void PlayerRandingParticle::AddRandingParticle(const Vector3& pos, const Vector3& up, const Vector3 & front, const Vector3& color)
{
	
	if(particles_.max_size())
	particles_.emplace_front();

	auto& p = particles_.front();

	p.s_scale = 3.0f;
	p.e_scale = 10.0f;

	p.position = pos;

	Vector3 angle;
	angle.x = static_cast<float>(rand() % 200 - 100);
	angle.y = 100.0f;
	angle.z = static_cast<float>(rand() % 200 - 100);
	angle = angle.normalize();
	angle = DirectX::XMVector3Rotate( F3toV(angle), DirectX::XMQuaternionRotationMatrix(FukuMath::GetMatRot(F3toV(up), F3toV(front))));
	p.velocity = angle * 0.3f;

	p.accel = -p.velocity *= 0.003f;

	p.dead_frame = 120;
	p.s_color = { color.x,color.y ,color.z ,1.0f };
	p.e_color = { color.x,color.y ,color.z ,0.0f };
}
