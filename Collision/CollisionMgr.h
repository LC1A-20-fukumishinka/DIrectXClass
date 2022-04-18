#pragma once
#include <forward_list>
#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include <d3d12.h>
class BaseCollider;

class CollisionManager
{
public://�ÓI�����o�֐�
	static CollisionManager *GetInstance();

public:
	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void AddCollider(BaseCollider *collider)
	{
		colliders.push_front(collider);
	}
	/// <summary>
	/// �R���C�_�[�̍폜
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void RemoveCollider(BaseCollider *collider)
	{
		colliders.remove(collider);
	}

	/// <summary>
	/// �S�Ă̏Փ˃`�F�b�N
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// ���C�L���X�g
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="hitInfo">�Փˏ��</param>
	/// <param name="maxDistance">�ő勗��</param>
	/// <returns>�Ⴊ�C�ӂ̃R���C�_�[�ƌ����ꍇ��true, ����ȊO��false</returns>
	bool Raycast(const Ray &ray, RaycastHit *hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager &operator=(const CollisionManager&)= delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider *> colliders;
};