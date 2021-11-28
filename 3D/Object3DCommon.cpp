#include "Object3DCommon.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>


using namespace DirectX;
Object3DCommon::Object3DCommon()
{
}

void Object3DCommon::Init()
{
	MyDirectX *myDirectX = MyDirectX::GetInstance();


	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = Object3DSRVCount;
	HRESULT result = myDirectX->GetDevice()->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&descHeap));

	CornInit();

	BoxInit();

	PlaneInit();
}

void Object3DCommon::SpriteLoadTexture(UINT texnumber, const wchar_t *filename)
{
	MyDirectX *myDirectX = MyDirectX::GetInstance();
	//�ُ�Ȕԍ��̎w������o
	assert(texnumber <= Object3DSRVCount - 1);
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
		IID_PPV_ARGS(&texBuff[texnumber]));
#pragma endregion

#pragma region texDataSend
	result = texBuff[texnumber]->WriteToSubresource(
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
		texBuff[texnumber].Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,							//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,
			myDirectX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
#pragma endregion


}


void Object3DCommon::CornInit()
{

	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::GetInstance();

	Vertex vertices[5] = {};

	unsigned short indices[CornNumIndices] = {};
	//���_�o�b�t�@����
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&cornVertBuff)
	);

	//�C���f�b�N�X�o�b�t�@����
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�C���f�b�N�X��񂪓��镪�̃T�C�Y
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&cornIndexBuff));


	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
	CornTransferIndexBufferVertexBuffer();

	int size = sizeof(indices);
	//���_�o�b�t�@�r���[�̍쐬
	cornVBView.BufferLocation = cornVertBuff->GetGPUVirtualAddress();
	cornVBView.SizeInBytes = sizeof(vertices);
	cornVBView.StrideInBytes = sizeof(vertices[0]);

	cornIBView.BufferLocation = cornIndexBuff->GetGPUVirtualAddress();
	cornIBView.Format = DXGI_FORMAT_R16_UINT;
	cornIBView.SizeInBytes = sizeof(indices);
}

void Object3DCommon::CornTransferIndexBufferVertexBuffer()
{
	const float topHeight = 1.0f;
	float radius = 1.0f;
	float pi = XM_PI;
	Vertex vertices[5];
#pragma region pos
	for (int i = 0; i < 3; i++)
	{
		vertices[i].pos =
		{
			radius * sinf(2 * pi / 3 * i),
			radius * cosf(2 * pi / 3 * i),
			0
		};
	}
	vertices[3].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[4].pos = XMFLOAT3(0.0f, 0.0f, -topHeight);

#pragma endregion

#pragma region uv
	for (int i = 0; i < 5; i++)
	{
		vertices[i].uv = XMFLOAT2(0, 0);
	}

	vertices[4].uv = XMFLOAT2(1, 1);
#pragma endregion

#pragma region IndexData
	unsigned short indices[CornNumIndices] = {};
	for (int i = 0; i < 3; i++)
	{
		indices[i * 3 + 1] = i;
		indices[i * 3 + 9] = i;
		if (i >= 2)
		{
			indices[i * 3] = 0;
			indices[i * 3 + 1 + 9] = 0;
		}
		else
		{
			indices[i * 3] = i + 1;
			indices[i * 3 + 1 + 9] = i + 1;
		}

		indices[i * 3 + 2] = 3;
		indices[i * 3 + 2 + 9] = 4;
	}

#pragma endregion

#pragma region Normal
	for (int i = 0; i < _countof(indices) / 3; i++)
	{

		//�O�p�`1���Ɍv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short i0 = indices[i * 3 + 0];
		unsigned short i1 = indices[i * 3 + 1];
		unsigned short i2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[i0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[i1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[i2].pos);

		//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z(�x�N�g���̌v�Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[i0].normal, normal);
		XMStoreFloat3(&vertices[i1].normal, normal);
		XMStoreFloat3(&vertices[i2].normal, normal);
	}
#pragma endregion

	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex *vertMap = nullptr;
	HRESULT result = cornVertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	//�}�b�v������
	cornVertBuff->Unmap(0, nullptr);

	//GPU��̃o�b�t�@�Ɂu�Ή��������z�������̎擾
	unsigned short *indexMap = nullptr;
	result = cornIndexBuff->Map(0, nullptr, (void **)&indexMap);

	memcpy(indexMap, indices, sizeof(indices));

	//�q���������
	cornIndexBuff->Unmap(0, nullptr);

}

void Object3DCommon::BoxInit()
{

	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::GetInstance();

	Vertex vertices[24] = {};

	unsigned short indices[BoxNumIndices] = {};	
	//���_�o�b�t�@����
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&boxVertBuff)
	);

	//�C���f�b�N�X�o�b�t�@����
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�C���f�b�N�X��񂪓��镪�̃T�C�Y
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&boxIndexBuff));


	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
	BoxTransferIndexBufferVertexBuffer();

	//���_�o�b�t�@�r���[�̍쐬
	boxVBView.BufferLocation = boxVertBuff->GetGPUVirtualAddress();
	boxVBView.SizeInBytes = sizeof(vertices);
	boxVBView.StrideInBytes = sizeof(vertices[0]);

	boxIBView.BufferLocation = boxIndexBuff->GetGPUVirtualAddress();
	boxIBView.Format = DXGI_FORMAT_R16_UINT;
	boxIBView.SizeInBytes = sizeof(indices);
}


void Object3DCommon::BoxTransferIndexBufferVertexBuffer()
{
#pragma region VertexData
	Vertex vertices[] = {
	//�O
	{{ -1.0f, -1.0f,  -1.0f},{},{0.0f, 1.0f}},	//����
	{{ -1.0f,  1.0f,  -1.0f},{},{0.0f, 0.0f}},	//����
	{{  1.0f, -1.0f,  -1.0f},{},{1.0f, 1.0f}},	//�E��
	{{  1.0f,  1.0f,  -1.0f},{},{1.0f, 0.0f}},	//�E��

	//��
	{{  1.0f, -1.0f,   1.0f},{},{1.0f, 1.0f}},	//�E��
	{{  1.0f,  1.0f,   1.0f},{},{1.0f, 0.0f}},	//�E��
	{{ -1.0f, -1.0f,   1.0f},{},{0.0f, 1.0f}},	//����
	{{ -1.0f,  1.0f,   1.0f},{},{0.0f, 0.0f}},	//����


	//��
	{{  -1.0f, -1.0f, -1.0f},{},{0.0f, 1.0f}},	//����
	{{  -1.0f, -1.0f,  1.0f},{},{0.0f, 0.0f}},	//����
	{{  -1.0f,  1.0f, -1.0f},{},{1.0f, 1.0f}},	//�E��
	{{  -1.0f,  1.0f,  1.0f},{},{1.0f, 0.0f}},	//�E��

	//�E
	{{   1.0f,  1.0f, -1.0f},{},{1.0f, 1.0f}},	//�E��
	{{   1.0f,  1.0f,  1.0f},{},{1.0f, 0.0f}},	//�E��
	{{   1.0f, -1.0f, -1.0f},{},{0.0f, 1.0f}},	//����
	{{   1.0f, -1.0f,  1.0f},{},{0.0f, 0.0f}},	//����


	//��
	{{ -1.0f,  -1.0f, -1.0f},{},{0.0f, 1.0f}},	//����
	{{  1.0f,  -1.0f, -1.0f},{},{0.0f, 0.0f}},	//����
	{{ -1.0f,  -1.0f,  1.0f},{},{1.0f, 1.0f}},	//�E��
	{{  1.0f,  -1.0f,  1.0f},{},{1.0f, 0.0f}},	//�E��

	//��
	{{ -1.0f,  1.0f,  1.0f},{},{1.0f, 1.0f}},	//�E��
	{{  1.0f,  1.0f,  1.0f},{},{1.0f, 0.0f}},	//�E��
	{{ -1.0f,  1.0f, -1.0f},{},{0.0f, 1.0f}},	//����
	{{  1.0f,  1.0f, -1.0f},{},{0.0f, 0.0f}},	//����

};
#pragma endregion


#pragma region IndexData
	unsigned short indices[BoxNumIndices] = {};

	//�C���f�b�N�X�ɒl������
	for (int i = 0; i < 6; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}
#pragma endregion

#pragma region Normal

	//�C���f�b�N�X���g���Ė@������������
	for (int i = 0; i < _countof(indices) / 3; i++)
	{

		//�O�p�`1���Ɍv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short i0 = indices[i * 3 + 0];
		unsigned short i1 = indices[i * 3 + 1];
		unsigned short i2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[i0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[i1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[i2].pos);

		//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z(�x�N�g���̌v�Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[i0].normal, normal);
		XMStoreFloat3(&vertices[i1].normal, normal);
		XMStoreFloat3(&vertices[i2].normal, normal);
	}


#pragma endregion
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex *vertMap = nullptr;
	HRESULT result = boxVertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));

	//�}�b�v������
	boxVertBuff->Unmap(0, nullptr);

	//GPU��̃o�b�t�@�ɑΉ��������z�������̎擾
	unsigned short *indexMap = nullptr;
	result = boxIndexBuff->Map(0, nullptr, (void **)&indexMap);

	memcpy(indexMap, indices, sizeof(indices));
	//�q���������
	boxIndexBuff->Unmap(0, nullptr);

}

void Object3DCommon::PlaneInit()
{
	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::GetInstance();

	Vertex vertices[4] = {};

	unsigned short indices[PlaneNumIndices] = {};
	//���_�o�b�t�@����
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&planeVertBuff)
	);

	//�C���f�b�N�X�o�b�t�@����
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�C���f�b�N�X��񂪓��镪�̃T�C�Y
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&planeIndexBuff));


	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
	PlaneTransferIndexBufferVertexBuffer();

	//���_�o�b�t�@�r���[�̍쐬
	planeVBView.BufferLocation = planeVertBuff->GetGPUVirtualAddress();
	planeVBView.SizeInBytes = sizeof(vertices);
	planeVBView.StrideInBytes = sizeof(vertices[0]);

	planeIBView.BufferLocation = planeIndexBuff->GetGPUVirtualAddress();
	planeIBView.Format = DXGI_FORMAT_R16_UINT;
	planeIBView.SizeInBytes = sizeof(indices);
}

void Object3DCommon::PlaneTransferIndexBufferVertexBuffer()
{
#pragma region VertexData
	Vertex vertices[] = {
		//�O
		{{ -1.0f, -1.0f,  0.0f},{},{0.0f, 1.0f}},	//����
		{{ -1.0f,  1.0f,  0.0f},{},{0.0f, 0.0f}},	//����
		{{  1.0f, -1.0f,  0.0f},{},{1.0f, 1.0f}},	//�E��
		{{  1.0f,  1.0f,  0.0f},{},{1.0f, 0.0f}},	//�E��
	};
#pragma endregion


#pragma region IndexData
	unsigned short indices[PlaneNumIndices] = 
	{
		0,1,2,
		2,1,3
	};

#pragma endregion

#pragma region Normal

	//�C���f�b�N�X���g���Ė@������������
	for (int i = 0; i < _countof(indices) / 3; i++)
	{

		//�O�p�`1���Ɍv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short i0 = indices[i * 3 + 0];
		unsigned short i1 = indices[i * 3 + 1];
		unsigned short i2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[i0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[i1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[i2].pos);

		//p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z(�x�N�g���̌v�Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[i0].normal, normal);
		XMStoreFloat3(&vertices[i1].normal, normal);
		XMStoreFloat3(&vertices[i2].normal, normal);
	}


#pragma endregion
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex *vertMap = nullptr;
	HRESULT result = planeVertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));

	//�}�b�v������
	planeVertBuff->Unmap(0, nullptr);

	//GPU��̃o�b�t�@�ɑΉ��������z�������̎擾
	unsigned short *indexMap = nullptr;
	result = planeIndexBuff->Map(0, nullptr, (void **)&indexMap);

	memcpy(indexMap, indices, sizeof(indices));
	//�q���������
	planeIndexBuff->Unmap(0, nullptr);
}
