#pragma once
#include <DirectXMath.h>

class Object3D;
class BaseCollider;
/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(Object3D *object, BaseCollider *collider, const DirectX::XMVECTOR &inter)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	//衝突相手オブジェクト
	Object3D * object = nullptr;
	//衝突相手のコライダー
	BaseCollider *collider = nullptr;
	//衝突店
	DirectX::XMVECTOR inter;
};