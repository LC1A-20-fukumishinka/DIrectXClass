#include "ModelPhongPipeline.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
using namespace PipeClass;
using namespace Microsoft::WRL;

ModelPhongPipeline::ModelPhongPipeline()
{
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	pipelineSet = BaseGraphicsPipeline::CreatePipeLine(
		L"Resources/shaders/OBJVertexShader.hlsl",	// �V�F�[�_�t�@�C����
		L"Resources/shaders/OBJPixelShader.hlsl",	// �V�F�[�_�t�@�C����
		inputLayout,
		_countof(inputLayout),
		rootparams,
		_countof(rootparams));
}

ModelPhongPipeline::~ModelPhongPipeline()
{
}

PipeClass::PipelineSet *ModelPhongPipeline::GetPipeLine()
{
	return pipelineSet.get();
}

ModelPhongPipeline *ModelPhongPipeline::Instance()
{
	static ModelPhongPipeline instance;
	return &instance;
}
