#include "PostEffectTestPipeline.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
#include<cassert>
using namespace PipeClass;
using namespace Microsoft::WRL;

PostEffectTestPipeline::PostEffectTestPipeline()
{

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

		//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
#pragma region VShader
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestVS.hlsl",//�V�F�[�_�t�@�C����
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
		L"Resources/shaders/PostEffectTestPS.hlsl",//�V�F�[�_�t�@�C����
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
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
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
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
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 ���W�X�^
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@�r���[�Ƃ��ď�����(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
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

	assert(SUCCEEDED(result));
	//���[�g�V�O�l�`���̐���
	result =
		device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSet.rootSignature));
	gpipeline.pRootSignature = pipelineSet.rootSignature.Get();

	assert(SUCCEEDED(result));

	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));

	assert(SUCCEEDED(result));

#pragma endregion
	if (FAILED(result)) {

	}

}

PostEffectTestPipeline::~PostEffectTestPipeline()
{
}

const PipeClass::PipelineSet &PostEffectTestPipeline::GetPipeLine()
{
	return pipelineSet;
}

PostEffectTestPipeline *PostEffectTestPipeline::Instance()
{
	static PostEffectTestPipeline instance;
	return &instance;
}