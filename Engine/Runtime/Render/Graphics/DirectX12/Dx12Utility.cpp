#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/DirectX12/Dx12Utility.h"

namespace SaplingEngine
{
	void Dx12Utility::CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		auto* pDevice = GraphicsManager::GetDx12Device();
		CreateDescriptorHeap(pDevice, descriptorHeap, type, descriptorCount, flags);
	}

	void Dx12Utility::CreateDescriptorHeap(ID3D12Device* pDevice, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = type;
		heapDesc.NumDescriptors = descriptorCount;
		heapDesc.Flags = flags;
		heapDesc.NodeMask = 0;

		ThrowIfFailed(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf())));
	}
}