#include "ImguiManager.h"
#include "WinAPI.h"


Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ImguiManager::heapForImgui_;
ID3D12GraphicsCommandList *ImguiManager::cmdList_ = nullptr;
void ImguiManager::Init(ID3D12Device *device ,ID3D12GraphicsCommandList *cmdList)
{

	cmdList_ = cmdList;

	heapForImgui_ = CreateDescriptorHeapForImgui(device);

	if (heapForImgui_ == nullptr)
	{
		assert(0);
	}

	if (ImGui::CreateContext() == nullptr)
	{
		assert(0);
	}

	bool bInResult = ImGui_ImplWin32_Init(WinAPI::Instance()->hwnd);
	if (!bInResult)
	{
		assert(0);
	}

	bInResult = ImGui_ImplDX12_Init(
		device,
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		heapForImgui_.Get(),
		heapForImgui_->GetCPUDescriptorHandleForHeapStart(),
		heapForImgui_->GetGPUDescriptorHandleForHeapStart());
}


Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ImguiManager::CreateDescriptorHeapForImgui(ID3D12Device *device)
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ret;
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	device->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));
	return ret;
}

void ImguiManager::PreDraw()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::MakeWindow()
{
	ImGui::Begin("Test");
	ImGui::SetWindowSize(ImVec2(1000, 1000), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::End();
}

void ImguiManager::Draw()
{
	ImGui::Render();
	cmdList_->SetDescriptorHeaps(
		1, heapForImgui_.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList_);
}

void ImguiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	heapForImgui_.Reset();
}
