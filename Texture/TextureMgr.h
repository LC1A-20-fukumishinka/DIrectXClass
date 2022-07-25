#pragma once
#include "MyDirectX.h"
#include <DirectXMath.h>
class TextureMgr
{
private:
	enum Texture
	{
		TextureMaxCount = 256
	};
	TextureMgr();
	~TextureMgr();
public:
	TextureMgr(const TextureMgr &obj) = delete;

	TextureMgr operator=(const TextureMgr &obj) = delete;

	static TextureMgr *Instance();

	int SpriteLoadTexture(const wchar_t *filename);

	/// <summary>
	/// レンダーターゲット用のテクスチャバッファの生成を行う
	/// </summary>
	/// <param name="texBuff">描画用バッファ</param>
	/// <param name="textureNums">各テクスチャ番号</param>
	void CreateRenderTarget(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &texBuff, std::vector<int> &textureNums, const int renderTargetCount);

	ID3D12DescriptorHeap *GetDescriptorHeap();

	ID3D12Resource *GetTexBuff(int handle);

	bool CheckHandle(int handle);
private:
	//テクスチャ用デスクリプタヒープの生成
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descHeap;
	//テクスチャリソース(テクスチャバッファ)の配列
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[TextureMaxCount];

	int loadTextureCount;
};