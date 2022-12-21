#pragma once
#include "particleManager.h"
#include "Vector3.h"
class GateParticle : public ParticleManager
{
public:
	GateParticle();
	void Update() override;
	void AddGateParticle(const Vector3 &gateAngle);
	void SetPlayerData(const Vector3& pos, const Vector3 &moveVec);

private:
	//移動のアップデート
	void MoveUpdate(Particle &e);

	//消滅のアップデート
	void DeadUpdate(Particle& e);
private:
	Vector3 playerPos_;
	Vector3 playerMoveVec_;
};

