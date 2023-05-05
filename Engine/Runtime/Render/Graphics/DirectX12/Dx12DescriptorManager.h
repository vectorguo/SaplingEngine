#pragma once

#include "Runtime/SaplingEnginePch.h"
#include "Runtime/Render/Graphics/DirectX12/Dx12UploadBuffer.h"

namespace SaplingEngine
{
	class Dx12DescriptorManager
	{
		friend class GraphicsManager;

	public:
		/**
		 * \brief	初始化
		 */
		static void Initialize();

		/**
		 * \brief	销毁
		 */
		static void Destroy();

		static void CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		static void CreateDescriptorHeap(ID3D12Device* pDevice, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		static void CreateRenderTargetView(ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateRenderTargetView(ID3D12Device* pDevice, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(ID3D12Device* pDevice, DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateShaderResourceView(ID3D12Resource* pResource, uint32_t index, uint32_t shaderMapping, DXGI_FORMAT format, D3D12_SRV_DIMENSION viewDimension);
		static void CreateShaderResourceView(ID3D12Device* pDevice, ID3D12Resource* pResource, uint32_t index, uint32_t shaderMapping, DXGI_FORMAT format, D3D12_SRV_DIMENSION viewDimension);

		static D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView();
		static D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView();

		/**
		 * \brief	获取对象常量缓冲区描述符堆
		 */
		static void GetObjectCbvDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, Dx12UploadBuffer*& pUploadBuffer);

		/**
		 * \brief	归还对象常量缓冲区描述符堆
		 */
		static void ReturnObjectCbvDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, Dx12UploadBuffer* pUploadBuffer);

		/**
		 * \brief	获取Pass上传缓冲区指针(不需要归还)
		 */
		static Dx12UploadBuffer* GetPassCbUploadBuffer()
		{
			return passCbUploadBuffer;
		}

		/**
		 * \brief	获取Pass常量缓冲区地址
		 * \return	常量Pass缓冲区地址
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetPassCbAddress()
		{
			return passCbUploadBuffer->GetGpuVirtualAddress();
		}

		/**
		 * \brief	获取ShadowPass常量缓冲区地址
		 * \return	常量ShadowPass缓冲区地址
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetShadowPassCbAddress()
		{
			return passCbUploadBuffer->GetGpuVirtualAddress(PassCommonCbSize);
		}

		/**
		 * \brief	获取SRV描述符堆的指针数组
		 * \return	SRV描述符堆的指针数组
		 */
		static ID3D12DescriptorHeap** GetSrvDescriptorHeaps()
		{
			return defaultSrvDescriptorHeap.GetAddressOf();
		}

		/**
		 * \brief	获取SRV描述符
		 * \param	index	SRV描述符堆中的索引
		 * \return	CPU描述符
		 */
		static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvCpuDescriptor(uint32_t index)
		{
			return GetCPUHandleFromDescriptorHeap(defaultSrvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	获取SRV描述符
		 * \param	index	SRV描述符堆中的索引
		 * \return	GPU描述符
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvGpuDescriptor(uint32_t index)
		{
			return GetGPUHandleFromDescriptorHeap(defaultSrvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	获取可用的SRV索引
		 */
		static uint32_t GetSrvIndex()
		{
			if (availableSrvIndices.empty())
			{
				throw Exception("贴图数量太多，无可用Srv描述符堆空间");
			}

			auto index = *availableSrvIndices.rbegin();
			availableSrvIndices.pop_back();
			return index;
		}

		/**
		 * \brief	归还可用的SRV索引
		 */
		static void ReturnSrvIndex(uint32_t index)
		{
			availableSrvIndices.push_back(index);
		}

	private:
		/**
		 * \brief	创建Pass常量缓冲区描述符堆
		 */
		static void CreatePassCbvDescriptorHeap();

		/**
		 * \brief	创建Srv缓冲区描述符堆
		 */
		static void CreateSrvDescriptorHeap();

	public:
		/**
		 * \brief	物体常量缓冲区描述符堆中描述符的数量
		 */
		static constexpr uint32_t ObjectCbvDescriptorCount = 100;

		/**
		 * \brief	Pass常量缓冲区描述符堆中描述符的数量
		 */
		static constexpr uint32_t PassCbvDescriptorCount = 2;

		/**
		 * \brief	物体通用常量缓冲区大小
		 */
		static constexpr uint32_t ObjectCommonCbSize = 256;

		/**
		 * \brief	物体通用常量缓冲区总大小
		 */
		static constexpr uint32_t TotalObjectCommonCbSize = ObjectCbvDescriptorCount * ObjectCommonCbSize;

		/**
		 * \brief	物体特殊常量缓冲区大小
		 */
		static constexpr uint32_t ObjectSpecialCbSize = 512;

		/**
		 * \brief	物体特殊常量缓冲区总大小
		 */
		static constexpr uint32_t TotalObjectSpecialCbSize = ObjectCbvDescriptorCount * ObjectSpecialCbSize;

		/**
		 * \brief	Pass常量缓冲区大小
		 */
		static constexpr uint32_t PassCommonCbSize = 512;

		/**
		 * \brief	SRV描述符数量
		 */
		static constexpr uint32_t SrvCount = 500;

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
		static std::vector<ComPtr<ID3D12DescriptorHeap>> objectCbvDescriptorHeaps;

		/**
		 * \brief	物体常量缓冲区对应的上传缓冲区列表
		 */
		static std::vector<Dx12UploadBuffer*> objectCbUploadBuffers;

		/**
		 * \brief	Pass常量缓冲区描述符堆列表
		 */
		static ComPtr<ID3D12DescriptorHeap> passCbvDescriptorHeap;

		/**
		 * \brief	Pass常量缓冲区对应的上传缓冲区
		 */
		static Dx12UploadBuffer* passCbUploadBuffer;

		/**
		 * \brief	SRV描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultSrvDescriptorHeap;

		/**
		 * \brief	可用的SRV索引列表
		 */
		static std::vector<uint32_t> availableSrvIndices;
	};

	using DescriptorManager = Dx12DescriptorManager;
}