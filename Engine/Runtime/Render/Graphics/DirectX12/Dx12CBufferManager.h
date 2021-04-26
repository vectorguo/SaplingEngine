#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;
	
	class Dx12CBufferManager final
	{
		struct ObjectUploadBufferData
		{
			ComPtr<ID3D12Resource> CommonUploadBuffer;
			uint8_t* CommonMappedData = nullptr;
			ComPtr<ID3D12Resource> SpecialUploadBuffer;
			uint8_t* SpecialMappedData = nullptr;
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
		 * \brief	更新Object常量缓冲区数据
		 */
		static void UpdateOcbData();

		/**
		 * \brief	更新Pass常量缓冲区数据
		 */
		static void UpdatePcbData(Camera* pCamera);

		/**
		 * \brief	获取Object常量缓冲区描述
		 * \param	ocbIndex	Object的常量缓冲区描述符索引
		 * \return	Object常量缓冲区描述
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetObjectCbvDescriptor(uint32_t ocbIndex);

		/**
		 * \brief	获取Pass常量缓冲区描述
		 * \return	常量Pass缓冲区描述
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetPassCbvDescriptor();
		
		/**
		 * \brief	获取常量缓冲区描述符堆的指针数组
		 * \return	常量缓冲区描述符堆的指针数组
		 */
		static ID3D12DescriptorHeap** GetCbvDescriptorHeaps()
		{
			return &m_CbvDescriptorHeapPointers[0];
		}

		/**
		 * \brief	获取常量缓冲区描述符堆的数量
		 * \return	常量缓冲区描述符堆的数量
		 */
		static uint32_t GetCbvDescriptorHeapCount()
		{
			return 1;
		}
		
		/**
		 * \brief	压入可用的物体常量缓冲区索引
		 * \param	index		可用的物体常量缓冲区索引
		 */
		static void PushObjectCbIndex(uint32_t index);
		
		/**
		 * \brief	弹出可用的物体常量缓冲区索引
		 * \return	可用的物体常量缓冲区索引
		 */
		static uint32_t PopObjectCbIndex();
		
	private:
		/**
		 * \brief	创建常量缓冲区描述符堆
		 * \param	pDevice		Dx12设备指针
		 */
		static void CreateCbvDescriptorHeap(ID3D12Device* pDevice);
		
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
		static constexpr uint32_t ConstantBufferDescriptorHeapSize = DoubleConstantBufferElementCount + 1;
		
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
		static ComPtr<ID3D12DescriptorHeap> m_CbvDescriptorHeap;
		static ID3D12DescriptorHeap** m_CbvDescriptorHeapPointers;
		
		/**
		 * \brief	Object的数据的上传缓冲区列表
		 */
		static std::vector<ObjectUploadBufferData> m_ObjectUploadBuffers;

		/**
		 * \brief	可用的物体上传缓冲区索引列表
		 */
		static std::vector<uint32_t> m_AvailableOubIndices;
		
		/**
		 * \brief	Pass通用数据上传缓冲区
		 */
		static ComPtr<ID3D12Resource> m_PassCommonUploadBuffer;

		/**
		 * \brief	Pass通用上传缓冲区指针
		 */
		static uint8_t* m_PassCommonMappedData;

		/**
		 * \brief	Pass常量缓冲区描述符偏移
		 */
		static uint32_t m_PassCbvDescriptorOffset;
	};

	using CBufferManager = Dx12CBufferManager;
}