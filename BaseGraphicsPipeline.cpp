#include "BaseGraphicsPipeline.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
using namespace PipeClass;
using namespace std;
using namespace GraphicsPipelineTypeName;
std::unique_ptr<PipelineSet> BaseGraphicsPipeline::CreatePipeLine(LPCWSTR VSname, LPCWSTR PSname, D3D12_INPUT_ELEMENT_DESC *inputLayout, size_t inputLayoutCount, CD3DX12_ROOT_PARAMETER *rootparams, size_t rootparamsCount, BlendName blendName, int renderTargetCount)
{

	unique_ptr<PipelineSet> pipeSet = make_unique<PipelineSet>();
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob>vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob>psBlob;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob>errorBlob;//�G���[�I�u�W�F�N�g
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
#pragma region VShader
	result = D3DCompileFromFile(
		VSname,//�V�F�[�_�t�@�C����
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
		PSname,//�V�F�[�_�t�@�C����
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
	//D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	//{
	//	{
	//		"POSITION",									//�Z�}���e�B�b�N
	//		0,											//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X(0�ŗǂ�)
	//		DXGI_FORMAT_R32G32B32_FLOAT,				//�v�f���ƃr�b�g����\��(XYZ��3��float�^�Ȃ̂�"R32G32B32_FLOAT")
	//		0,											//���̓X���b�g�C���f�b�N�X(0�ł悢)
	//		D3D12_APPEND_ALIGNED_ELEMENT,				//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	//���̓f�[�^���(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
	//		0											//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
	//	},//(1�s�ŏ������������₷���炵��)
	//	//�@���x�N�g��
	//	{
	//		"NORMAL",
	//		0,
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	//		0
	//	},
	//	//���W�ȊO�ɁA�F�A�e�N�X�`���Ȃǂ�n���ꍇ�͂���ɑ�����
	//	{
	//		"TEXCOORD",
	//		0,
	//		DXGI_FORMAT_R32G32_FLOAT,
	//		0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	//		0
	//	},
	//};
#pragma endregion

	int count = renderTargetCount;

	if (count > 8)
	{
		count = 8;
	}
	if (count <= 0)
	{
		count = 1;
	}
	//�p�C�v���C���X�e�[�g�ݒ�ϐ��̐錾�ƁA�e�퍀�ڂ̐ݒ�
#pragma region pipelineState
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�

	//�W���I�Ȑݒ�(�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L��)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc = {};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	switch (blendName)
	{
	case GraphicsPipelineTypeName::ALPHA:
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X(�`�悷��}�`�̃s�N�Z��)�̒l��100%�g��
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g(�`��Ώۃs�N�Z���@�@�@)�̒l��  0%�g��

		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - �\�[�X�̃A���t�@�l
		break;
	case GraphicsPipelineTypeName::ADD:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;
	case GraphicsPipelineTypeName::Sub:
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//���Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;				//�f�X�g�̒l��100%�g��

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;
	default:
		break;
	}


	for (int i = 0; i < count; i++)
	{
		gpipeline.BlendState.RenderTarget[i] = blenddesc;
	}
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
#pragma region DepthStencilState

	//�W������Ȑݒ�(�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;								//�[�x�l�t�H�[�}�b�g
#pragma endregion

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = static_cast<UINT>(inputLayoutCount);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = count;//�`��Ώۂ�N��
	for (int i = 0; i < count; i++)
	{
		gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;//0^255�w���RGBA
	}
	gpipeline.SampleDesc.Count = 1;//�s�N�Z���ɂ�1��T���v�����O


#pragma endregion
	//���[�g�V�O�l�`���̐���
#pragma region RootSignature
#pragma region rootParameter
////�f�X�N���v�^�e�[�u���̐ݒ�
//	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
//
//	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
//	//���[�g�p�����[�^�̐ݒ�
//	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
//	rootparams[0].InitAsConstantBufferView(0);
//	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
#pragma endregion

#pragma region textureSampler
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

#pragma endregion
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(static_cast<UINT>(rootparamsCount), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob>rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result =
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//���[�g�V�O�l�`���̐���
	result =
		device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeSet->rootSignature));
	gpipeline.pRootSignature = pipeSet->rootSignature.Get();

	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeSet->pipelineState));

#pragma endregion


	return pipeSet;
}
