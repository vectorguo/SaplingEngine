#include "Render/Graphics/DirectX12/Dx12DescriptorManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"

namespace SaplingEngine
{
	uint32_t								Dx12DescriptorManager::rtvDescriptorSize = 0;
	uint32_t								Dx12DescriptorManager::dsvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap>			Dx12DescriptorManager::defaultRtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>			Dx12DescriptorManager::defaultDsvDescriptorHeap;

	void Dx12DescriptorManager::Initialize()
	{
		auto* pDevice = GraphicsManager::GetDx12Device();
		rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		dsvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		const auto swapBufferCount = GraphicsManager::swapChainBufferCount;

		//创建默认渲染缓冲区和深度/模板缓冲区描述符堆
		CreateDescriptorHeap(pDevice, defaultRtvDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapBufferCount);
		CreateDescriptorHeap(pDevice, defaultDsvDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);

		auto* pSwapChainBuffer = GraphicsManager::swapChainBuffer;
		auto* pDepthStencilBuffer = GraphicsManager::depthStencilBuffer.Get();

		//创建渲染目标View
		for (auto i = 0; i < swapBufferCount; ++i)
		{
			CreateRenderTargetView(pDevice, defaultRtvDescriptorHeap.Get(), pSwapChainBuffer[i].Get(), i);
		}

		//创建深度模板View
		CreateDepthStencilView(pDevice, Dx12GraphicsManager::depthStencilViewFormat, defaultDsvDescriptorHeap.Get(), pDepthStencilBuffer, 0);
	}

	void Dx12DescriptorManager::CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		auto* pDevice = GraphicsManager::GetDx12Device();
		CreateDescriptorHeap(pDevice, descriptorHeap, type, descriptorCount, flags);
	}

	void Dx12DescriptorManager::CreateDescriptorHeap(ID3D12Device* pDevice, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = type;
		heapDesc.NumDescriptors = descriptorCount;
		heapDesc.Flags = flags;
		heapDesc.NodeMask = 0;

		ThrowIfFailed(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf())));
	}
	
	void Dx12DescriptorManager::CreateRenderTargetView(ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		CreateRenderTargetView(GraphicsManager::GetDx12Device(), pHeap, pResource, offset);
	}

	void Dx12DescriptorManager::CreateRenderTargetView(ID3D12Device* pDevice, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		auto descriptorHandle = GetCPUHandleFromDescriptorHeap(pHeap, offset, rtvDescriptorSize);
		pDevice->CreateRenderTargetView(pResource, nullptr, descriptorHandle);
	}
	
	void Dx12DescriptorManager::CreateDepthStencilView(DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		CreateDepthStencilView(GraphicsManager::GetDx12Device(), format, pHeap, pResource, offset);
	}
	
	void Dx12DescriptorManager::CreateDepthStencilView(ID3D12Device* pDevice, DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = format;
		dsvDesc.Texture2D.MipSlice = 0;

		auto descriptorHandle = GetCPUHandleFromDescriptorHeap(pHeap, offset, dsvDescriptorSize);
		pDevice->CreateDepthStencilView(pResource, &dsvDesc, descriptorHandle);
	}
	
	D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorManager::GetRenderTargetView()
	{
		return GetCPUHandleFromDescriptorHeap(defaultRtvDescriptorHeap.Get(), GraphicsManager::currentSwapChainIndex, rtvDescriptorSize);
	}
	
	D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorManager::GetDepthStencilView()
	{
		return defaultDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}
}
