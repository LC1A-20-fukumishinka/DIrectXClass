#include "PostBasePipeline.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
#include<cassert>
using namespace PipeClass;
using namespace Microsoft::WRL;

PostBasePipeline::PostBasePipeline()
{
	//頂点レイアウトの宣言と設定
#pragma region inputLayOut
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",									//セマンティック
			0,											//同じセマンティック名が複数あるときに使うインデックス(0で良い)
			DXGI_FORMAT_R32G32B32_FLOAT,				//要素数とビット数を表す(XYZの3つでfloat型なので"R32G32B32_FLOAT")
			0,											//入力スロットインデックス(0でよい)
			D3D12_APPEND_ALIGNED_ELEMENT,				//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	//入力データ種別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
			0											//一度に描画するインスタンス数(0でよい)
		},//(1行で書いた方が見やすいらしい)
		//座標以外に、色、テクスチャなどを渡す場合はさらに続ける
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
	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;


	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//定数バッファビューとして初期化(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
#pragma endregion

	PipelineDesc pd;
	pd.VSname = L"Resources/shaders/BaseDrawVS.hlsl";
	pd.PSname = L"Resources/shaders/BaseDrawPS.hlsl";
	pd.inputLayout = inputLayout;
	pd.inputLayoutCount = _countof(inputLayout);
	pd.rootparams = rootparams;
	pd.rootparamsCount = _countof(rootparams);
	pd.blendName = GraphicsPipelineTypeName::BlendName::NONE;
	pipelineSet = BaseGraphicsPipeline::CreatePipeLine(pd);
}

PostBasePipeline::~PostBasePipeline()
{
}

PipeClass::PipelineSet *PostBasePipeline::GetPipeLine()
{
	return pipelineSet.get();
}