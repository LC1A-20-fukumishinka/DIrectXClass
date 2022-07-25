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

	/// <summary>
	/// �����_�[�^�[�Q�b�g�p�̃e�N�X�`���o�b�t�@�̐������s��
	/// </summary>
	/// <param name="texBuff">�`��p�o�b�t�@</param>
	/// <param name="textureNums">�e�e�N�X�`���ԍ�</param>
	void CreateRenderTarget(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &texBuff, std::vector<int> &textureNums, const int renderTargetCount);

	ID3D12DescriptorHeap *GetDescriptorHeap();

	ID3D12Resource *GetTexBuff(int handle);

	bool CheckHandle(int handle);
private:
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[TextureMaxCount];

	int loadTextureCount;
};