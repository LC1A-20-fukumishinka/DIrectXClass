#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>


class Light
{
private://�G�C���A�X
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
	};
public:
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
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
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���C�g�̌��������i�P�ʃx�N�g���j
	XMVECTOR lightdir = {1, 0, 0, 0};
	//���C�g�F
	XMFLOAT3 lightcolor = {1, 1, 1};
	//�_�[�e�B�t���O
	bool dirty = false;
};