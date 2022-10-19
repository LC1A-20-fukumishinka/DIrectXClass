#include "MultiRenderTarget.h"
#include <d3dx12.h>
#include "MyDirectX.h"
#include "TextureMgr.h"
#include "BaseData.h"
#include "DirectInput.h"
#include <algorithm>
using namespace DirectX;
const float MultiRenderTarget::clearColor[4] = { 0.0f,0.0f ,0.0f ,0.0f };

MultiRenderTarget::MultiRenderTarget()
{
	cmdList = MyDirectX::Instance()->GetCommandList();
}

MultiRenderTarget::~MultiRenderTarget()
{
}

void MultiRenderTarget::Init(int targetCount)
{
	targetCount_ = targetCount;
	HRESULT result = S_FALSE;
	MyDirectX *myD = MyDirectX::Instance();

	//�e�N�X�`���}�l�[�W�����Ƀ����_�[�^�[�Q�b�g�p�̃e�N�X�`���o�b�t�@�𐶐�
	TextureMgr::Instance()->CreateRenderTarget(texBuff, textureNums, targetCount_);
	MakeRTV();
	MakeDepthBuffer();
	MakeDSV();
}

void MultiRenderTarget::PreDrawScene()
{
	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X���`��\)
	for (int i = 0; i < texBuff.size(); i++)
	{
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}
	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHs;
	rtvHs.reserve(5);
	rtvHs.resize(targetCount_);
	for (auto &e : rtvHs)
	{
		int index = static_cast<int>(&e - &rtvHs[0]);
		e = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(),
			index,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(targetCount_, rtvHs.data(), false, &dsvH);


	std::vector<CD3DX12_VIEWPORT> viewports;
	std::vector<CD3DX12_RECT> scissorRects;

	viewports.reserve(5);
	viewports.resize(targetCount_);
	scissorRects.reserve(5);
	scissorRects.resize(targetCount_);
	for (auto &e : viewports)
	{
		e = CD3DX12_VIEWPORT(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	for (auto &e : scissorRects)
	{
		e = CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}


	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(2, viewports.data());
	//�V�U�����O��`�̐ݒ�
	cmdList->RSSetScissorRects(2, scissorRects.data());

	//�S��ʃN���A
	for (auto &e : rtvHs)
	{
		cmdList->ClearRenderTargetView(e, clearColor, 0, nullptr);
	}
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MultiRenderTarget::PostDrawScene()
{
	//���\�[�X�o���A��ύX�i�`��\->�V�F�[�_�[���\�[�X�j
	for (auto &e : texBuff)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(e.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}

std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> MultiRenderTarget::GetTexBuff()
{
	return texBuff;
}

int MultiRenderTarget::GetTextureNum(int renderTargetNum)
{
	int num = std::clamp(renderTargetNum, 0, static_cast<int>(textureNums.size()) - 1);
	return textureNums[num];
}

void MultiRenderTarget::DepthReset()
{
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MultiRenderTarget::Finalize()
{
}


void MultiRenderTarget::MakeRTV()
{
	HRESULT result S_FALSE;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = static_cast<int>(RenderTargetCount::count);
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//�f�X�N���v�^�q�[�v��RTV�쐬
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

void MultiRenderTarget::MakeDepthBuffer()
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

	//�[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));
}

void MultiRenderTarget::MakeDSV()
{
	HRESULT result S_FALSE;

	ID3D12Device *device = MyDirectX::Instance()->GetDevice();
	//DSV�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV�p�f�X�N���v�^�q�[�v���쐬
	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}
