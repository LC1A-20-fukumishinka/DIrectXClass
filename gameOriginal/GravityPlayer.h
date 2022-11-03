#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include "../Collision/CollisionPrimitive.h"
class Planet;

enum class PlayerStatus
{
	STAND,
	JUMP,
};
class GravityPlayer
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
public://基本関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="model"></param>
	void Init(Model *model, std::shared_ptr<Planet> planet);
	void Update();
	void Finalize();
	void Draw();
	void ShadowDraw();
private:
	/// <summary>
	/// スティックによる移動関数
	/// </summary>
	/// <param name="isSetAngle">入力による向きの変化をつけるか</param>
	void Move(bool isSetAngle);

	/// <summary>
	/// 座標の移動
	/// </summary>
	/// <param name="move">スティック入力の移動量</param>
	void PosUpdate(const Vector3 &move);

	/// <summary>
	/// 姿勢の変形
	/// </summary>
	/// <param name="move">スティック入力の移動移動</param>
	void PostureUpdate(const Vector3 &move);
	void FloorMove(bool isSetAngle);

	void JumpMove(bool isSetAngle);
	/// <summary>
	/// playerを右スティックで回転できるようにする
	/// </summary>
	void PlayerRotation();

	void PostureReset();
private://状態毎の挙動管理

	//通常時挙動
	void NormalUpdate();

	void LockOnUpdate();
public://ゲッタセッタ
	void SetPos(const DirectX::XMFLOAT3 &pos);

	void SetRotation(const DirectX::XMFLOAT3 &rot);
	void SetModel(Model *model);
	void SetCamera(Camera *cam);
	void SetShadowCamera(Camera *cam);
	void SetLight(LightGroup *lights);
	const XMFLOAT3 &GetPos();
	const XMFLOAT3 GetAngle();
	const XMFLOAT3 GetUpVec();
	const float GetBasePlanetScale();
	const std::weak_ptr<Planet> &GetBasePlanet();
	const XMFLOAT3 &GetGravityAngle();
	const bool GetIsJump();
public://惑星周りの処理
	/// <summary>
	/// 掴む惑星のセットを行う
	/// </summary>
	/// <param name="planet">掴む惑星のポインタ</param>
	void SetGrabPlanet(std::shared_ptr<Planet> planet);

	void SetBasePlanet(std::shared_ptr<Planet> planet);

	/// <summary>
	/// 掴んでいる惑星を離す
	/// </summary>
	void ReleasePlanet();

	//掴んでいる際の更新処理ひょっとしたら要らないかも
	void GrabUpdate();

	void BlockCollision(const std::vector<Triangle> &boxPlanes);

	void AddGravity(Vector3 gravity);

	void shakeUpdate(float shakePower);
private:
	Vector3 pos;
	Vector3 rotation;
	Vector3 scale;
	Object3D drawObject;
	Object3D shadowObject;
	Camera *cam;
	Camera *shadowCamera;
	std::weak_ptr<Planet> grabPlanet;

	std::weak_ptr<Planet> basePlanet;

	PlayerStatus status;

	DirectX::XMVECTOR playerAngle;

	Vector3 warkVec_;

	Vector3 gravityVec_;

	Vector3 moveVec_;
	//惑星と自分の距離
	float baseLength = 0.0f;

	//惑星からの高さを(位置)
	float localHeight = 0.0f;
	//ジャンプ中の速度(速度)
	float jumpSpeed = 0.0f;

	//ジャンプ力
	const float jumpPower = 0.5f;

	bool oldLockonInput = false;

	bool isOneWayGravity_ = false;

	//単方向時重力
	Vector3 oneWayGravityAngle_;

	//世界からの重力
	Vector3 worldGravity_;

	//プレイヤーに最終的にかかる重力
	Vector3 gravityAngle_;
};

