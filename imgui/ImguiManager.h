#pragma once
#include "MyDirectX.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include <wrl.h>

class ImguiManager
{
public:
	static void Init(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList);
	static void PreDraw();
	static void Draw();

private:
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui(ID3D12Device *device);



	static void MakeWindow();

	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapForImgui_;
	static ID3D12GraphicsCommandList *cmdList_;
};

