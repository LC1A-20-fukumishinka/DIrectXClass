#pragma once
#include "Sprite.h"
class PostEffect :
	public Sprite
{
public:
	/// <summary>
	///�R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �`��R�}���h
	/// </summary>
	void Draw();

	/// <summary>
	/// ������
	/// </summary>
	void Init(UINT texnumber);
private:

	//�e�N�X�`���o�b�t�@�̐���
	void MakeTextureBuffer();

	//�C���[�W�f�[�^�̓]��
	void SendImage();

	void MakeDescHeap();
private:
	ID3D12CommandList *cmdList = nullptr;

	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

