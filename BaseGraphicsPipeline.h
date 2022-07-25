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
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;	//���[�g�V�O�l�`��
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;	//�p�C�v���C���X�e�[�g
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
			"POSITION",									//�Z�}���e�B�b�N
			0,											//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X(0�ŗǂ�)
			DXGI_FORMAT_R32G32B32_FLOAT,				//�v�f���ƃr�b�g����\��(XYZ��3��float�^�Ȃ̂�"R32G32B32_FLOAT")
			0,											//���̓X���b�g�C���f�b�N�X(0�ł悢)
			D3D12_APPEND_ALIGNED_ELEMENT,				//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	//���̓f�[�^���(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
			0											//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
		},//(1�s�ŏ������������₷���炵��)
		//���W�ȊO�ɁA�F�A�e�N�X�`���Ȃǂ�n���ꍇ�͂���ɑ�����
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

