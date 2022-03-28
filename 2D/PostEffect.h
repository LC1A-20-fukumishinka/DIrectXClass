#pragma once
#include "Sprite.h"
class PostEffect :
	public Sprite
{
public:
	/// <summary>
	///コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 描画コマンド
	/// </summary>
	void Draw();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(UINT texnumber);
private:

	//テクスチャバッファの生成
	void MakeTextureBuffer();

	//イメージデータの転送
	void SendImage();

	void MakeDescHeap();
private:
	ID3D12CommandList *cmdList = nullptr;

	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

