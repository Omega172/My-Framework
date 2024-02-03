#pragma once
#include "pch.h"

// This code was stolen from https://github.com/Sh0ckFR/Universal-Dear-ImGui-Hook and modified to work with my framework and to be more readable

// These comments were generated by GitHub Copilot
#if FRAMEWORK_RENDER_D3D12 // If the rendering API is D3D12 include the D3D12 headers and define the D3D12 hooks

#include <DXGI.h>
#include <D3D12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")

#include "../../ImGUI/Styles.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Function pointer types for the original functions
typedef long(__fastcall* PresentD3D12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__fastcall* DrawInstancedD3D12)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef void(__fastcall* DrawIndexedInstancedD3D12)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);

void(*oExecuteCommandLists)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);

// Original function pointers
PresentD3D12 oPresent;
DrawInstancedD3D12 oDrawInstanced;
DrawIndexedInstancedD3D12 oDrawIndexedInstanced;

// D3D12 device and descriptor heap variables
ID3D12Device* d3d12Device = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12CommandQueue* d3d12CommandQueue = nullptr;

// Structure to hold per-frame context
struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};

// Other variables
uint64_t buffersCounts = -1;
FrameContext* frameContext;
HWND Window;
WNDPROC oWndProc;
bool init = false;

// Window procedure for handling messages
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	return (GUI::bMenuOpen ? true : CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam));
}

// Hooked Present function
extern long __fastcall hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!Cheat::bShouldRun)
		return oPresent(pSwapChain, SyncInterval, Flags);

	if (!init)
	{
		// Initialize ImGui and other resources
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&d3d12Device))) {
			ImGui::CreateContext();
			// ... (omitted for brevity)
			oWndProc = (WNDPROC)SetWindowLongPtrA(Window, GWLP_WNDPROC, (LONG_PTR)WndProc);
		}
		init = true;
	}

	// Render ImGui
	GUI::BeginRender();
	ImGui::PushFont(tahomaFont);
	GUI::Render();
	ImGui::PopFont();
	GUI::EndRender();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12CommandList);

	// Execute the command list
	d3d12CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&d3d12CommandList));

	return oPresent(pSwapChain, SyncInterval, Flags);
}

// Hooked ExecuteCommandLists function
void hkExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists)
{
	if (!d3d12CommandQueue)
		d3d12CommandQueue = queue;

	oExecuteCommandLists(queue, NumCommandLists, ppCommandLists);
}

// Function to release D3D12 resources
void D3D12Release()
{
	d3d12Device->Release();
	d3d12DescriptorHeapBackBuffers->Release();
	d3d12DescriptorHeapImGuiRender->Release();
	d3d12CommandList->Release();
	d3d12CommandQueue->Release();

	oWndProc = (WNDPROC)SetWindowLongPtrA(Window, GWLP_WNDPROC, (LONG_PTR)oWndProc);
}

#endif