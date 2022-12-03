#include "TitlePostEffect.h"
#include <vector>
#include "MyDirectX.h"
#include "../imgui/ImguiManager.h"

TitlePostEffect::TitlePostEffect()
{
	obj.Init();

	PipelineDesc desc;

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


	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;


	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ
	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//定数バッファビューとして初期化(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0,D3D12_SHADER_VISIBILITY_ALL);

	desc.VSname = L"Resources/shaders/TitleAnimationVS.hlsl";
	desc.PSname = L"Resources/shaders/TitleAnimationPS.hlsl";
	desc.inputLayout = inputLayout;
	desc.inputLayoutCount = _countof(inputLayout);
	desc.rootparams = rootparams;
	desc.rootparamsCount = _countof(rootparams);
	desc.blendName = GraphicsPipelineTypeName::BlendName::NONE;
	pipeline = BaseGraphicsPipeline::CreatePipeLine(desc);
}

TitlePostEffect::~TitlePostEffect()
{
}

void TitlePostEffect::Init()
{
HRESULT result;
MyDirectX *myD = MyDirectX::Instance();
	//定数バッファの生成
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(TitleBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff2_)
	);
	TitleBuffer *constMap = nullptr;
	result = constBuff2_->Map(0, nullptr, (void **)&constMap);
	constMap->t = 0.0f;//アニメーションの位置
	constMap->type = Titletype;
	constBuff2_->Unmap(0, nullptr);
	animationTimer.Init(300);
	startTimer_ = 60;
}

void TitlePostEffect::Update()
{
	bool AnimationReset = false;
	ImGui::Begin("Title");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	AnimationReset = ImGui::Button("AnimationStart");
	ImGui::RadioButton("Normal", &Titletype,0);
	ImGui::RadioButton("Up", &Titletype, 1);
	ImGui::RadioButton("Circle", &Titletype, 2);

	ImGui::End();

	HRESULT result;

	if (AnimationReset)
	{
		animationTimer.Reset();
	}
	if(startTimer_ <= 0)
	{
	animationTimer.Do(Easing::EaseMove::Out, Easing::Type::Expo);
	TitleBuffer *constMap = nullptr;
	result = constBuff2_->Map(0, nullptr, (void **)&constMap);
	constMap->t = animationTimer.Read();//アニメーションの位置
	constMap->type = Titletype;
	constBuff2_->Unmap(0, nullptr);
	}
	else
	{
		startTimer_--;
	}
}

void TitlePostEffect::Draw(int textureHandle)
{
	std::vector<int>textures;

	textures.emplace_back(textureHandle);

	obj.SetDrawBuffer(pipeline.get(), textures);
	MyDirectX::Instance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuff2_->GetGPUVirtualAddress());
	obj.DrawCommand();
}

void TitlePostEffect::Finalize()
{
}

void TitlePostEffect::Reset()
{
	animationTimer.Reset();
}
