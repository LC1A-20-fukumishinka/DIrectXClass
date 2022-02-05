#pragma once
#include "CollisionType.h"
#include "Object3D.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
BaseCollider() = default;
//仮想デストラクタ
virtual ~BaseCollider() = default;

inline void SetObject(Object3D *object)
{
	this->object3d = object;
}

inline Object3D *GetObject3D()
{
	return object3d;
}

/// <summary>
/// 更新
/// </summary>
virtual void Update() = 0;

//形状タイプの取得
inline CollisionShapeType GetShapeType() 
{
	return shapeType;
}

protected:
Object3D *object3d = nullptr;
//形状タイプ
CollisionShapeType shapeType = SHAPE_UNKNOWN;
};