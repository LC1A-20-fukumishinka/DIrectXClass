#pragma once
#include "particleManager.h"
#include "gameConstData.h"
#include <random>
class FieldParticle : public ParticleManager
{
public:
	FieldParticle();
	void Update() override;
	void AddFieldParticle();

	void SetPlayerDatas(const Vector3 &playerPos,DirectX::XMVECTOR playerPosture, const GameDatas::GravityData &gravityDatas);
private:
	Vector3 playerPos_;
	Vector3 SpawnRange_;
	DirectX::XMVECTOR playerPosture_ = {};
	GameDatas::GravityData playerGravityData_;
};

