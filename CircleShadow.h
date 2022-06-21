#pragma once
#include <DirectXMath.h>

class CirrcleShadow
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct ConstBufferData
	{
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float atten;
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};
public:
	static CirrcleShadow *Create();
public:
	void SetDir(const XMVECTOR &lightdir);
	void SetPos(const XMFLOAT3 &lightpos);
	void SetDistanceCasterLight(const XMFLOAT3 &distanceCasterLight);
	void SetAtten(const XMFLOAT3 &lightAtten);
	void SetFactorAngle(const XMFLOAT2 &lightFactorAngle);
	void SetDirty(const bool dirty) { this->dirty = dirty; }
	void SetActive(bool active) { this->active = active; }

	inline const XMVECTOR &GetLightDir() { return lightdir; }
	inline const XMFLOAT3 &GetLightPos() { return lightpos; }
	inline const XMFLOAT3 &GetLightColor() { return lightcolor; }
	inline const XMFLOAT3 &GetLightAtten() { return lightAtten; }
	inline const XMFLOAT2 &GetLightFactorAngleCos() { return lightFactorAngleCos; }
	inline bool IsActive() { return active; }
	inline bool IsDirty() { return dirty; }
private:
	//方向（単位ベクトル）
	XMVECTOR dir = {1, 0, 0, 0};
	//キャスターとライトの距離
	float distancecasterLight = 100.0f;
	//キャスター座標（ワールド座標系）
	XMFLOAT3 casterPos = {0, 0, 0};
	//距離減衰係数
	XMFLOAT3 
};