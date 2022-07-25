#include "GraphicsPipeline3D.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
using namespace PipeClass;


GraphicsPipeline3D::GraphicsPipeline3D()
{
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
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
#pragma region rootParameter
//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;

	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
#pragma endregion

	PipelineDesc pipelineDesc;
	pipelineDesc.VSname = L"Resources/shaders/BasicVS.hlsl";
	pipelineDesc.PSname = L"Resources/shaders/BasicPS.hlsl";
	pipelineDesc.inputLayout = inputLayout;
	pipelineDesc.inputLayoutCount = _countof(inputLayout);
	pipelineDesc.rootparams = rootparams;
	pipelineDesc.rootparamsCount = _countof(rootparams);
	pipelineSet = BaseGraphicsPipeline::CreatePipeLine(pipelineDesc);
}
GraphicsPipeline3D::~GraphicsPipeline3D()
{
}

PipelineSet *GraphicsPipeline3D::GetPipeLine()
{
	return pipelineSet.get();
}

GraphicsPipeline3D *GraphicsPipeline3D::Instance()
{
	static GraphicsPipeline3D instance;
	return &instance;
}

