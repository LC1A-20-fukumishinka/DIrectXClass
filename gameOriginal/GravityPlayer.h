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

	void PosMove(const Vector3 &move);

	void PostureUpdate(const Vector3 &move, bool isSetAngle);
	void FloorMove(bool isSetAngle);

	void JumpMove(bool isSetAngle);
	/// <summary>
	/// playerを右スティックで回転できるようにする
	/// </summary>
	void PlayerRotation();
	void NormalUpdate();

	void PostureReset();
private:
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
	const float jumpPower = 1.0f;

	bool oldLockonInput = false;

	bool isOneWayGravity_ = false;

	Vector3 oneWayGravityAngle_;
};

