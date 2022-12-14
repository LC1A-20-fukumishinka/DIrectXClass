#pragma once
#include "particleManager.h"
class PlanetParticle :public ParticleManager
{
public:
PlanetParticle();
void Update()override;

/// <summary>
/// 惑星のパーティクルを生成
/// </summary>
/// <param name="pos">惑星の座標</param>
/// <param name="radius">惑星の半径</param>
/// <param name="color">惑星の色</param>
void AddPlanetParticle(const Vector3 &pos, float radius,const XMFLOAT4 &color);

private:
};

