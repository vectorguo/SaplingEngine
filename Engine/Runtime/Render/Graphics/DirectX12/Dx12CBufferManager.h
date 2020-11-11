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
		Dx12CBufferManager()
		{
			m_Instance = this;
		}
		~Dx12CBufferManager() = default;

		Dx12CBufferManager(const Dx12CBufferManager&) = delete;
		Dx12CBufferManager(Dx12CBufferManager&&) = delete;
		Dx12CBufferManager& operator=(const Dx12CBufferManager&) = delete;
		Dx12CBufferManager& operator=(Dx12CBufferManager&&) = delete;
		
		/**
		 * \brief 获取单例
		 * \return 单例
		 */
		static Dx12CBufferManager* Instance()
		{
			return m_Instance;
		}
		
		/**
		 * \brief 初始化
		 */
		void Initialize();

		/**
		 * \brief 销毁
		 */
		void Destroy();

		/**
		 * \brief 更新Object常量缓冲区数据
		 */
		void UpdateOcbData(Scene* pActiveScene);

		/**
		 * \brief 更新Pass常量缓冲区数据
		 */
		void UpdatePcbData(Camera* pCamera);

		/**
		 * \brief 获取Object常量缓冲区描述
		 * \param ocbIndex Object的常量缓冲区描述符索引
		 * \return Object常量缓冲区描述
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetObjectCbvDescriptor(uint32_t ocbIndex) const;

		/**
		 * \brief 获取Pass常量缓冲区描述
		 * \return 常量Pass缓冲区描述
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetPassCbvDescriptor() const;
		
		/**
		 * \brief 获取常量缓冲区描述符堆的指针数组
		 * \return 常量缓冲区描述符堆的指针数组
		 */
		ID3D12DescriptorHeap** GetCbvDescriptorHeaps() const
		{
			return &m_CbvDescriptorHeapPointers[0];
		}

		/**
		 * \brief 获取常量缓冲区描述符堆的数量
		 * \return 常量缓冲区描述符堆的数量
		 */
		uint32_t GetCbvDescriptorHeapCount() const
		{
			return 1;
		}
		
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
		
	private:
		/**
		 * \brief 创建常量缓冲区描述符堆
		 * \param pDevice Dx12设备指针
		 */
		void CreateCbvDescriptorHeap(ID3D12Device* pDevice);
		
		/**
		 * \brief 创建上传缓冲区
		 * \param descriptorHeap 描述符堆
		 * \param uploadBuffer 上传缓冲区指针
		 * \param mappedData 数据指针
		 * \param elementCount 元素数量
		 * \param elementSize 元素大小
		 * \param descriptorHeapOffset 在常量缓冲区描述符堆中的偏移位置
		 */
		void CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset) const;
		
	public:
		/**
		 * \brief 常量缓冲区的元素数量
		 */
		static constexpr uint32_t ConstantBufferElementCount = 100;
		static constexpr uint32_t DoubleConstantBufferElementCount = ConstantBufferElementCount * 2;
		static constexpr uint32_t ConstantBufferDescriptorHeapSize = DoubleConstantBufferElementCount * 10 + 1;
		
		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

		/**
		 * \brief 常量缓冲区描述符大小
		 */
		static uint32_t CbvDescriptorSize;

	private:
		/**
		 * \brief 单例
		 */
		static Dx12CBufferManager* m_Instance;

		/**
		 * \brief 常量缓冲区描述符堆指针
		 */
		ComPtr<ID3D12DescriptorHeap> m_CbvDescriptorHeap;
		ID3D12DescriptorHeap** m_CbvDescriptorHeapPointers = nullptr;
		
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

		/**
		 * \brief Pass常量缓冲区描述符偏移
		 */
		uint32_t m_PassCbvDescriptorOffset = 0;
	};

	using CBufferManager = Dx12CBufferManager;
}