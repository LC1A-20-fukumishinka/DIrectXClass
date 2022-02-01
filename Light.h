#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>


class Light
{
private://エイリアス
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://サブクラス
	//定数バッファ用データ
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
	};
public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device *device);

	static Light *Create();
private:
	static ID3D12Device *device;
public:
	Light();
	~Light();


	void Initialize();

	void Update();

	void Draw(UINT rootParameterIndex);

	void TransferConstBuffer();

	void SetLightDir(const XMVECTOR &lightdir);

	void SetLightColor(const XMFLOAT3 &lightcolor);
private:
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//ライトの光線方向（単位ベクトル）
	XMVECTOR lightdir = {1, 0, 0, 0};
	//ライト色
	XMFLOAT3 lightcolor = {1, 1, 1};
	//ダーティフラグ
	bool dirty = false;
};