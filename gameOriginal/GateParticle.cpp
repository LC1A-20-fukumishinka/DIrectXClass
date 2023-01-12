#include "GateParticle.h"
#include "FukuMath.h"
#include "../EaseClass.h"
using namespace DirectX;
GateParticle::GateParticle()
{
}

void GateParticle::Update()
{
	//�������s�����p�[�e�B�N����S�폜
	particles_.remove_if(
		[](Particle& x)
		{
			return x.frame >= x.dead_frame;
		}
	);
	for (auto& e : particles_)
	{
		//�����Ă�����
		bool isAlive = !e.isDead;
		if (isAlive)
		{//�ڋ߂���
			MoveUpdate(e);
		}
		else
		{//�F�������Ȃ���ł���
			DeadUpdate(e);
		}
	}

	SendBuffers();
}

void GateParticle::AddGateParticle(const Vector3& gateAngle, const Vector3 color)
{


	
	Vector3 cross = gateAngle.cross(FukuMath::YVec);

	cross = cross.normalize();
	for (int i = 0; i < 30; i++)
	{
		particles_.emplace_front();

		auto& p = particles_.front();

		p.dead_frame = 1;

		DirectX::XMVECTOR rotQ = DirectX::XMQuaternionRotationAxis(F3toV(gateAngle), (FukuMath::PI / 15.0f) * i);
		Vector3 particleAngle = DirectX::XMVector3Rotate(F3toV(cross), rotQ);
		//particleAngle *= 15.0f;
		p.position = playerPos_;
		p.position += (particleAngle * 2.0f);
		p.scale = 3.0f;
		p.velocity = (particleAngle * 20.0f);

		if (playerMoveVec_.length() <= 2.0f)
		{
			playerMoveVec_ = (playerMoveVec_.normalize() * 2.0f);
		}

		p.velocity += playerMoveVec_ * 3.0f;
		p.dead_frame = 20;
		p.s_color = { color.x,color.y ,color.z , 1.0f };
		p.e_color = { color.x,color.y ,color.z , 0.0f };
		XMStoreFloat4(&p.draw_color, p.s_color);

		p.s_scale = 1.0f;
		p.e_scale = 10.0f;

	}

}

void GateParticle::SetPlayerData(const Vector3& pos, const Vector3& moveVec)
{
	playerPos_ = pos;
	playerMoveVec_ = moveVec;
}

void GateParticle::MoveUpdate(Particle& e)
{
	//���x�ɉ����x�����Z
	float power = e.velocity.length();

	e.velocity = (e.velocity + playerMoveVec_).normalize() * power;


	e.velocity *= 0.95f;


	Vector3 dist = playerPos_ - e.position;

	//���@�ɋ߂Â����瑬�x�����ɂ���
	if (dist.length() <= 40.0f)
	{
		e.position += dist.normalize() * 4.0f;
	}
	else
	{
		e.position += dist / 7.0f;
	}
	e.position += e.velocity;
	if (dist.length() <= 2.0f)
	{
		e.position = playerPos_;
		e.isDead = true;
	}
}

void GateParticle::DeadUpdate(Particle& e)
{
	e.frame++;
	e.position = playerPos_;

	//�i�s�x��0~1�͈̔͂Ɋ��Z
	float f = e.frame / (float)e.dead_frame;

	//�ω��������H
	f = Easing::easeOutCubic(f);
	DirectX::XMVECTOR color = e.e_color - e.s_color;

	//�F��ω�
	color *= f;
	color += e.s_color;
	XMStoreFloat4(&e.draw_color, color);

	//�X�P�[���̐��`���
	float dist = (e.e_scale - e.s_scale);
	e.scale = dist * f;

	//�ω��ʂɏ����l�����Z���ăX�P�[�����v�Z
	e.scale += e.s_scale;
}
