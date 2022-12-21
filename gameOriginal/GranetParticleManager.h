#pragma once
#include "FieldParticle.h"
#include "PlanetParticle.h"
#include "GateParticle.h"
#include "gameConstData.h"
class Camera;

/// <summary>
/// ゲーム内のパーティクルをまとめて管理するクラス
/// </summary>
class GranetParticleManager
{
public:
	/// <summary>
	/// パーティクルを生成するのに必要なデータ
	/// </summary>
	struct MakeParticleDatas
	{
		//カメラの座標
		Vector3 cameraPos;
		//カメラの姿勢
		DirectX::XMVECTOR cameraRot;
		//プレイヤーの持つ重力データ
		GameDatas::GravityData playerGravity;
		//プレイヤーの座標
		Vector3 playerPos;
		//プレイヤーの
		Vector3 playerMoveVec;
	};
public:
	//コンストラクタ
	GranetParticleManager();
	//デストラクタ
	~GranetParticleManager();
	//初期化
	void Init(Camera* camera);
	//更新処理
	void Update();
	//描画
	void Draw();

	//パーティクルの生成に必要な情報
	void SetMakeParticleDatas(const MakeParticleDatas &data);
	//PlanetParticleのオブジェクトのポインタ
	PlanetParticle* GetPlanetParticles();
	//GateParticleのオブジェクトのポインタ
	GateParticle* GetGateParticles();
private:
	//全体に常に出ているパーティクル
	FieldParticle fieldParticles_;
	//惑星の色が変わった際に出るパーティクル
	PlanetParticle planetParticles_;
	//リング通過時に生成されるパーティクル
	GateParticle gateParticles_;

	//生成に必要なデータ
	MakeParticleDatas makeData;
};

