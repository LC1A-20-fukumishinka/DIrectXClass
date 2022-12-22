#include "PlanetParticle.h"
#include "FukuMath.h"
#include "../EaseClass.h"
using namespace DirectX;
PlanetParticle::PlanetParticle()
{
}

void PlanetParticle::Update()
{
	//寿命が尽きたパーティクルを全削除
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
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;

		//進行度を0~1の範囲に換算
		float f = it->frame / (float)it->dead_frame;

		//スケールの線形補間
		float a = (it->e_scale - it->s_scale);
		it->scale = a * f;

		it->scale += it->s_scale;

		float easeF = Easing::easeOutCubic(f);
		XMStoreFloat4(&it->draw_color, ((it->s_color * (1.0f - easeF))+ (it->e_color * easeF)));
	}

	SendBuffers();
}

void PlanetParticle::AddPlanetParticle(const Vector3 &pos, float radius, const XMFLOAT4 &color)
{

	for (int i = 0; i < 100; i++)
	{

		//リストに要素を追加
		particles_.emplace_front();
		//追加した要素の参照
		Particle &p = particles_.front();

		//発生する方向をランダムで指定
		Vector3 angle;
		angle.x = static_cast<float>(rand() % 201 - 100);
		angle.y = static_cast<float>(rand() % 201 - 100);
		angle.z = static_cast<float>(rand() % 201 - 100);
		angle = angle.normalize();

		//惑星の半径から表面から出るように座標調整
		p.position = angle * radius;
		p.position += pos;

		//中心から法線方向へ移動する
		p.velocity = angle * 1.0f;

		//徐々に減速するように
		p.accel = -angle * 0.01f;
		p.dead_frame = 120;
		p.s_scale = 3.0f;
		p.e_scale = 0.0f;
		p.s_color = XMLoadFloat4(&color);
		p.e_color = p.s_color;
		p.s_color *= 2.0f;
	}
}
