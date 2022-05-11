#pragma once
#include "Sprite.h"
#include <IGraphicsPipeline.h>
#include <array>
class MultiTexture
	//public Sprite
{
public:
	/// <summary>
	///�R���X�g���N�^
	/// </summary>
	MultiTexture();

	/// <summary>
	/// �`��R�}���h
	/// </summary>
	void Draw(PipeClass::PipelineSet *pipelineSet);

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	void PreDrawScene();

	void PostDrawScene();
private:

	//�e�N�X�`���o�b�t�@�̐���
	void MakeTextureBuffer();

	//�C���[�W�f�[�^�̓]��
	void SendImage();

	void MakeDescHeap();

	void MakeRTV();

	void MakeDepthBuffer();

	void MakeDSV();
private:
	//���W
	DirectX::XMFLOAT2 position;
	//Z�����̉�]��
	float rotation;
	//�F(RGBA)
	DirectX::XMFLOAT4 color;
	//���[���h�s��
	DirectX::XMMATRIX matWorld;
	//�傫��
	DirectX::XMFLOAT2 size;
	//�A���J�[�|�C���g
	DirectX::XMFLOAT2 anchorpoint;
	//���E���]
	bool isFlipX;
	//�㉺���]
	bool isFlipY;
	//�e�N�X�`���̍�����W
	DirectX::XMFLOAT2 texLeftTop;
	//�e�N�X�`���؂�o���T�C�Y
	DirectX::XMFLOAT2 texSize;
	//��\��
	bool isInvisible;

	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//�F(RGBA)
		DirectX::XMMATRIX mat;	//3D�ϊ��s��
	};
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos;	//xyz���W
		DirectX::XMFLOAT2 uv;	//uv���W
	};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;				//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;				//�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vBView{};			//���_�o�b�t�@�r���[
	ID3D12GraphicsCommandList *cmdList = nullptr;

	//�e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> texBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	//RTV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	//DSV�p�̃f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	//��ʃN���A�J���[
	static const float clearColor[4];

};

