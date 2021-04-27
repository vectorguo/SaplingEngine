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
		static D3D12_GPU_DESCRIPTOR_HANDLE GetPassCbvDescriptor(const std::string& shaderName)
		{
			return GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeapPointers[shaderName], DoubleConstantBufferElementCount, CbvDescriptorSize);
		}
		
		/**
		 * \brief	获取常量缓冲区描述符堆的指针数组
		 * \return	常量缓冲区描述符堆的指针数组
		 */
		static ID3D12DescriptorHeap** GetCbvDescriptorHeaps(const std::string& shaderName)
		{
			return &m_CbvDescriptorHeapPointers[shaderName];
		}

		/**
		 * \brief	获取常量缓冲区描述符堆的数量
		 * \return	常量缓冲区描述符堆的数量
		 */
		static uint32_t GetCbvDescriptorHeapCount()
		{
			return static_cast<uint32_t>(m_CbvDescriptorHeapPointers.size());
		}

		/**
		 * \brief	压入可用的物体常量缓冲区索引
		 */
		static void PushCbvIndex(const std::string& shaderName, uint32_t index);
		
		/**
		 * \brief	弹出可用的物体常量缓冲区索引
		 */
		static uint32_t PopCbvIndex(const std::string& shaderName, D3D12_GPU_DESCRIPTOR_HANDLE& commonCbvDescriptor, D3D12_GPU_DESCRIPTOR_HANDLE& specialCbvDescriptor);

		/**
		 * \brief	填充物体常量缓冲区数据
		 * \param	index			可用的物体常量缓冲区索引
		 * \param	pCommonData		通用数据
		 * \param	commonDataSize	通用数据大小
		 * \param	pSpecialData	特殊数据
		 * \param	specialDataSize	特殊数据大小
		 */
		static void FillOcbData(const std::string& shaderName, uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize);

		/**
		 * \brief	填充Pass常量缓冲区数据
		 * \param	pData			通用数据
		 * \param	dataSize		通用数据大小
		 */
		static void FillPcbData(const std::string& shaderName, const void* pData, size_t dataSize);
		
	private:
		/**
		 * \brief	创建常量缓冲区描述符堆
		 */
		static void CreateCbvDescriptorHeap(const std::string& shaderName);
		
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
		 * \brief	常量缓冲区的元素数量
		 */
		static constexpr uint32_t ConstantBufferElementCount = 100;
		static constexpr uint32_t DoubleConstantBufferElementCount = ConstantBufferElementCount * 2;
		
		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

		/**
		 * \brief	常量缓冲区描述符大小
		 */
		static uint32_t CbvDescriptorSize;

	private:
		/**
		 * \brief	常量缓冲区描述符堆指针
		 */
		static std::map<std::string, ComPtr<ID3D12DescriptorHeap>> m_CbvDescriptorHeaps;
		static std::map<std::string, ID3D12DescriptorHeap*> m_CbvDescriptorHeapPointers;
		
		/**
		 * \brief	Object的数据的上传缓冲区列表
		 */
		static std::map<std::string, ObjectUploadBufferData> m_ObjectUploadBuffers;

		/**
		 * \brief	可用的物体上传缓冲区索引列表
		 */
		static std::map<std::string, std::vector<uint32_t>> availableCbvIndices;
	};

	using CBufferManager = Dx12CBufferManager;
}