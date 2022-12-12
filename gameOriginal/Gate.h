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
	Gate();
	~Gate();


	void Init(const Vector3 &pos, const Vector3 &angle, int index, bool isDraw);
	void Update();
	void Draw();
	void Finalize();

	void Reset();

	bool Collision(const Sphere &player);

public:
	static void SetModel(Model *model);
	static void SetCamera(Camera *camera);
	static void SetLightGroup(LightGroup *lights);
private:

	// 機能している状態か
	bool isAlive_ = false;
	//描画しているか
	bool isDraw_ = true;

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
	static Model *sModel;
	//カメラ
	static Camera *sCamera;
	//ライト
	static LightGroup *sLights;

};

