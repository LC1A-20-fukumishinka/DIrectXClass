#pragma once
#include <DirectXMath.h>
namespace FukuMath
{
	/// <summary>
	/// �]���藝�pc�̃R�T�C�������߂�
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c">�Ε�</param>
	/// <returns>cosC</returns>
	float CosineTheorem(const float a, const float b, const float c);

	/// <summary>
	/// �^����ꂽ�x�N�g������p�������
	/// </summary>
	/// <param name="v1">������x�N�g��</param>
	/// <param name="v2">��������</param>
	/// <param name="cross">���łɉE�����x�N�g�����擾����ꍇ����Ă���</param>
	/// <returns>�p���s��</returns>
	DirectX::XMMATRIX GetMatRot(const DirectX::XMVECTOR &v1, const DirectX::XMVECTOR &v2, DirectX::XMVECTOR &cross = DirectX::XMVECTOR{});

	const float PI = 3.14159265f;

	const float degree = PI / 180;

	const DirectX::XMVECTOR XVec = { 1.0f, 0.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR YVec = { 0.0f, 1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR ZVec = { 0.0f, 0.0f, 1.0f, 0.0f };

}
