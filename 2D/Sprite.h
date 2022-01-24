#pragma once
#include "../Base/MyDirectX.h"
#include <DirectXMath.h>
#include "SpriteCommon.h"

class Sprite
{
public:
	Sprite();

	void Init( UINT texnumber, DirectX::XMFLOAT2 anchorpoint = { 0.5f, 0.5f }, bool isFlipX = false, bool isFlipY = false);

	void SpriteTransferVertexBuffer();

	//�X�v���C�g�̕`��R�}���h
	void Draw();
	//�X�v���C�g���̍X�V(�s��v�Z��A�F�ύX�Ȃ�)
	void Update();


	//���W
	DirectX::XMFLOAT3 position;
	//Z�����̉�]��
	float rotation;
	//�F(RGBA)
	DirectX::XMFLOAT4 color;
	//���[���h�s��
	DirectX::XMMATRIX matWorld;
	//�e�N�X�`���ԍ�
	UINT texNumber;
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
private:
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
};
