#pragma once
#include "../Camera.h"
#include <memory>
#include "../EaseClass.h"
#include "gameConstData.h"
class Planet;
class GameCamera
{
public:
	void Init();
	void Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	void Draw();
	void Finalize();

public:

	void SetGravityData(GameDatas::GravityData isOneWayGravity);
	Camera *GetCamera();
	Vector3 GetCameraPos();
	bool GetIsCameraStop();

	void CameraStop();
	void SetBasePlanet(const std::weak_ptr<Planet> planet);

	/// <summary>
	/// 現在のカメラの状態を保存しアニメーションをスタート
	/// </summary>
	/// <param name="isTargetEase">目標座標を遷移させるか</param>
	/// <param name="EaseTimer">アニメーション時間</param>
	void StartCameraAnimation(bool isTargetEase, int EaseTimer);

	void ClearAnimationStart(const Vector3 &playerPos);


	void TitleAnimationStart();
	/// <summary>
	/// クリアからゲームに遷移
	/// </summary>
	void ClearToIngme();

	/// <summary>
	/// タイトルからゲームに遷移
	/// </summary>
	void TitleToIngame(const Vector3 &playerPos, const Vector3 &playerZVec);

	/// <summary>
	/// クリア時にカメラが向く座標
	/// </summary>
	void SetNextPlantPos(const Vector3 &pos);

	/// <summary>
	/// アニメーションが終了したかどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsAnimationEnd();
private://状況別更新処理

	/// <summary>
	/// カメラが遷移系アニメーション中の更新処理
	/// </summary>
	void CameraAnimationUpdate();

	/// <summary>
	/// ゲームプレイ中の処理
	/// </summary>
	void IngameCameraUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	//クリア時カメラ挙動処理
	void ClearCameraUpdate();
	//通常時のカメラ挙動
	void NormalUpdate(const Vector3 &playerPos, const Vector3 &playerYVec);
	//注目状態のカメラ挙動
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	//掴み状態のカメラ挙動
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
	//タイトル時のカメラ挙動
	void TitleUpdate();

	//惑星と接触していた際の更新処理
	void PlanetCollisionUpdate();
private:
	void camRot(const DirectX::XMFLOAT2 &rot);
private:
	Camera cam_;

	//計算して出す視点座標
	Vector3 nextEyePos_ = {};
	//対象座標
	Vector3 nextTargetPos_ = {};
	//カメラの姿勢
	DirectX::XMVECTOR nextCamUpRot_ = {};
	//アニメーション用に設定する過去の視点
	Vector3 oldEyePos_ = {};
	//アニメーション用に設定する過去の対象
	Vector3 oldTargetPos_ = {};
	//アニメーション用に設定する過去の姿勢
	DirectX::XMVECTOR oldCamUpRot_ = {};
	std::weak_ptr<Planet> planet_;
	bool isCameraStop_ = false;
	//次の惑星の座標
	Vector3 nextPlanetPos_ = {};
private://挙動のイージング
	bool isChangeBasePlanetAnimation_;
	Easing CameraAnimationEase_;
	bool isTargetEase_ = false;

	bool isClearMode_ = false;
	bool isTitleMode_ = true;

	GameDatas::GravityData gravity_;
	bool oldIsOneWayGravity_ = false;
};

