#pragma once

#include <Windows.h>
#include <wrl.h>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include "Runtime/Render/Graphics/DirectX12/d3dx12.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace SaplingEngine
{
	class Dx12Utility
	{

	};
}

static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap)
{
	return pHeap->GetCPUDescriptorHandleForHeapStart();
}

static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap, uint32_t offset, uint32_t descriptorSize)
{
	auto cbvHeapHandle = pHeap->GetCPUDescriptorHandleForHeapStart();
	const auto cbvHeapHandleOffset = offset * descriptorSize;
	cbvHeapHandle.ptr += static_cast<uint64_t>(cbvHeapHandleOffset);
	return cbvHeapHandle;
}

static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap)
{
	return pHeap->GetGPUDescriptorHandleForHeapStart();
}

static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap, uint32_t offset, uint32_t descriptorSize)
{
	auto cbvHeapHandle = pHeap->GetGPUDescriptorHandleForHeapStart();
	const auto cbvHeapHandleOffset = offset * descriptorSize;
	cbvHeapHandle.ptr += static_cast<uint64_t>(cbvHeapHandleOffset);
	return cbvHeapHandle;
}