#include "FbxModel.h"
#include "MyDirectX.h"
#include "TextureMgr.h"
FbxModel::~FbxModel()
{
	//FBX�V�[���̉��
	fbxScene->Destroy();
}
void FbxModel::CreateBuffers()
{
	HRESULT result;
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices.size());

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUvSkin *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[
	vbView.BufferLocation =
		vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//���_�C���f�b�N�X�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	//�C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	//�C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬
	ibView.BufferLocation =
		indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void FbxModel::Draw()
{
	ID3D12GraphicsCommandList *cmdList = MyDirectX::Instance()->GetCommandList();
	//���_�o�b�t�@���Z�b�g(VBV)
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g(IBV)
	cmdList->IASetIndexBuffer(&ibView);

	//�f�X�N���v�^�q�[�v�̃Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = { TextureMgr::Instance()->GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE
		(TextureMgr::Instance()->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
		material.textureHandle,
		MyDirectX::Instance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
	);
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
