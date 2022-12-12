#include "FieldParticle.h"
#include <random>
#include <DirectXMath.h>
using namespace DirectX;
FieldParticle::FieldParticle()
{
	SpawnRange_ = Vector3(100, 100, 200);
	playerPosture_ = XMQuaternionIdentity();
}

void FieldParticle::Update()
{
	//寿命が尽きたパーティクルを全削除
	particles_.remove_if(
		[](Particle &x)
		{
			return x.frame >= x.dead_frame;
		}
	);
	AddFieldParticle();

	Vector3 gravityPower;

	if (playerGravityData_.isOneWayGravity)
	{
		gravityPower = playerGravityData_.angle * 0.01f;
	}
	for (std::forward_list<Particle>::iterator it = particles_.begin();
		it != particles_.end();
		it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		it->accel = gravityPower;
		
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;

		//進行度を0~1の範囲に換算
		float f = it->frame / (float)it->dead_frame;

		//スケールの線形補間
		float dist = (it->e_scale - it->s_scale);
		it->scale = dist * f;

		//変化量に初期値を加算してスケールを計算
		it->scale += it->s_scale;

		//スケーリングの半分を超えたら縮小していく
		if (f >= 0.5f)
		{
			float harf = it->e_scale / 2.0f;
			harf += it->s_scale;
			it->scale = harf - (it->scale - harf);
		}

		DirectX::XMVECTOR color = it->e_color - it->s_color;
		color *= f;
		color += it->s_color;

		XMStoreFloat4(&it->draw_color, color);
		//if (f >= 0.5f)
		//{
		//	DirectX::XMVECTOR harf = it->e_color / 2.0f;

		//	it->draw_color = harf - (it->draw_color - harf);
		//}
	}
	SendBuffers(particles_);
}

void FieldParticle::AddFieldParticle()
{
	Vector3 spawnPos;
	spawnPos.x = (static_cast<float>(rand() % 201 - 100) / 100.0f) * SpawnRange_.x;
	spawnPos.y = (static_cast<float>(rand() % 201 - 100) / 100.0f) * SpawnRange_.y;
	spawnPos.z = (static_cast<float>(rand() % 111 - 10) / 100.0f) * SpawnRange_.z;

	spawnPos = XMVector3Rotate(XMLoadFloat3(&spawnPos), playerPosture_);
	spawnPos += playerPos_;
	//リストに要素を追加
	particles_.emplace_front();
	//追加した要素の参照
	Particle &p = particles_.front();

	//値のセット
	p.position = spawnPos;
	p.dead_frame = 240;
	p.scale = 0.0f;
	p.s_scale = 0.0f;
	p.e_scale = 1.0f;
	p.s_color = { 1, 1, 1, 1 };
	p.e_color = { 1, 1, 1, 1 };

}

void FieldParticle::SetPlayerDatas(const Vector3 &playerPos, DirectX::XMVECTOR playerPosture, const GameDatas::GravityData &gravityDatas)
{
	playerPos_ = playerPos;
	playerPosture_ = playerPosture;
	playerGravityData_ = gravityDatas;
}
