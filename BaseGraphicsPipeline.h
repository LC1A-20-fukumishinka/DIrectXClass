#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <memory>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
namespace PipeClass
{
	struct PipelineSet
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;	//ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;	//パイプラインステート
	};

	struct GSPipelineSet: public PipelineSet
	{
	};
}

namespace GraphicsPipelineTypeName
{
	enum class BlendName
	{
		ALPHA,
		ADD,
		Sub
	};
}

namespace FukuDefaultPipeline
{
	const D3D12_INPUT_ELEMENT_DESC kPostLayout[] =
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
}
struct PipelineDesc
{
	LPCWSTR VSname;
	LPCWSTR PSname;
	D3D12_INPUT_ELEMENT_DESC *inputLayout;
	size_t inputLayoutCount;
	CD3DX12_ROOT_PARAMETER *rootparams;
	size_t rootparamsCount;
	GraphicsPipelineTypeName::BlendName blendName = GraphicsPipelineTypeName::BlendName::ALPHA;
	bool isDepthWrite = true;
	int renderTargetCount = 1;
	D3D12_TEXTURE_ADDRESS_MODE loopMode = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
};
class BaseGraphicsPipeline
{
public:
	static std::unique_ptr<PipeClass::PipelineSet> CreatePipeLine(const PipelineDesc &pipelineDesc);

	static std::unique_ptr<PipeClass::GSPipelineSet> CreatePipeLine(const PipelineDesc &pipelineDesc, LPCWSTR GSname);

};

