#pragma once
#include <DirectXMath.h>

class SpotLight
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};
public:
	static SpotLight *Create();
public:
	void SetLightDir(const XMVECTOR &lightdir);
	void SetLightPos(const XMFLOAT3 &lightpos) ;
	void SetLightColor(const XMFLOAT3 &lightcolor);
	void SetLightAtten(const XMFLOAT3 &lightAtten);
	void SetLightFactorAngle(const XMFLOAT2 &lightFactorAngle);
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
	//���C�g����(�P�ʃx�N�g��)
	XMVECTOR lightdir = { 1.0f,0.0f ,0.0f ,0.0f };
	//���C�g���W(���[���h���W�n)
	XMFLOAT3 lightpos = { 0.0f,0.0f ,0.0f };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1.0f,1.0f, 1.0f };
	//���C�g���������W��
	XMFLOAT3 lightAtten = { 1.0f , 1.0f , 1.0f };
	//���C�g�����p�x(�J�n�p�x�A�I���p�x)
	XMFLOAT2 lightFactorAngleCos = {0.5f, 0.2f};
	//�L���t���O
	bool active = false;
	//�_�[�e�B�[�t���O
	bool dirty = false;

};