#include "FukuMath.h"

using namespace DirectX;
float FukuMath::CosineTheorem(const float a, const float b, const float c)
{
	float bunbo = (a*b*2);
	float bunshi = ((a*a) + (b*b) - (c*c));
	return (bunshi / bunbo);
}

DirectX::XMMATRIX FukuMath::GetMatRot(DirectX::XMVECTOR &v1, DirectX::XMVECTOR &v2, DirectX::XMVECTOR &cross)
{
	XMVECTOR upVector = XMVECTOR{ 0, 1, 0, 0 };

	//(仮の) 上方向
	if (!XMVector3Equal(v2, XMVectorZero()) && !XMVector3IsInfinite(v2))
	{
		upVector = v2;
	}

	//カメラZ軸
	XMVECTOR frontAxisZ = v1;


	//正面ベクトルが0だった場合
	bool isZero = XMVector3Equal(frontAxisZ, XMVectorZero());
	//正面ベクトルが無限だったら
	bool isInfinite = XMVector3IsInfinite(frontAxisZ);
	bool isException = isZero && isInfinite;
	//0ベクトルだと向が定まらないので除外
	if (isException)
	{
		frontAxisZ = {0.0f,0.0f ,1.0f , 0.0f };
	}


	//上ベクトルと正面ベクトルが同一だったら上ベクトルの方向を変更
	if (XMVector3Equal(upVector, frontAxisZ))
	{//上ベクトルを変更する
		XMVECTOR upVector = XMVECTOR{ 0, 0, -1, 0 };
	}

	//ベクトルの正規化
	frontAxisZ = XMVector3Normalize(frontAxisZ);

	//カメラのX軸
	XMVECTOR rightAxisX;
	//X軸は上方向→Z軸の外積で求まる
	rightAxisX = XMVector3Cross(upVector, frontAxisZ);
	//ベクトルを正規化
	rightAxisX = XMVector3Normalize(rightAxisX);
	//右方向ベクトル
	cross = rightAxisX;
	//カメラのY軸
	XMVECTOR upAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	upAxisY = XMVector3Cross(frontAxisZ, rightAxisX);
	//ベクトルの正規化
	upAxisY = XMVector3Normalize(upAxisY);
	//上方向ベクトルを書き換える
	v2 = upAxisY;
	//カメラ回転行列
	XMMATRIX matRot;
	//カメラ座標系→ワールド座標系の変換行列
	matRot.r[0] = rightAxisX;
	matRot.r[1] = upAxisY;
	matRot.r[2] = frontAxisZ;
	matRot.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//ローカル変数返してる
	return matRot;
}
