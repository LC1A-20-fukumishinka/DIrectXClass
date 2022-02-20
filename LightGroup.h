#pragma once
#include "Light.h"
#include "PointLight.h"
#include <DirectXMath.h>
class LightGroup
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct ConstBufferData
	{
		XMFLOAT3 ambientColor;
		float pad1;
		Light::ConstBufferData dirLight;
		PointLight::ConstBufferData pointLight;
	};
public:
	static LightGroup *Create();
public:
	void SetLight(Light *light);

	void SetPoinntLight(PointLight *pointLight);

	Light *GetLight() { return light; }

	PointLight *GetPoinntLight() { return pointLight; }
	void SetAmbientColor(XMFLOAT3 ambientColor);
public:
	LightGroup();
	void Init();
	void Update();
	void Draw(UINT rootParameterIndex);

	void TransferConstBuffer();
private:
	Light *light = nullptr;
	PointLight *pointLight = nullptr;
	ComPtr<ID3D12Resource> constBuff;
	XMFLOAT3 ambientColor = {0.0f,0.0f ,0.0f };
	bool dirty = false;
};