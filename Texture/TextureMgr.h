#pragma once
#include "MyDirectX.h"
#include <DirectXMath.h>
class TextureMgr
{
private:
	enum Texture
	{
		TextureMaxCount = 256
	};
	TextureMgr();
	~TextureMgr();
public:
	TextureMgr(const TextureMgr &obj) = delete;

	TextureMgr operator=(const TextureMgr &obj) = delete;

	static TextureMgr *Instance();

	int SpriteLoadTexture(const wchar_t *filename);

	ID3D12DescriptorHeap *GetDescriptorHeap();

	bool CheckHandle(int handle);
private:
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[TextureMaxCount];

	int LoadTextureCount;
};