#pragma once
#include "particleManager.h"
class PlanetParticle :public ParticleManager
{
public:
PlanetParticle();
void Update()override;

/// <summary>
/// �f���̃p�[�e�B�N���𐶐�
/// </summary>
/// <param name="pos">�f���̍��W</param>
/// <param name="radius">�f���̔��a</param>
/// <param name="color">�f���̐F</param>
void AddPlanetParticle(const Vector3 &pos, float radius,const XMFLOAT4 &color);

private:
};

