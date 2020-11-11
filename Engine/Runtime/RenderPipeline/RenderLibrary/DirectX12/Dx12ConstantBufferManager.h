#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12ConstantBufferManager final
	{
		struct ObjectUploadBufferData
		{
			ComPtr<ID3D12Resource> CommonUploadBuffer;
			uint8_t* CommonMappedData = nullptr;
			ComPtr<ID3D12Resource> SpecialUploadBuffer;
			uint8_t* SpecialMappedData = nullptr;
		};
		
	public:
		Dx12ConstantBufferManager(ID3D12Device* pDevice, ID3D12DescriptorHeap* pCbvDescriptorHeap, uint32_t cbvDescriptorSize);
		~Dx12ConstantBufferManager() = default;

		Dx12ConstantBufferManager(const Dx12ConstantBufferManager&) = delete;
		Dx12ConstantBufferManager(Dx12ConstantBufferManager&&) = delete;
		Dx12ConstantBufferManager& operator=(const Dx12ConstantBufferManager&) = delete;
		Dx12ConstantBufferManager& operator=(Dx12ConstantBufferManager&&) = delete;

		/**
		 * \brief 压入可用的物体常量缓冲区索引
		 * \param index 可用的物体常量缓冲区索引
		 */
		void PushObjectCbIndex(uint32_t index);
		
		/**
		 * \brief 弹出可用的物体常量缓冲区索引
		 * \return 可用的物体常量缓冲区索引
		 */
		uint32_t PopObjectCbIndex();

		/**
		 * \brief 复制数据到常量缓冲区
		 * \param index 物体的常量缓冲区索引
		 * \param pData 数据指针
		 * \param dataSize 数据大小
		 * \param isSpecial 是否是特殊常量缓冲区数据
		 */
		void CopyObjectCbData(uint32_t index, const void* pData, size_t dataSize, bool isSpecial);

		void CopyPassCbData(const void* pData, size_t dataSize);
		
	private:
		/**
		 * \brief 创建上传缓冲区
		 * \param uploadBuffer 上传缓冲区指针
		 * \param mappedData 数据指针
		 * \param elementCount 元素数量
		 * \param elementSize 元素大小
		 * \param descriptorHeapOffset 在常量缓冲区描述符堆中的偏移位置
		 */
		void CreateUploadBuffer(ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset) const;
		
	public:
		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

	private:
		/**
		 * \brief Dx12的Device指针
		 */
		ID3D12Device* m_pD3DDevice;

		/**
		 * \brief 常量缓冲区描述符堆指针
		 */
		ID3D12DescriptorHeap* m_pCbvDescriptorHeap;

		/**
		 * \brief 常量缓冲区描述符大小
		 */
		uint32_t m_CbvDescriptorSize;
		
		/**
		 * \brief Object的数据的上传缓冲区列表
		 */
		std::vector<ObjectUploadBufferData> m_ObjectUploadBuffers;

		/**
		 * \brief 可用的物体上传缓冲区索引列表
		 */
		std::vector<uint32_t> m_AvailableOubIndices;
		
		/**
		 * \brief Pass通用数据上传缓冲区
		 */
		ComPtr<ID3D12Resource> m_PassCommonUploadBuffer;

		/**
		 * \brief Pass通用上传缓冲区指针
		 */
		uint8_t* m_PassCommonMappedData = nullptr;
	};
}