#include "PlanetParticle.h"
#include "FukuMath.h"

using namespace DirectX;
PlanetParticle::PlanetParticle()
{
}

void PlanetParticle::Update()
{
	//�������s�����p�[�e�B�N����S�폜
	particles_.remove_if(
		[](Particle &x)
		{
			return x.frame >= x.dead_frame;
		}
	);

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

		XMStoreFloat4(&it->draw_color, ((it->s_color * f )+ (it->e_color * (1.0f - f))));
	}

	SendBuffers(particles_);
}

void PlanetParticle::AddPlanetParticle(const Vector3 &pos, float radius, const XMFLOAT4 &color)
{

	for (int i = 0; i < 100; i++)
	{

		//���X�g�ɗv�f��ǉ�
		particles_.emplace_front();
		//�ǉ������v�f�̎Q��
		Particle &p = particles_.front();

		//������������������_���Ŏw��
		Vector3 angle;
		angle.x = static_cast<float>(rand() % 201 - 100);
		angle.y = static_cast<float>(rand() % 201 - 100);
		angle.z = static_cast<float>(rand() % 201 - 100);
		angle = angle.normalize();

		p.position = angle * radius;
		p.position += pos;
		p.velocity = angle * 1.0f;
		p.accel = -angle * 0.01f;
		p.dead_frame = 120;
		p.s_scale = 3.0f;
		p.e_scale = 0.0f;
		p.s_color = XMLoadFloat4(&color);
		p.e_color = p.s_color;
		p.s_color *= 2.0f;
	}
}
