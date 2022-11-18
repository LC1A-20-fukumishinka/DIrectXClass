#pragma once
#include "FukuMath.h"
#include "Vector3.h"
namespace GameDatas
{
	struct GravityData
	{
		Vector3 angle;
		bool isOneWayGravity = false;
	};
	//ïœêî
	const float RotRate = 2.0f * FukuMath::degree;
	const float camMaxLength = 30.0f;
	const float camMinLength = 10.0f;
	const float maxMoveSpeed = 0.5f;
	const DirectX::XMVECTOR LockOnShift = {2.0f, 2.0f, 0.0f};
	const float baseGravityPower = 0.025f;
	const float sTitleCameraDistance = 200.0f;
}