#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "ModelManager.h"
#include <memory>
#include "gameConstData.h"
#include "../Collision/CollisionPrimitive.h"
class Gate
{
public:
	//コンストラクタ
	Gate();
	//デストラクタ
	~Gate();

	//初期化生成処理
	void Init(const Vector3& pos, const Vector3& angle, int index, bool isDraw);
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//終了処理
	void Finalize();
	//リセット処理(ゲームリセット時に使用する)
	void Reset();

	void Spawn();
	//復活処理(リングを復活させたいときに使う)
	void Reborn();
	//衝突処理
	bool Collision(const Sphere& player);
	//場所のゲッタ
	const DirectX::XMFLOAT3& GetPos();
	//向いてる方向のゲッタ
	Vector3 GetAngle();

	void SetAngle(const Vector3& angle);
	/// <summary>
	/// 座標のセッタ
	/// </summary>
	/// <param name="pos">入れる座標</param>
	void SetPos(const Vector3& pos);
	/// <summary>
	/// 色をセットする
	/// </summary>
	/// <param name="color">セットする色</param>
	void SetColor(const DirectX::XMFLOAT4& color);
	/// <summary>
	/// 自分の色を返す
	/// </summary>
	/// <returns>自分の色</returns>
	const DirectX::XMFLOAT4& GetColor();

	int GetID();
public:
	static void SetModel(Model* model);
	static void SetCamera(Camera* camera);
	static void SetLightGroup(LightGroup* lights);
private:

	// 機能している状態か
	bool isAlive_ = false;
	//描画しているか
	bool isDraw_ = true;
	//最初から描画しているか
	bool isStartDraw_ = false;
	//現在の座標
	Vector3 pos_;

	//リングの向き
	Vector3 angle_;

	//半径
	float scale_ = 30.0f;


	//自分のステージ番号
	int index_ = -1;

	//描画用オブジェクト
	Object3D object_;

	//モデル
	static Model* sModel;
	//カメラ
	static Camera* sCamera;
	//ライト
	static LightGroup* sLights;

};

