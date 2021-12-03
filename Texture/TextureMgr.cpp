#include "TextureMgr.h"
#include <cassert>
#include <DirectXTex.h>
TextureMgr::TextureMgr()
{
	MyDirectX *myDirectX = MyDirectX::GetInstance();

	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = TextureMaxCount;
	HRESULT result = myDirectX->GetDevice()->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&descHeap));
	LoadTextureCount = 0;
}

TextureMgr::~TextureMgr()
{
}

TextureMgr *TextureMgr::Instance()
{
	static TextureMgr instance;
	return &instance;
}

int TextureMgr::SpriteLoadTexture(const wchar_t *filename)
{
	MyDirectX *myDirectX = MyDirectX::GetInstance();
	//�ُ�Ȕԍ��̎w������o
	assert(LoadTextureCount <= TextureMaxCount - 1);
	HRESULT result;

	//WIC�e�N�X�`���̃��[�h
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,	//�uResources�v�t�H���_�́utexuture.png�v
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const DirectX::Image *img = scratchImg.GetImage(0, 0, 0);	//���f�[�^���o

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
#pragma region texBuff

	result = myDirectX->GetDevice()->CreateCommittedResource(	//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuff[LoadTextureCount]));
#pragma endregion

#pragma region texDataSend
	result = texBuff[LoadTextureCount]->WriteToSubresource(
		0,
		nullptr,							//�S�̈�փR�s�[
		img->pixels,							//���f�[�^�A�h���X
		(UINT16)img->rowPitch,		//1���C���T�C�Y
		(UINT16)img->slicePitch	//�S�T�C�Y
	);

	//delete[] imageData;
#pragma endregion
#pragma region SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};		//�ݒ�\����
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	myDirectX->GetDevice()->CreateShaderResourceView(
		texBuff[LoadTextureCount].Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,							//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeap->GetCPUDescriptorHandleForHeapStart(), LoadTextureCount,
			myDirectX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
#pragma endregion

	//�ǂݍ��񂾉摜�̐������Z����
	LoadTextureCount++;

	int textureNumber = LoadTextureCount - 1;

	return textureNumber;
}

ID3D12DescriptorHeap *TextureMgr::GetDescriptorHeap()
{
	return descHeap.Get();
}

ID3D12Resource *TextureMgr::GetTexBuff(int handle)
{
	return texBuff[handle].Get();
}

bool TextureMgr::CheckHandle(int handle)
{
	bool isCheck = false;
		if (handle < LoadTextureCount && handle >= 0)
	{
			isCheck = true;
	}
	return isCheck;
}
