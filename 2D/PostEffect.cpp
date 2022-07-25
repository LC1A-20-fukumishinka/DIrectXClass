#include "PostEffect.h"
#include <d3dx12.h>
#include "MyDirectX.h"
#include "TextureMgr.h"
#include "SpriteCommon.h"
#include "BaseData.h"
#include "DirectInput.h"
using namespace DirectX;

PostEffect::PostEffect()
{
	cmdList = MyDirectX::Instance()->GetCommandList();
}

PostEffect::~PostEffect()
{
}

void PostEffect::Draw(PipeClass::PipelineSet *pipelineSet, const std::vector<int> &textureHandles)
{

	for (auto &textureHandle : textureHandles)
	{
		if (!TextureMgr::Instance()->CheckHandle(textureHandle))
		{
			assert(0);
			return;
		}
	}

	MyDirectX *myD = MyDirectX::Instance();
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();


	//パイプランステートの設定
	myD->GetCommandList()->SetPipelineState(pipelineSet->pipelineState.Get());
	//ルートシグネチャの設定
	myD->GetCommandList()->SetGraphicsRootSignature(pipelineSet->rootSignature.Get());
	//プリミティブ形状を設定
	myD->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//デスクリプタヒープの配列

	ID3D12DescriptorHeap *descHeap = TextureMgr::Instance()->GetDescriptorHeap();
	ID3D12DescriptorHeap *ppHeaps[] = { descHeap };
	myD->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);


	//頂点バッファをセット
	myD->GetCommandList()->IASetVertexBuffers(0, 1, &vBView_);

	//定数バッファをセット
	myD->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	//シェーダーリソースビューをセット

	for (auto &textureHandle : textureHandles)
	{
		int slotNum = static_cast<int>(&textureHandle - &textureHandles[0]) + 1;
		myD->GetCommandList()->SetGraphicsRootDescriptorTable(
			slotNum,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				descHeap->GetGPUDescriptorHandleForHeapStart(),
				textureHandle,
				myD->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	//描画コマンド
	myD->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::Init()
{
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
		IID_PPV_ARGS(&vertBuff_));
	//バッファへのデータ転送
	VertexPosUv *vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff_->Unmap(0, nullptr);
	//頂点バッファビューの作成
	vBView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vBView_.SizeInBytes = sizeof(vertices);
	vBView_.StrideInBytes = sizeof(vertices[0]);
	//定数バッファの生成
	result = myD->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);
	ConstBufferData *constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void **)&constMap);
	constMap->color = { 1, 1, 1, 1 };//色指定(RGBA)
	constMap->mat = XMMatrixIdentity();	//平行透視投影
	constBuff_->Unmap(0, nullptr);

}