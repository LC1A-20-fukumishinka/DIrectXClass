#pragma once
#include "particleManager.h"
class PlayerRandingParticle : public ParticleManager
{
public:
	//コンストラクタ
	PlayerRandingParticle();
	//デストラクタ
	~PlayerRandingParticle();
	//更新処理
	void Update() override;

	/// <summary>
	/// 着地時のパーティクル生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="up">キャラクターの上方向</param>
	void AddRandingParticle(const Vector3& pos, const Vector3& up, const Vector3 &color);
};

