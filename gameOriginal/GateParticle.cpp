#include "GateParticle.h"
#include "FukuMath.h"
#include "../EaseClass.h"
using namespace DirectX;
GateParticle::GateParticle()
{
}

void GateParticle::Update()
{
	//寿命が尽きたパーティクルを全削除
	particles_.remove_if(
		[](Particle& x)
		{
			return x.frame >= x.dead_frame;
		}
	);
	for (auto& e : particles_)
	{
		//生きていたら
		bool isAlive = !e.isDead;
		if (isAlive)
		{//接近する
			MoveUpdate(e);
		}
		else
		{//色が薄くなり消滅する
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
	//速度に加速度を加算
	float power = e.velocity.length();

	e.velocity = (e.velocity + playerMoveVec_).normalize() * power;


	e.velocity *= 0.95f;


	Vector3 dist = playerPos_ - e.position;

	//自機に近づいたら速度を一定にする
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

	//進行度を0~1の範囲に換算
	float f = e.frame / (float)e.dead_frame;

	//変化率を加工
	f = Easing::easeOutCubic(f);
	DirectX::XMVECTOR color = e.e_color - e.s_color;

	//色を変化
	color *= f;
	color += e.s_color;
	XMStoreFloat4(&e.draw_color, color);

	//スケールの線形補間
	float dist = (e.e_scale - e.s_scale);
	e.scale = dist * f;

	//変化量に初期値を加算してスケールを計算
	e.scale += e.s_scale;
}
