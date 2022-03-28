#include "PostEffect.h"
#include <d3dx12.h>
#include "MyDirectX.h"
#include "TextureMgr.h"
#include "SpriteCommon.h"
#include "BaseData.h"
PostEffect::PostEffect()
{
	cmdList = MyDirectX::Instance()->GetCommandList();
}

void PostEffect::Draw()
{
	//�`��t���O��true����Ȃ������瑁�����^�[��
	if (isInvisible) return;

	if (!TextureMgr::Instance()->CheckHandle(texNumber))
	{
		assert(0);
		return;
	}

	MyDirectX *myD = MyDirectX::Instance();
	//�p�C�v�����X�e�[�g�̐ݒ�
	myD->GetCommandList()->SetPipelineState(SpriteCommon::Instance()->pipelineSet.pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	myD->GetCommandList()->SetGraphicsRootSignature(SpriteCommon::Instance()->pipelineSet.rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap *ppHeaps[] = { descHeapSRV.Get() };
	myD->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);


	//���_�o�b�t�@���Z�b�g
	myD->GetCommandList()->IASetVertexBuffers(0, 1, &vBView);

	//�萔�o�b�t�@���Z�b�g
	myD->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	myD->GetCommandList()->SetGraphicsRootDescriptorTable(1,descHeapSRV->GetGPUDescriptorHandleForHeapStart());
	//�`��R�}���h
	myD->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::Init(UINT texnumber)
{

	//���N���X�Ƃ��Ă̏�����
	Sprite::Init(texnumber);

	MakeTextureBuffer();
	SendImage();
	MakeDescHeap();
}

void PostEffect::MakeTextureBuffer()
{
	HRESULT result;

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	DXGI_FORMAT_R8G8B8A8_UNORM,
	WINDOW_WIDTH,
	(UINT)WINDOW_HEIGHT,
	1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	result = MyDirectX::Instance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));
}

void PostEffect::SendImage()
{	//�e�N�X�`����ԃN���A

	HRESULT result;

	//��f��(1280 * 720 = 921600)�s�N�Z��
	const UINT pixelCount = WINDOW_WIDTH * WINDOW_HEIGHT;
	//�摜1�s���̃f�[�^�T�C�Y
	const UINT rowPitch = sizeof(UINT) * WINDOW_WIDTH;
	//�摜�S�̂̃f�[�^�T�C�Y
	const UINT depthPitch = rowPitch * WINDOW_HEIGHT;
	//�摜�C���[�W
	UINT * img = new UINT[pixelCount];
	for(int i = 0;i< pixelCount; i++){img[i] = 0xff0000ff; };

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff->WriteToSubresource(0, nullptr, 
	img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));
	delete[] img;
}

void PostEffect::MakeDescHeap()
{
	HRESULT result;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v
	device->CreateShaderResourceView(texBuff.Get(), 
	&srvDesc,
	descHeapSRV->GetCPUDescriptorHandleForHeapStart());
}
