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
	/// �x�N�g�����炻�̌����������s��𐶐�����
	/// </summary>
	/// <param name="angle">��������</param>
	/// <param name="up">������x�N�g��</param>
	/// <param name="right">�E�����x�N�g�����擾����ꍇ����Ă���</param>
	/// <returns></returns>
	const DirectX::XMMATRIX &GetMatRot(DirectX::XMVECTOR &angle, DirectX::XMVECTOR &up = DirectX::XMVECTOR {}, DirectX::XMVECTOR &right = DirectX::XMVECTOR{});

	const float PI = 3.14159265f;

	const float degree = PI / 180;

	const DirectX::XMVECTOR XVec = { 1.0f, 0.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR YVec = { 0.0f, 1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR ZVec = { 0.0f, 0.0f, 1.0f, 0.0f };

}
