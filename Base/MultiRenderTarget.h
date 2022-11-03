#pragma once
#include <DirectXMath.h>
#include <IGraphicsPipeline.h>
#include <vector>
#include "Vector3.h"
class MultiRenderTarget
{
public:
	/// <summary>
	///コンストラクタ
	/// </summary>
	MultiRenderTarget();
	~MultiRenderTarget();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="targetCount">生成するターゲットの個数</param>
	void Init(int targetCount, Vector3 targetScale = Vector3(1280.0f, 720.0f, 0.0f));

	void PreDrawScene();

	void PostDrawScene();

	//テクスチャバッファの配列を渡す(使わなさそう)
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> GetTexBuff();

	//テクスチャマネージャ内のテクスチャ番号を返す
	int GetTextureNum(int renderTargetNum);

	//深度地リセット
	void DepthReset();

	void Finalize();
private:

	void MakeRTV();

	void MakeDepthBuffer();

	void MakeDSV();
private:
//レンダーターゲットの数
	enum class RenderTargetCount
	{
		count = 1,
	};
private:
	int targetCount_;

	ID3D12GraphicsCommandList *cmdList = nullptr;

	//テクスチャバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff;
	//テクスチャ番号
	std::vector<int> textureNums;

	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	//RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	//DSV用のデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	//画面クリアカラー
	static const float clearColor[4];

	Vector3 renderTargetScale_;
};

