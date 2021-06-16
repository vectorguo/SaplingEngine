#pragma once

#include "Render/Graphics/DirectX12/Dx12Utility.h"

namespace SaplingEngine
{
	class Dx12DescriptorPool
	{

	};

	class Dx12DescriptorManager
	{
		friend class Dx12GraphicsManager;

	public:
		/**
		 * \brief	初始化
		 */
		static void Initialize();

		static void CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		static void CreateDescriptorHeap(ID3D12Device* pDevice, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		static void CreateRenderTargetView(ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateRenderTargetView(ID3D12Device* pDevice, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(ID3D12Device* pDevice, DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);

	private:
		/**
		 * \brief	渲染目标视图大小
		 */
		static uint32_t rtvDescriptorSize;

		/**
		 * \brief	深度/模板视图大小
		 */
		static uint32_t dsvDescriptorSize;

		/**
		 * \brief	默认渲染对象描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultRtvDescriptorHeap;

		/**
		* \brief	默认深度/模板描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultDsvDescriptorHeap;
	};

	using DescriptorManager = Dx12DescriptorManager;
}