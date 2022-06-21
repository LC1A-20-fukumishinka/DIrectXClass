#pragma once
#include "../Collision/BaseCollider.h"
#include <DirectXMath.h>

class Object3d;

/// <summary>
/// ���C�L���X�g�ɂ����𓾂�ׂ̍\����
/// </summary>
struct RaycastHit
{
//�Փˑ���̃I�u�W�F�N�g
	Object3D *object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider *collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
	//�Փ˓_�܂ł̋���
	float distance = 0.0f;
};