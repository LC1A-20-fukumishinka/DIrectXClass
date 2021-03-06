#include "BaseGraphicsPipeline.h"
#include <d3dcompiler.h>
#include "MyDirectX.h"
using namespace PipeClass;
using namespace std;
using namespace GraphicsPipelineTypeName;
using namespace Microsoft::WRL;
std::unique_ptr<PipelineSet> BaseGraphicsPipeline::CreatePipeLine(LPCWSTR VSname, LPCWSTR PSname, D3D12_INPUT_ELEMENT_DESC *inputLayout, size_t inputLayoutCount, CD3DX12_ROOT_PARAMETER *rootparams, size_t rootparamsCount, BlendName blendName, int renderTargetCount)
{

	unique_ptr<PipelineSet> pipeSet = make_unique<PipelineSet>();
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	HRESULT result;
	ComPtr<ID3DBlob>vsBlob;//頂点シェーダオブジェクト
	ComPtr<ID3DBlob>psBlob;//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob>errorBlob;//エラーオブジェクト
	//頂点シェーダの読み込みとコンパイル
#pragma region VShader
	result = D3DCompileFromFile(
		VSname,//シェーダファイル名
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
		PSname,//シェーダファイル名
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
	//D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	//{
	//	{
	//		"POSITION",									//セマンティック
	//		0,											//同じセマンティック名が複数あるときに使うインデックス(0で良い)
	//		DXGI_FORMAT_R32G32B32_FLOAT,				//要素数とビット数を表す(XYZの3つでfloat型なので"R32G32B32_FLOAT")
	//		0,											//入力スロットインデックス(0でよい)
	//		D3D12_APPEND_ALIGNED_ELEMENT,				//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	//入力データ種別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
	//		0											//一度に描画するインスタンス数(0でよい)
	//	},//(1行で書いた方が見やすいらしい)
	//	//法線ベクトル
	//	{
	//		"NORMAL",
	//		0,
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	//		0
	//	},
	//	//座標以外に、色、テクスチャなどを渡す場合はさらに続ける
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
	//パイプラインステート設定変数の宣言と、各種項目の設定
#pragma region pipelineState
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定

	//標準的な設定(背面カリング、塗りつぶし、深度クリッピング有効)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc = {};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	switch (blendName)
	{
	case GraphicsPipelineTypeName::ALPHA:
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソース(描画する図形のピクセル)の値を100%使う
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デスト(描画対象ピクセル　　　)の値を  0%使う

		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - ソースのアルファ値
		break;
	case GraphicsPipelineTypeName::ADD:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;
	case GraphicsPipelineTypeName::Sub:
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//減算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;				//デストの値を100%使う

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
	//デプスステンシルステートの設定
#pragma region DepthStencilState

	//標準定期な設定(深度テストを行う、書き込み許可、深度が小さければ合格)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;								//深度値フォーマット
#pragma endregion

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = static_cast<UINT>(inputLayoutCount);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = count;//描画対象はNつ
	for (int i = 0; i < count; i++)
	{
		gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;//0^255指定のRGBA
	}
	gpipeline.SampleDesc.Count = 1;//ピクセルにつき1回サンプリング


#pragma endregion
	//ルートシグネチャの生成
#pragma region RootSignature
#pragma region rootParameter
////デスクリプタテーブルの設定
//	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
//
//	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ
//	//ルートパラメータの設定
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
	//バージョン自動判定でのシリアライズ
	result =
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//ルートシグネチャの生成
	result =
		device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeSet->rootSignature));
	gpipeline.pRootSignature = pipeSet->rootSignature.Get();

	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeSet->pipelineState));

#pragma endregion


	return pipeSet;
}

std::unique_ptr<PipeClass::GSPipelineSet> BaseGraphicsPipeline::CreatePipeLine(LPCWSTR VSname, LPCWSTR PSname, LPCWSTR GSname, D3D12_INPUT_ELEMENT_DESC *inputLayout, size_t inputLayoutCount, CD3DX12_ROOT_PARAMETER *rootparams, size_t rootparamsCount, GraphicsPipelineTypeName::BlendName blendName, bool isDepthWrite, int renderTargetCount)
{

	HRESULT result = S_FALSE;
	unique_ptr<GSPipelineSet> pipeSet = make_unique<GSPipelineSet>();
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();

	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;
	ComPtr<ID3DBlob> psBlob;// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		/*L"Resources/shaders/particleVS.hlsl"*/VSname,	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		/*L"Resources/shaders/particlePS.hlsl"*/PSname,	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//ジオメトリシェーダの代みっこみとコンパイル
	result = D3DCompileFromFile(
		/*L"Resources/shaders/particleGS.hlsl"*/GSname,//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	int count = renderTargetCount;

	if (count > 8)
	{
		count = 8;
	}
	if (count <= 0)
	{
		count = 1;
	}

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//深度バッファに書き込みを行うかどうか
	if (!isDepthWrite)
	{
		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;


	switch (blendName)
	{
	case GraphicsPipelineTypeName::ALPHA:
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソース(描画する図形のピクセル)の値を100%使う
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デスト(描画対象ピクセル　　　)の値を  0%使う

		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - ソースのアルファ値
		break;
	case GraphicsPipelineTypeName::ADD:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;
	case GraphicsPipelineTypeName::Sub:
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//減算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;				//デストの値を100%使う

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;
	default:
		break;
	}

	// ブレンドステートの設定
	for (int i = 0; i < count; i++)
	{
		gpipeline.BlendState.RenderTarget[i] = blenddesc;
	}

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = static_cast<UINT>(inputLayoutCount);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = count;	// 描画対象は1つ
	for (int i = 0; i < count; i++)
	{
		gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;//0^255指定のRGBA
	}
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(static_cast<UINT>(rootparamsCount), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeSet->rootSignature));
	if (FAILED(result)) {
	}

	gpipeline.pRootSignature = pipeSet->rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeSet->pipelineState));

	if (FAILED(result)) {
	}

	return pipeSet;
}
