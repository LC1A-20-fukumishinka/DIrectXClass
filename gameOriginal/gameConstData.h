#pragma once
#include "FukuMath.h"
#include "Vector3.h"
namespace GameDatas
{

	//重力のデータ
	struct GravityData
	{
		Vector3 angle;
		bool isOneWayGravity = false;
	};

	//ゲートを通過したかどうかの
	struct CollisionGateData
	{
		//接触したか
		bool isCollision;
		//接触したゲートの色
		Vector3 color;
	};
	//プレイヤーが取りうる状態
	enum class PlayerStatus
	{
		STAND,
		JUMP,
	};

	//キャラクターの顔の表情分類
	enum class PlayerFaceTexture
	{
		NORMAL,
		BLINK,
		SMILE,
		STRIVE,
	};
	//変数
		//スティック入力によるカメラの回転速度
	const float RotRate = 2.0f * FukuMath::degree;
	//カメラがキャラクター取りうる最大距離
	const float camMaxLength = 30.0f;
	//キャラクターのスティックによる移動の最高速度
	const float maxMoveSpeed = 0.5f;
	//主観視点になった時にキャラクタからずらす位置
	const DirectX::XMVECTOR LockOnShift = {2.0f, 2.0f, 0.0f};
	//重力の基準量
	const float baseGravityPower = 0.025f;
	//タイトル時に取るカメラの距離
	const float sTitleCameraDistance = 200.0f;
}