#pragma once
#include "../Base/MyDirectX.h"
#include<DirectXMath.h>



class SpriteCommon
{
public:
	enum Sprites
	{
		spriteSRVCount = 512
	};
	//�p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	//�ˉe�s��
	DirectX::XMMATRIX matProjection{};

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[spriteSRVCount];


	void Init();

	void SpriteLoadTexture(UINT texnumber, const wchar_t *filename);
private:

	MyDirectX *myDirectX;
	//�X�v���C�g�̃p�C�v���C���ݒ�֐�
void SpriteCreateGraphPipeline(ID3D12Device *dev);
};