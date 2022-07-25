#include "MultiTexture.h"
#include <d3dx12.h>
#include "MyDirectX.h"
#include "TextureMgr.h"
#include "BaseData.h"
#include "DirectInput.h"
using namespace DirectX;
const float MultiTexture::clearColor[4] = { 0.2f,0.2f ,0.2f ,0.0f };

MultiTexture::MultiTexture()
{
	cmdList = MyDirectX::Instance()->GetCommandList();
}

void MultiTexture::Draw(PipeClass::PipelineSet *pipelineSet)
{
	//�`��t���O��true����Ȃ������瑁�����^�[��
	if (isInvisible) return;

	//if (!TextureMgr::Instance()->CheckHandle(texNumber))
	//{
	//	assert(0);
	//	return;
	//}
	MyDirectX *myD = MyDirectX::Instance();
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();

	//if (Input::Instance()->KeyTrigger(DIK_0))
	//{
	//	static int tex = 0;
	//	tex = (tex + 1) % 2;

	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//	srvDesc.Texture2D.MipLevels = 1;
	//	device->CreateShaderResourceView(texBuff[tex].Get(),
	//		&srvDesc,
	//		descHeapSRV->GetCPUDescriptorHandleForHeapStart());
	//}


	//�p�C�v�����X�e�[�g�̐ݒ�
	myD->GetCommandList()->SetPipelineState(pipelineSet->pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet->rootSignature.Get());
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
	myD->GetCommandList()->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(),
			0,
			myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	myD->GetCommandList()->SetGraphicsRootDescriptorTable(
		2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(),
			1,
			myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//�`��R�}���h
	myD->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}

void MultiTexture::Init()
{

	//���N���X�Ƃ��Ă̏�����
	//Sprite::Init(texnumber);

	anchorpoint = { 0.5f, 0.5f };
	isFlipX = false;
	isFlipY = false;
	position = { 0, 0 };
	color = { 1, 1, 1, 1 };
	rotation = 0.0f;

	size = { 100, 100 };
	texLeftTop = { 0, 0 };
	texSize = { 0 , 0 };
	isInvisible = false;


	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::Instance();


	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},//����
		{{-1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},//����
		{{+1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},//�E��
		{{+1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},//�E��
	};

	//���_�o�b�t�@�̐���
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	//�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
	//���_�o�b�t�@�r���[�̍쐬
	vBView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vBView.SizeInBytes = sizeof(vertices);
	vBView.StrideInBytes = sizeof(vertices[0]);
	//�萔�o�b�t�@�̐���
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	XMFLOAT4 tmpColor = {clearColor[0],clearColor[1] ,clearColor[2] ,clearColor[3] };
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = tmpColor;//�F�w��(RGBA)
	constMap->mat = XMMatrixIdentity();	//���s�������e
	constBuff->Unmap(0, nullptr);

}