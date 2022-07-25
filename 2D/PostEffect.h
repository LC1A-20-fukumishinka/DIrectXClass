#pragma once
#include <IGraphicsPipeline.h>
#include <vector>
#include <DirectXMath.h>
class PostEffect
	//public Sprite
{
public:
	/// <summary>
	///コンストラクタ
	/// </summary>
	PostEffect();

	~PostEffect();
	/// <summary>
	/// 描画コマンド
	/// </summary>
	void Draw(PipeClass::PipelineSet *pipelineSet, const std::vector<int> &textureHandles);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

private:

	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//色(RGBA)
		DirectX::XMMATRIX mat;	//3D変換行列
	};
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos;	//xyz座標
		DirectX::XMFLOAT2 uv;	//uv座標
	};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;				//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;				//定数バッファ
	D3D12_VERTEX_BUFFER_VIEW vBView_{};			//頂点バッファビュー
	ID3D12GraphicsCommandList *cmdList = nullptr;
};

