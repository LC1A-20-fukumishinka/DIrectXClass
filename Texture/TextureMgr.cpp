#include "TextureMgr.h"
#include <cassert>
#include <DirectXTex.h>
#include "MyDirectX.h"
#include "BaseData.h"

TextureMgr::TextureMgr()
{
	MyDirectX *myDirectX = MyDirectX::Instance();

	//デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = TextureMaxCount;
	HRESULT result = myDirectX->GetDevice()->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&descHeap));
	loadTextureCount = 0;
}

TextureMgr::~TextureMgr()
{
}

TextureMgr *TextureMgr::Instance()
{
	static TextureMgr instance;
	return &instance;
}

int TextureMgr::SpriteLoadTexture(const wchar_t *filename)
{
	MyDirectX *myDirectX = MyDirectX::Instance();
	//異常な番号の指定を検出
	assert(loadTextureCount <= TextureMaxCount - 1);
	HRESULT result;

	//WICテクスチャのロード
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,	//「Resources」フォルダの「texuture.png」
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const DirectX::Image *img = scratchImg.GetImage(0, 0, 0);	//生データ抽出

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
#pragma region texBuff

	result = myDirectX->GetDevice()->CreateCommittedResource(	//GPUリソースの生成
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texBuff[loadTextureCount]));
#pragma endregion

#pragma region texDataSend
	result = texBuff[loadTextureCount]->WriteToSubresource(
		0,
		nullptr,							//全領域へコピー
		img->pixels,							//元データアドレス
		(UINT16)img->rowPitch,		//1ラインサイズ
		(UINT16)img->slicePitch	//全サイズ
	);

	//delete[] imageData;
#pragma endregion
#pragma region SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};		//設定構造体
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	myDirectX->GetDevice()->CreateShaderResourceView(
		texBuff[loadTextureCount].Get(),	//ビューと関連付けるバッファ
		&srvDesc,							//テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeap->GetCPUDescriptorHandleForHeapStart(), loadTextureCount,
			myDirectX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
#pragma endregion

	//読み込んだ画像の数を加算する
	loadTextureCount++;

	int textureNumber = loadTextureCount - 1;

	return textureNumber;
}

void TextureMgr::CreateRenderTarget(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &texBuff, std::vector<int> &textureNums, const int renderTargetCount, Vector3 renderTargetScale)
{
	HRESULT result;

	for (int i = 0; i < renderTargetCount; i++)
	{
		assert(loadTextureCount <= TextureMaxCount - 1);
#pragma region MakeTextureBuffer
		//テクスチャリソース設定
		CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			static_cast<UINT>(renderTargetScale.x),
			static_cast<UINT>(renderTargetScale.y),
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

		const float renderTargetClearColor[4] = { 0.0f,0.0f ,0.0f ,0.0f };
		//テクスチャバッファの生成
		result = MyDirectX::Instance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, renderTargetClearColor),
			IID_PPV_ARGS(&this->texBuff[loadTextureCount])
		);
		assert(SUCCEEDED(result));
#pragma endregion

#pragma region MakeDescHeap

		//SRV設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;


		//デスクリプタヒープにSRVを作成
		MyDirectX::Instance()->GetDevice()->CreateShaderResourceView(this->texBuff[loadTextureCount].Get(),
			&srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),
				loadTextureCount,
				MyDirectX::Instance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
#pragma endregion
		//引数のバッファに渡す
		texBuff.push_back(this->texBuff[loadTextureCount]);
		textureNums.push_back(loadTextureCount);

		//テクスチャカウントを一つ進める
		loadTextureCount++;
	}
}

ID3D12DescriptorHeap *TextureMgr::GetDescriptorHeap()
{
	return descHeap.Get();
}

ID3D12Resource *TextureMgr::GetTexBuff(int handle)
{
	return texBuff[handle].Get();
}

bool TextureMgr::CheckHandle(int handle)
{
	bool isCheck = false;
	if (handle < loadTextureCount && handle >= 0)
	{
		isCheck = true;
	}
	return isCheck;
}
