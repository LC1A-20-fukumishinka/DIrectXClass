#include "MultiTexture.h"
#include <d3dx12.h>
#include "MyDirectX.h"
#include "TextureMgr.h"
#include "SpriteCommon.h"
#include "BaseData.h"
#include "DirectInput.h"
using namespace DirectX;
const float MultiTexture::clearColor[4] = { 0.2f,0.2f ,0.2f ,0.0f };

MultiTexture::MultiTexture()
{
	cmdList = MyDirectX::Instance()->GetCommandList();
}

void MultiTexture::Draw(PipeClass::PipelineSet *pipelineSet)
{
	//描画フラグがtrueじゃなかったら早期リターン
	if (isInvisible) return;

	//if (!TextureMgr::Instance()->CheckHandle(texNumber))
	//{
	//	assert(0);
	//	return;
	//}
	MyDirectX *myD = MyDirectX::Instance();
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();

	//if (Input::Instance()->KeyTrigger(DIK_0))
	//{
	//	static int tex = 0;
	//	tex = (tex + 1) % 2;

	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//	srvDesc.Texture2D.MipLevels = 1;
	//	device->CreateShaderResourceView(texBuff[tex].Get(),
	//		&srvDesc,
	//		descHeapSRV->GetCPUDescriptorHandleForHeapStart());
	//}


	//パイプランステートの設定
	myD->GetCommandList()->SetPipelineState(pipelineSet->pipelineState.Get());
	//ルートシグネチャの設定
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet->rootSignature.Get());
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
	myD->GetCommandList()->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(),
			0,
			myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	myD->GetCommandList()->SetGraphicsRootDescriptorTable(
		2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(),
			1,
			myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//描画コマンド
	myD->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}

void MultiTexture::Init()
{

	//基底クラスとしての初期化
	//Sprite::Init(texnumber);

	anchorpoint = { 0.5f, 0.5f };
	isFlipX = false;
	isFlipY = false;
	position = { 0, 0 };
	color = { 1, 1, 1, 1 };
	rotation = 0.0f;

	size = { 100, 100 };
	texLeftTop = { 0, 0 };
	texSize = { 0 , 0 };
	isInvisible = false;


	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::Instance();


	//頂点データ
	VertexPosUv vertices[] = {
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},//左下
		{{-1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},//左上
		{{+1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},//右下
		{{+1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},//右上
	};

	//頂点バッファの生成
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	//バッファへのデータ転送
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
	//頂点バッファビューの作成
	vBView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vBView.SizeInBytes = sizeof(vertices);
	vBView.StrideInBytes = sizeof(vertices[0]);
	//定数バッファの生成
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = color;//色指定(RGBA)
	constMap->mat = XMMatrixIdentity();	//平行透視投影
	constBuff->Unmap(0, nullptr);

	MakeTextureBuffer();
	SendImage();
	MakeDescHeap();
	MakeRTV();
	MakeDepthBuffer();
	MakeDSV();
}

void MultiTexture::PreDrawScene()
{
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//リソースバリアを変更(シェーダーリソース→描画可能)
	for (int i = 0; i < texBuff.size(); i++)
	{
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}
	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];
	for (int i = 0; i < 2; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(),
			i
			, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);


	CD3DX12_VIEWPORT viewports[2];
	CD3DX12_RECT scissorRects[2];
	for (int i = 0; i < 2; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
		scissorRects[i] = CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	//ビューポートの設定
	cmdList->RSSetViewports(2, viewports);
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(2, scissorRects);

	//全画面クリア
	for (int i = 0; i < 2; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MultiTexture::PostDrawScene()
{
	//リソースバリアを変更（描画可能->シェーダーリソース）
	for (int i = 0; i < 2; i++)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}

void MultiTexture::MakeTextureBuffer()
{
	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WINDOW_WIDTH,
		(UINT)WINDOW_HEIGHT,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	for (int i = 0; i < texBuff.size(); i++)
	{
		//テクスチャバッファの生成
		result = MyDirectX::Instance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
			IID_PPV_ARGS(&texBuff[i])
		);
		assert(SUCCEEDED(result));
	}
}

void MultiTexture::SendImage()
{	//テクスチャを赤クリア

	HRESULT result;

	//画素数(1280 * 720 = 921600)ピクセル
	const UINT pixelCount = WINDOW_WIDTH * WINDOW_HEIGHT;
	//画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * WINDOW_WIDTH;
	//画像全体のデータサイズ
	const UINT depthPitch = rowPitch * WINDOW_HEIGHT;
	//画像イメージ
	UINT *img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; };

	//テクスチャバッファにデータ転送
	for (int i = 0; i < texBuff.size(); i++)
	{
		result = texBuff[i]->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
	}
	delete[] img;
}

void MultiTexture::MakeDescHeap()
{
	HRESULT result;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;//個数
	//SRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < 2; i++)
	{
		//デスクリプタヒープにSRVを作成
		device->CreateShaderResourceView(texBuff[i].Get(),
			&srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV->GetCPUDescriptorHandleForHeapStart(),
				i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
}
void MultiTexture::MakeRTV()
{
	HRESULT result S_FALSE;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;
	//RTV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//デスクリプタヒープにRTV作成
	for (int i = 0; i < texBuff.size(); i++)
	{
		device->CreateRenderTargetView(texBuff[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(),
				i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
			));
	}
}

void MultiTexture::MakeDepthBuffer()
{
	HRESULT result S_FALSE;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	//深度バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));
}

void MultiTexture::MakeDSV()
{
	HRESULT result S_FALSE;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//DSV用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}
