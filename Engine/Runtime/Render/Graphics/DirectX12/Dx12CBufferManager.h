#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;
	
	class Dx12CBufferManager final
	{
		friend class RenderPipeline;

		struct ObjectUploadBufferData
		{
			ComPtr<ID3D12Resource> CommonUploadBuffer;
			uint8_t* CommonMappedData = nullptr;
			ComPtr<ID3D12Resource> SpecialUploadBuffer;
			uint8_t* SpecialMappedData = nullptr;
			ComPtr<ID3D12Resource> PassUploadBuffer;
			uint8_t* PassMappedData = nullptr;
		};
		
	public:
		/**
		 * \brief	初始化
		 */
		static void Initialize();

		/**
		 * \brief	销毁
		 */
		static void Destroy();

		/**
		 * \brief	获取Pass常量缓冲区描述
		 * \return	常量Pass缓冲区描述
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetPassCbvDescriptor(size_t shaderHashValue)
		{
			return GetGPUHandleFromDescriptorHeap(cbvDescriptorHeapPointers[shaderHashValue], DoubleConstantBufferElementCount, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	获取Pass常量缓冲区地址
		 * \return	常量Pass缓冲区地址
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetPassCbAddress(size_t shaderHashValue)
		{
			const auto& uploadBuffer = objectUploadBuffers[shaderHashValue];
			return uploadBuffer.PassUploadBuffer->GetGPUVirtualAddress();
		}

		/**
		 * \brief	获取ShadowPass常量缓冲区描述
		 * \return	常量ShadowPass缓冲区描述
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetShadowPassCbvDescriptor(size_t shaderHashValue)
		{
			return GetGPUHandleFromDescriptorHeap(cbvDescriptorHeapPointers[shaderHashValue], DoubleConstantBufferElementCount + 1, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	获取ShadowPass常量缓冲区地址
		 * \return	常量ShadowPass缓冲区地址
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetShadowPassCbAddress(size_t shaderHashValue)
		{
			const auto& uploadBuffer = objectUploadBuffers[shaderHashValue];
			return uploadBuffer.PassUploadBuffer->GetGPUVirtualAddress() + PassCommonCbSize;
		}

		/**
		 * \brief	获取常量缓冲区描述符堆的指针数组
		 * \return	常量缓冲区描述符堆的指针数组
		 */
		static ID3D12DescriptorHeap** GetCbvDescriptorHeaps(size_t shaderHashValue)
		{
			return &cbvDescriptorHeapPointers[shaderHashValue];
		}

		static ID3D12DescriptorHeap** GetSrvDescriptorHeaps()
		{
			return &srvDescriptorHeapPointer;
		}

		/**
		 * \brief	压入可用的物体常量缓冲区索引
		 */
		static void PushCbvIndex(size_t shaderHashValue, uint32_t index);
		
		/**
		 * \brief	弹出可用的物体常量缓冲区索引
		 */
		static uint32_t PopCbvIndex(size_t shaderHashValue, D3D12_GPU_DESCRIPTOR_HANDLE& commonCbvDescriptor, D3D12_GPU_DESCRIPTOR_HANDLE& specialCbvDescriptor, D3D12_GPU_VIRTUAL_ADDRESS& commonCbAddress, D3D12_GPU_VIRTUAL_ADDRESS& specialCbAddress);

		static void PushSrvIndex(uint32_t index);

		static void PopSrvIndex(Texture2D* pTexture2D);

		static int32_t PopSrvIndex(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

		/**
		 * \brief	填充物体常量缓冲区数据
		 * \param	shaderHashValue	Shader对应的HashValue
		 * \param	index			可用的物体常量缓冲区索引
		 * \param	pCommonData		通用数据
		 * \param	commonDataSize	通用数据大小
		 * \param	pSpecialData	特殊数据
		 * \param	specialDataSize	特殊数据大小
		 */
		static void FillOcbData(size_t shaderHashValue, uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize);

		/**
		 * \brief	填充Pass常量缓冲区数据
		 * \param	shaderHashValue	Shader对应的HashValue
		 * \param	pData			通用数据
		 * \param	dataSize		通用数据大小
		 */
		static void FillPcbData(size_t shaderHashValue, const void* pData, size_t dataSize);

		/**
		 * \brief	填充ShadowPass常量缓冲区数据
		 * \param	shaderHashValue	Shader对应的HashValue
		 * \param	pData			通用数据
		 * \param	dataSize		通用数据大小
		 */
		static void FillShadowPcbData(size_t shaderHashValue, const void* pData, size_t dataSize);
		
	private:
		/**
		 * \brief	创建常量缓冲区描述符堆
		 */
		static void CreateCbvDescriptorHeap(size_t shaderHashValue);
		
		/**
		 * \brief	创建上传缓冲区
		 * \param	descriptorHeap	描述符堆
		 * \param	uploadBuffer	上传缓冲区指针
		 * \param	mappedData		数据指针
		 * \param	elementCount	元素数量
		 * \param	elementSize		元素大小
		 * \param	descriptorHeapOffset 在常量缓冲区描述符堆中的偏移位置
		 */
		static void CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset);
		
	public:
		/**
		 * \brief	常量缓冲区描述符大小
		 */
		static uint32_t cbvSrvDescriptorSize;

		/**
		 * \brief	常量缓冲区的元素数量
		 */
		static constexpr uint32_t ConstantBufferElementCount = 100;
		static constexpr uint32_t DoubleConstantBufferElementCount = ConstantBufferElementCount * 2;
		
		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

		/**
		 * \brief	SRV
		 */
		static constexpr uint32_t SrvCount = 100;

	private:
		/**
		 * \brief	常量缓冲区描述符堆指针
		 */
		static std::map<size_t, ComPtr<ID3D12DescriptorHeap>> cbvDescriptorHeaps;
		static std::map<size_t, ID3D12DescriptorHeap*> cbvDescriptorHeapPointers;

		/**
		 * \brief	SRV描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;

		static ID3D12DescriptorHeap* srvDescriptorHeapPointer;
		
		/**
		 * \brief	Object的数据的上传缓冲区列表
		 */
		static std::map<size_t, ObjectUploadBufferData> objectUploadBuffers;

		/**
		 * \brief	可用的物体上传缓冲区索引列表
		 */
		static std::map<size_t, std::vector<uint32_t>> availableCbvIndices;

		/**
		 * \brief	可用的SRV索引列表
		 */
		static std::vector<uint32_t> availableSrvIndices;
	};

	using CBufferManager = Dx12CBufferManager;
}