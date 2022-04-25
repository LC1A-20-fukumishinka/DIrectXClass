#include "ModelPhongPipeline.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
using namespace PipeClass;
using namespace Microsoft::WRL;

ModelPhongPipeline::ModelPhongPipeline()
{
	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	pipelineSet = BaseGraphicsPipeline::CreatePipeLine(
		L"Resources/shaders/OBJVertexShader.hlsl",	// シェーダファイル名
		L"Resources/shaders/OBJPixelShader.hlsl",	// シェーダファイル名
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
