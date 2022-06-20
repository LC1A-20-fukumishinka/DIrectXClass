#pragma once
#include "FukuMath.h"
#include "Vector3.h"
namespace GameDatas
{
	static const float RotRate = 2.0f * FukuMath::degree;
	static const float camMaxRength = 15.0f;
	static const float camMinRength = 5.0f;
	static const DirectX::XMVECTOR LockOnShift = {2.0f, 2.0f, 0.0f};
}