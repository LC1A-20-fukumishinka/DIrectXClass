#pragma once
#include <memory>
#include <DirectXMath.h>

class PointLight
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
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad;
		XMFLOAT3 lightatten;
		unsigned int active;
	};
public:
void SetLightPos(const XMFLOAT3 &lightpos);
inline const XMFLOAT3 &GetlightPos(){return lightPos;}

void SetLightColor(const XMFLOAT3 &lightcolor);
inline const XMFLOAT3 &GetLightColor(){return lightColor;}

void SetLightAtten(const XMFLOAT3 &lightAtten);
inline const XMFLOAT3 &GetLightAtten() {return lightAtten;}

void SetActive(bool active);

inline bool IsActive() {return active;}

void SetDirty(bool dirty);
inline bool GetDirty(){return dirty;}
private:
XMFLOAT3 lightPos = {0, 0, 0};
XMFLOAT3 lightColor = { 1, 1, 1 };
XMFLOAT3 lightAtten = {1.0f, 1.0f, 1.0f};
bool active = false;

bool dirty = false;
};