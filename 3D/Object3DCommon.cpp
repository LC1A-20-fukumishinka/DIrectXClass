#include "Object3DCommon.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>


using namespace DirectX;
Object3DCommon::Object3DCommon()
{
	pipelineSet = {};
	matProjection = DirectX::XMMatrixIdentity();
}

void Object3DCommon::Init()
{
	MyDirectX *myDirectX = MyDirectX::GetInstance();

	//������s��̐���
	matProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(60.0f),				//�㉺��p60�x
		(float)myDirectX->winWidth / (float)myDirectX->winHeight,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1000.0f							//�O�[�A���[
	);

	//�r���[�s��
	float angle = 0.0f;	//�J�����̉�]�p
	//DirectX::XMFLOAT3 eye(0, 0, -100);
	//DirectX::XMFLOAT3 target(0, 0, 0);
	//DirectX::XMFLOAT3 up(0, 1, 0);
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//�p�C�v���C���̐���
	Object3DCreateGraphPipeline(myDirectX->GetDevice());

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

void Object3DCommon::Object3DCreateGraphPipeline(ID3D12Device *dev)
{

	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob>vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob>psBlob;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob>errorBlob;//�G���[�I�u�W�F�N�g
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
#pragma region VShader
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main", "vs_5_0",//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&vsBlob, &errorBlob);
#pragma endregion
	//�s�N�Z���V�F�[�_�[�ǂݍ���
#pragma region PShader
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main", "ps_5_0",//�G���g���[�|�C���g���A�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result))
	{
		//errorBlob����G���[�̓��e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
	//���_���C�A�E�g�̐錾�Ɛݒ�
#pragma region inputLayOut
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",									//�Z�}���e�B�b�N
			0,											//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X(0�ŗǂ�)
			DXGI_FORMAT_R32G32B32_FLOAT,				//�v�f���ƃr�b�g����\��(XYZ��3��float�^�Ȃ̂�"R32G32B32_FLOAT")
			0,											//���̓X���b�g�C���f�b�N�X(0�ł悢)
			D3D12_APPEND_ALIGNED_ELEMENT,				//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	//���̓f�[�^���(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
			0											//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
		},//(1�s�ŏ������������₷���炵��)
		//�@���x�N�g��
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//���W�ȊO�ɁA�F�A�e�N�X�`���Ȃǂ�n���ꍇ�͂���ɑ�����
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};
#pragma endregion
	//�p�C�v���C���X�e�[�g�ݒ�ϐ��̐錾�ƁA�e�퍀�ڂ̐ݒ�
#pragma region pipelineState
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�

	//�W���I�Ȑݒ�(�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L��)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��

	D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];

	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X(�`�悷��}�`�̃s�N�Z��)�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g(�`��Ώۃs�N�Z���@�@�@)�̒l��  0%�g��

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - �\�[�X�̃A���t�@�l


	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
#pragma region DepthStencilState

	//�W������Ȑݒ�(�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;								//�[�x�l�t�H�[�}�b�g
#pragma endregion

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;//�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0^255�w���RGBA
	gpipeline.SampleDesc.Count = 1;//�s�N�Z���ɂ�1��T���v�����O
#pragma endregion
	//���[�g�V�O�l�`���̐���
#pragma region RootSignature
#pragma region rootParameter
//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;

	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
#pragma endregion

#pragma region textureSampler
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

#pragma endregion
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob>rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result =
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//���[�g�V�O�l�`���̐���
	result =
		dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSet.rootSignature));
	gpipeline.pRootSignature = pipelineSet.rootSignature.Get();

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));

#pragma endregion

}

void Object3DCommon::Object3DCommonBeginDraw()
{
	MyDirectX *myD = MyDirectX::GetInstance();
	//�p�C�v�����X�e�[�g�̐ݒ�
	myD->GetCommandList()->SetPipelineState(pipelineSet.pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet.rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
