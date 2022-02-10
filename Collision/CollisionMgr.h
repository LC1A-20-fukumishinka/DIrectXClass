#pragma once
#include <forward_list>
#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include <d3d12.h>
class BaseCollider;

class CollisionManager
{
public://静的メンバ関数
	static CollisionManager *GetInstance();

public:
	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void AddCollider(BaseCollider *collider)
	{
		colliders.push_front(collider);
	}
	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void RemoveCollider(BaseCollider *collider)
	{
		colliders.remove(collider);
	}

	/// <summary>
	/// 全ての衝突チェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="hitInfo">衝突情報</param>
	/// <param name="maxDistance">最大距離</param>
	/// <returns>例が任意のコライダーと交わる場合はtrue, それ以外はfalse</returns>
	bool Raycast(const Ray &ray, RaycastHit *hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager &operator=(const CollisionManager&)= delete;
	//コライダーのリスト
	std::forward_list<BaseCollider *> colliders;
};
