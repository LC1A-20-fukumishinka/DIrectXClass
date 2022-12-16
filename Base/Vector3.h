#pragma once
#include <DirectXMath.h>

class Vector3:public DirectX::XMFLOAT3
{
public:
	//float x;
	//float y;
	//float z;

public:
	//コンストラクタ
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const DirectX::XMFLOAT3 &v);
	Vector3(const DirectX::XMVECTOR &v);
	Vector3(const Vector3 &obj);
	//メンバ関数
	float length() const;
	Vector3 normalize() const;
	float dot(const Vector3 &v)const;
	Vector3 cross(const Vector3 &v) const;

	//単項演算子
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子
	Vector3 &operator+=(const Vector3 &v);
	Vector3 &operator-=(const Vector3 &v);
	Vector3 &operator*=(float s);
	Vector3 &operator*=(const Vector3 &v);
	Vector3 &operator/=(float s);
	Vector3 &operator =(const DirectX::XMFLOAT3 &v);
};

//2項演算子
const Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator*(const Vector3 &v, float s);
const Vector3 operator*(float s, const Vector3 &v);
const Vector3 operator*(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator/(const Vector3 &v, float s);

/// <summary>
/// Vector3型をXMVECTOR型にコンバートする
/// </summary>
/// /// <param name="v">入れるベクトル</param>
/// /// <returns>帰ってくるXMVECTOR型</returns>
DirectX::XMVECTOR F3toV(const Vector3 &v);