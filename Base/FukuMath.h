#pragma once
#include <DirectXMath.h>
namespace FukuMath
{
	/// <summary>
	/// 余弦定理角cのコサインを求める
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c">対辺</param>
	/// <returns>cosC</returns>
	float CosineTheorem(const float a, const float b, const float c);

	/// <summary>
	/// 与えられたベクトルから姿勢を作る
	/// </summary>
	/// <param name="v1">上方向ベクトル</param>
	/// <param name="v2">向く方向</param>
	/// <param name="cross">ついでに右方向ベクトルを取得する場合入れておく</param>
	/// <returns>姿勢行列</returns>
	DirectX::XMMATRIX GetMatRot(const DirectX::XMVECTOR &v1, const DirectX::XMVECTOR &v2, DirectX::XMVECTOR &cross = DirectX::XMVECTOR{});

	const float PI = 3.14159265f;

	const float degree = PI / 180;

	const DirectX::XMVECTOR XVec = { 1.0f, 0.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR YVec = { 0.0f, 1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR ZVec = { 0.0f, 0.0f, 1.0f, 0.0f };

}
