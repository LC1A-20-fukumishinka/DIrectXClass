#pragma once
#include "../Collision/BaseCollider.h"
#include "../Collision/CollisionPrimitive.h"
#include <DirectXMath.h>

/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
private:
	using XMVECTOR = DirectX::XMVECTOR;
public:
	SphereCollider(XMVECTOR offset = { 0, 0, 0, 0 }, float radius = 1.0f):
		offset(offset),
		radius(radius)
	{
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	void Update() override;

	inline void SetRadius(float radius){this->radius = radius;}

	private:
	//オブジェクト中心からのオフセット
	XMVECTOR offset;
	//半径
	float radius;

};