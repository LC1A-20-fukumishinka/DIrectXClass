#pragma once
#include "../Camera.h"
#include <memory>
#include "../EaseClass.h"
class Planet;
class GameCamera
{
public:
	void Init();
	void Update(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	void Draw();
	void Finalize();

public:
	Camera *GetCamera();
	Vector3 GetCameraPos();
	bool GetIsChangeBasePlanet();

	void SetBasePlanet(const std::weak_ptr<Planet> planet);
	/// <summary>
	/// 惑星を変更した
	/// </summary>
	void IsAnimationOn();

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
	void SetNextPlantPos(const Vector3 pos);

	/// <summary>
	/// アニメーションが終了したかどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsAnimationEnd();
private:
	static const float sTitleCameraDistance;
private:
	void CameraAnimationUpdate();
	void IngameCameraUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
private://状況別更新処理
	//クリア時カメラ挙動処理
	void ClearCameraUpdate();
	//通常時のカメラ挙動
	void NormalUpdate(const Vector3 &playerPos);
	//注目状態のカメラ挙動
	void LockonUpdate(const Vector3 &playerPos, const Vector3 &playerZVec, const Vector3 &playerYVec);
	//掴み状態のカメラ挙動
	void GrabUpdate(const Vector3 &playerPos, const Vector3 &playerZVec);
	//タイトル時のカメラ挙動
	void TitleUpdate();

private:
	void camRot(DirectX::XMFLOAT2 rot);
private:
	Camera cam;

	Vector3 nextEyePos = {};
	Vector3 nextTargetPos = {};
	DirectX::XMVECTOR nextCamUpRot = {};
	Vector3 oldEyePos = {};
	Vector3 oldTargetPos = {};
	DirectX::XMVECTOR oldCamUpRot = {};
	std::weak_ptr<Planet> planet;
	bool isChangeBasePlanet = false;
	//次の惑星の座標
	Vector3 nextPlanetPos_ = {};
private://挙動のイージング
	bool isChangeBasePlanetAnimation;
	Easing CameraAnimationEase;
	bool isTargetEase = false;

	bool isClearMode_ = false;
	bool isTitleMode_ = true;
};

