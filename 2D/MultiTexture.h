#pragma once
#include <DirectXMath.h>
#include <IGraphicsPipeline.h>
#include <vector>

//複数のテクスチャを受け取って描画するClass(まだ改善できるだろう)
//マルチレンダーターゲットと合わさってるじゃねえか
class MultiTexture
	//public Sprite
{
public:
	/// <summary>
	///コンストラクタ
	/// </summary>
	MultiTexture();

	/// <summary>
	/// 描画コマンド
	/// </summary>
	void Draw(PipeClass::PipelineSet *pipelineSet);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

private:
	//座標
	DirectX::XMFLOAT2 position;
	//Z軸回りの回転軸
	float rotation;
	//色(RGBA)
	DirectX::XMFLOAT4 color;
	//ワールド行列
	DirectX::XMMATRIX matWorld;
	//大きさ
	DirectX::XMFLOAT2 size;
	//アンカーポイント
	DirectX::XMFLOAT2 anchorpoint;
	//左右反転
	bool isFlipX;
	//上下反転
	bool isFlipY;
	//テクスチャの左上座標
	DirectX::XMFLOAT2 texLeftTop;
	//テクスチャ切り出しサイズ
	DirectX::XMFLOAT2 texSize;
	//非表示
	bool isInvisible;

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
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;				//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;				//定数バッファ
	D3D12_VERTEX_BUFFER_VIEW vBView{};			//頂点バッファビュー
	ID3D12GraphicsCommandList *cmdList = nullptr;

	//テクスチャバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff;
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	//RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	//DSV用のデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	//画面クリアカラー
	static const float clearColor[4];

};

