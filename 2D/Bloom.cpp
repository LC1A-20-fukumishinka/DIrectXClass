#include "Bloom.h"
#include <vector>
#include "MyDirectX.h"
//#include "../imgui/ImguiManager.h"
Bloom::Bloom()
{
	float tmp = 10.0f;

	bloomTargets.resize(5);
	for (auto &e : bloomTargets)
	{
		float count = static_cast<float>(&e - &bloomTargets[0]);
		count += 1.0f;
		e.Init(1, Vector3((1280.0f / powf(2.0f, count)), (720.0f / powf(2.0f, count)), 0.0f));
	}
	//描画用オブジェクトを初期化
	brightnessExtractionObj.Init();

	bloomObj.Init();
	//パイプライン初期化構造体を宣言
	PipelineDesc desc;

	//input 構造体を作る
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

	//デスクリプタテーブルの設定メインとブルーム
	CD3DX12_DESCRIPTOR_RANGE  mainTextureDescRangeSRV;
	CD3DX12_DESCRIPTOR_RANGE  brightDescRangeSRV;

	mainTextureDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ
	brightDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 レジスタ

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &mainTextureDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL); //メインの描画テクスチャ
	rootparams[2].InitAsDescriptorTable(1, &brightDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL); //ブルームの描画テクスチャ


	desc.VSname = L"Resources/shaders/AddColorVS.hlsl";
	desc.PSname = L"Resources/shaders/AddColorPS.hlsl";
	desc.inputLayout = inputLayout;
	desc.inputLayoutCount = _countof(inputLayout);
	desc.rootparams = rootparams;
	desc.rootparamsCount = _countof(rootparams);
	desc.blendName = GraphicsPipelineTypeName::BlendName::NONE;
	bloomPipeline = BaseGraphicsPipeline::CreatePipeLine(desc);

	PipelineDesc GaussianBlurDesc;


	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE  bloomBaseDescRangeSRV;
	bloomBaseDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ
	CD3DX12_DESCRIPTOR_RANGE  bloomBrightDescRangeSRV;
	bloomBrightDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 レジスタ

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER bloomRootparams[2] = {};
	bloomRootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	bloomRootparams[1].InitAsDescriptorTable(1, &bloomBaseDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL); //通常の描画


	GaussianBlurDesc.VSname = L"Resources/shaders/GaussianblurVS.hlsl";
	GaussianBlurDesc.PSname = L"Resources/shaders/GaussianblurPS.hlsl";
	GaussianBlurDesc.inputLayout = inputLayout;
	GaussianBlurDesc.inputLayoutCount = _countof(inputLayout);
	GaussianBlurDesc.rootparams = bloomRootparams;
	GaussianBlurDesc.rootparamsCount = _countof(bloomRootparams);
	GaussianBlurDesc.blendName = GraphicsPipelineTypeName::BlendName::NONE;
	GaussianBlurDesc.loopMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	GaussianBlurPipeline = BaseGraphicsPipeline::CreatePipeLine(GaussianBlurDesc);
}

Bloom::~Bloom()
{
}

void Bloom::Init()
{
}

void Bloom::Update()
{
}

void Bloom::BrightUpdate(int brightTextureHandle)
{
	int drawTexture = brightTextureHandle;

	for (auto &e : bloomTargets)
	{
		std::vector<int>textures;

		textures.emplace_back(drawTexture);

		e.PreDrawScene();
		brightnessExtractionObj.SetDrawBuffer(GaussianBlurPipeline.get(), textures);
		brightnessExtractionObj.DrawCommand();
		e.PostDrawScene();
		drawTexture = e.GetTextureNum(0);
	}
}

void Bloom::Draw(int mainTextureHandle)
{
	{
		std::vector<int>textures;
		textures.emplace_back(mainTextureHandle);
		textures.emplace_back(bloomTargets[4].GetTextureNum(0));
		bloomObj.Draw(bloomPipeline.get(), textures);
	}
}

void Bloom::Finalize()
{
}
