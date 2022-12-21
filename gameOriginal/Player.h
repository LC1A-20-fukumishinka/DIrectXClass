#pragma once
#include "Object3D.h"
#include "Vector3.h"
#include "Model.h"
#include <memory>
#include "gameConstData.h"
#include "../Collision/CollisionPrimitive.h"
class Planet;

class Player
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
public://基本関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="model"></param>
	void Init(std::shared_ptr<Planet> planet);
	void Update();
	void Finalize();
	void Draw();
	void ShadowDraw();

	void ImguiDraw();
public://固有関数
	void Boost();
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
	const XMVECTOR GetPosture();
	const Vector3 &GetMoveVec();
	float GetBasePlanetScale();
	const std::weak_ptr<Planet> &GetBasePlanet();
	GameDatas::GravityData GetGravityData();
	GameDatas::PlayerStatus GetPlayerStatus();
	bool GetIsOneWayGravity();
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

	void LoadModel();

	bool LandingCamReset();

private://アニメーション関連
	//プレイヤーキャラクターの表情テクスチャハンドル
	std::vector<int> faceTextureHandles_;
	//現在の表情
	GameDatas::PlayerFaceTexture face_ = GameDatas::PlayerFaceTexture::NORMAL;

	int blinkTimer_ = 300;
	void FaceUpdate();
private:
	Vector3 pos_;
	Object3D drawObject_;
	Object3D leftTrackObject_;
	Object3D rightTrackObject_;

	Object3D shadowObject_;
	Camera *cam_;
	std::weak_ptr<Planet> grabPlanet;

	std::weak_ptr<Planet> basePlanet;

	//プレイヤーの状態
	GameDatas::PlayerStatus status_;
	//位置フレーム前の状態
	GameDatas::PlayerStatus oldStatus_;
	//重力を変更したか
	bool isGravityChanged_ = false;

	DirectX::XMVECTOR playerAngle;

	Vector3 warkVec_;


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

	GameDatas::GravityData gravity_;
	//単方向時重力
	Vector3 oneWayGravityAngle_;

	//世界からの重力
	Vector3 worldGravity_;

	//プレイヤーに最終的にかかる重力

	int shakeTimer_ = 5;
	int shakeTimerMax_ = 15;

	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> leftTrackModel_;
	std::unique_ptr<Model> rightTrackModel_;

	std::unique_ptr<Model> ArrowModel_;
};

