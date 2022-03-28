#include "PostEffect.h"
#include <d3dx12.h>
#include "MyDirectX.h"
#include "TextureMgr.h"
#include "SpriteCommon.h"
#include "BaseData.h"
PostEffect::PostEffect()
{
	cmdList = MyDirectX::Instance()->GetCommandList();
}

void PostEffect::Draw()
{
	//描画フラグがtrueじゃなかったら早期リターン
	if (isInvisible) return;

	if (!TextureMgr::Instance()->CheckHandle(texNumber))
	{
		assert(0);
		return;
	}

	MyDirectX *myD = MyDirectX::Instance();
	//パイプランステートの設定
	myD->GetCommandList()->SetPipelineState(SpriteCommon::Instance()->pipelineSet.pipelineState.Get());
	//ルートシグネチャの設定
	myD->GetCommandList()->SetGraphicsRootSignature(SpriteCommon::Instance()->pipelineSet.rootSignature.Get());
	//プリミティブ形状を設定
	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { descHeapSRV.Get() };
	myD->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);


	//頂点バッファをセット
	myD->GetCommandList()->IASetVertexBuffers(0, 1, &vBView);

	//定数バッファをセット
	myD->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//シェーダーリソースビューをセット
	myD->GetCommandList()->SetGraphicsRootDescriptorTable(1,descHeapSRV->GetGPUDescriptorHandleForHeapStart());
	//描画コマンド
	myD->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::Init(UINT texnumber)
{

	//基底クラスとしての初期化
	Sprite::Init(texnumber);

	MakeTextureBuffer();
	SendImage();
	MakeDescHeap();
}

void PostEffect::MakeTextureBuffer()
{
	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	DXGI_FORMAT_R8G8B8A8_UNORM,
	WINDOW_WIDTH,
	(UINT)WINDOW_HEIGHT,
	1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	result = MyDirectX::Instance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));
}

void PostEffect::SendImage()
{	//テクスチャを赤クリア

	HRESULT result;

	//画素数(1280 * 720 = 921600)ピクセル
	const UINT pixelCount = WINDOW_WIDTH * WINDOW_HEIGHT;
	//画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * WINDOW_WIDTH;
	//画像全体のデータサイズ
	const UINT depthPitch = rowPitch * WINDOW_HEIGHT;
	//画像イメージ
	UINT * img = new UINT[pixelCount];
	for(int i = 0;i< pixelCount; i++){img[i] = 0xff0000ff; };

	//テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(0, nullptr, 
	img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));
	delete[] img;
}

void PostEffect::MakeDescHeap()
{
	HRESULT result;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープ
	device->CreateShaderResourceView(texBuff.Get(), 
	&srvDesc,
	descHeapSRV->GetCPUDescriptorHandleForHeapStart());
}
