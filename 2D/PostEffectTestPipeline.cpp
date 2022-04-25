#include "PostEffectTestPipeline.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
#include<cassert>

using namespace PipeClass;
using namespace Microsoft::WRL;

PostEffectTestPipeline::PostEffectTestPipeline()
{
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
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 レジスタ
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//定数バッファビューとして初期化(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
#pragma endregion

	pipelineSet =BaseGraphicsPipeline::CreatePipeLine(
		L"Resources/shaders/PostEffectTestVS.hlsl",
		L"Resources/shaders/PostEffectTestPS.hlsl",
		inputLayout,
		_countof(inputLayout),
		rootparams,
		_countof(rootparams));

}

PostEffectTestPipeline::~PostEffectTestPipeline()
{
}

PipeClass::PipelineSet *PostEffectTestPipeline::GetPipeLine()
{
	return pipelineSet.get();
}

PostEffectTestPipeline *PostEffectTestPipeline::Instance()
{
	static PostEffectTestPipeline instance;
	return &instance;
}