#include "ShadowDrawPipeline.h"

ShadowDrawPipeline::ShadowDrawPipeline()
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
	//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[5];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);

	PipelineDesc pipelineDesc;
	pipelineDesc.VSname = L"Resources/shaders/OBJSHadowDrawVS.hlsl";
	pipelineDesc.PSname = L"Resources/shaders/OBJSHadowDrawPS.hlsl";
	pipelineDesc.inputLayout = inputLayout;
	pipelineDesc.inputLayoutCount = _countof(inputLayout);
	pipelineDesc.rootparams = rootparams;
	pipelineDesc.rootparamsCount = _countof(rootparams);

	pipelineSet = BaseGraphicsPipeline::CreatePipeLine(pipelineDesc);
}

ShadowDrawPipeline::~ShadowDrawPipeline()
{
}

PipeClass::PipelineSet *ShadowDrawPipeline::GetPipeLine()
{
	return pipelineSet.get();
}

ShadowDrawPipeline *ShadowDrawPipeline::Instance()
{
	static ShadowDrawPipeline instance;
	return &instance;
}
