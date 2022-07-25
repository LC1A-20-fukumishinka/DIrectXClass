#include "GraphicsPipeline3D.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
using namespace PipeClass;


GraphicsPipeline3D::GraphicsPipeline3D()
{
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob>vsBlob;//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob>psBlob;//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob>errorBlob;//エラーオブジェクト
	//頂点シェーダの読み込みとコンパイル
#pragma region VShader
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "vs_5_0",//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用指定
		0,
		&vsBlob, &errorBlob);
#pragma endregion
	//ピクセルシェーダー読み込み
#pragma region PShader
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "ps_5_0",//エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result))
	{
		//errorBlobからエラーの内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
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
		//法線ベクトル
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
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
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;

	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ
	//ルートパラメータの設定
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

