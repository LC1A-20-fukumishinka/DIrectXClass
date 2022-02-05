#pragma once
#include <DirectXMath.h>

class Object3D;
class BaseCollider;
/// <summary>
/// �Փˏ��
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

	//�Փˑ���I�u�W�F�N�g
	Object3D * object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider *collider = nullptr;
	//�Փ˓X
	DirectX::XMVECTOR inter;
};