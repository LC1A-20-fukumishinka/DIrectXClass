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
}

namespace GraphicsPipelineTypeName
{
	enum BlendName
	{
		ALPHA,
		ADD,
		Sub
	};
}
class BaseGraphicsPipeline
{
public:
	static std::unique_ptr<PipeClass::PipelineSet> CreatePipeLine(LPCWSTR VSname, LPCWSTR PSname, D3D12_INPUT_ELEMENT_DESC *inputLayout, size_t inputLayoutCount, CD3DX12_ROOT_PARAMETER *rootparams, size_t rootparamsCount,GraphicsPipelineTypeName::BlendName blendName = GraphicsPipelineTypeName::ALPHA,  int renderTargetCount = 1);
};

