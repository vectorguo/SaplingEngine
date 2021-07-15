#pragma once

#include "Render/Graphics/DirectX12/Dx12UploadBuffer.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12DescriptorManager
	{
		friend class Dx12GraphicsManager;

	public:
		struct Dx12DescriptorHeap
		{
			Dx12DescriptorHeap(uint32_t uploadBufferSize) : UploadBuffer(uploadBufferSize)
			{
				CreateDescriptorHeap(DescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, ObjectCbvDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
			}

			ComPtr<ID3D12DescriptorHeap> DescriptorHeap;
			Dx12UploadBuffer UploadBuffer;
		};

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

		static D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView();
		static D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView();

		/**
		 * \brief	获取对象常量缓冲区描述符堆
		 */
		static Dx12DescriptorHeap* GetObjectCbvDescriptorHeap();

	public:
		/**
		 * \brief	物体常量缓冲区描述符堆中描述符的数量
		 */
		static constexpr uint32_t ObjectCbvDescriptorCount = 100;

		/**
		 * \brief	物体通用常量缓冲区大小
		 */
		static constexpr uint32_t ObjectCommonCbSize = 256;

		/**
		 * \brief	物体特殊常量缓冲区大小
		 */
		static constexpr uint32_t ObjectSpecialCbSize = 512;

		/**
		 * \brief	Pass常量缓冲区大小
		 */
		static constexpr uint32_t PassCommonCbSize = 512;

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
		 * \brief	常量缓冲区描述符大小
		 */
		static uint32_t cbvSrvDescriptorSize;

		/**
		 * \brief	默认渲染对象描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultRtvDescriptorHeap;

		/**
		 * \brief	默认深度/模板描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultDsvDescriptorHeap;

		/**
		 * \brief	物体常量缓冲区描述符堆列表
		 */
		static std::vector<Dx12DescriptorHeap*> objectCbvDescriptorHeaps;
	};

	using DescriptorManager = Dx12DescriptorManager;
}