#pragma once
#include <IGraphicsPipeline.h>
#include <vector>
#include <DirectXMath.h>
class PostEffect
	//public Sprite
{
public:
	/// <summary>
	///�R���X�g���N�^
	/// </summary>
	PostEffect();

	~PostEffect();
	/// <summary>
	/// �`��R�}���h
	/// </summary>
	void Draw(PipeClass::PipelineSet *pipelineSet, const std::vector<int> &textureHandles);

	/// <summary>
	/// ������
	/// </summary>
	void Init();

private:

	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//�F(RGBA)
		DirectX::XMMATRIX mat;	//3D�ϊ��s��
	};
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos;	//xyz���W
		DirectX::XMFLOAT2 uv;	//uv���W
	};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;				//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;				//�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vBView_{};			//���_�o�b�t�@�r���[
	ID3D12GraphicsCommandList *cmdList = nullptr;
};

