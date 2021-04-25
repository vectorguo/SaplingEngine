#include "Dx12CBufferManager.h"
#include "Dx12GraphicsManager.h"
#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Renderer/Renderer.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	uint32_t						Dx12CBufferManager::CbvDescriptorSize = 0;

	ComPtr<ID3D12DescriptorHeap>	Dx12CBufferManager::m_CbvDescriptorHeap;
	ID3D12DescriptorHeap**			Dx12CBufferManager::m_CbvDescriptorHeapPointers = nullptr;
	std::vector<Dx12CBufferManager::ObjectUploadBufferData> Dx12CBufferManager::m_ObjectUploadBuffers;
	std::vector<uint32_t>			Dx12CBufferManager::m_AvailableOubIndices;
	ComPtr<ID3D12Resource>			Dx12CBufferManager::m_PassCommonUploadBuffer;
	uint8_t*						Dx12CBufferManager::m_PassCommonMappedData = nullptr;
	uint32_t						Dx12CBufferManager::m_PassCbvDescriptorOffset = 0;
	
	/**
	 * \brief 初始化
	 */
	void Dx12CBufferManager::Initialize()
	{
		auto* pDevice = GraphicsManager::GetDx12Device();
		
		//查询描述符大小
		CbvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//创建常量缓冲区描述符堆
		CreateCbvDescriptorHeap(pDevice);

		//记录可用Object上传缓冲区索引
		m_AvailableOubIndices.reserve(ConstantBufferElementCount);
		for (uint32_t index = 0; index < ConstantBufferElementCount; ++index)
		{
			m_AvailableOubIndices.emplace_back(index);
		}
	}

	/**
	 * \brief 销毁
	 */
	void Dx12CBufferManager::Destroy()
	{
		delete[] m_CbvDescriptorHeapPointers;
		m_CbvDescriptorHeapPointers = nullptr;
	}

	/**
	 * \brief 更新Object常量缓冲区数据
	 */
	void Dx12CBufferManager::UpdateOcbData(Scene* pActiveScene)
	{
		size_t dataSize;
		const auto& renderItems = pActiveScene->GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			auto* pRenderer = *iter;
			const auto* pCommonData = CommonOcbData::FillOcbData(dataSize, pRenderer->GetTransform());
			const auto index = pRenderer->GetCommonOcbIndex();
			const auto offset = static_cast<uint64_t>(index % DoubleConstantBufferElementCount);
			const auto& uploadBuffer = m_ObjectUploadBuffers[index / DoubleConstantBufferElementCount];
			memcpy(uploadBuffer.CommonMappedData + offset * static_cast<uint64_t>(ObjectCommonCbSize), pCommonData, dataSize);

			const auto* pSpecialData = pRenderer->GetSpecialOcbData()->FillOcbData(dataSize, pRenderer->GetMaterial());
			memcpy(uploadBuffer.SpecialMappedData + offset * static_cast<uint64_t>(ObjectSpecialCbSize), pSpecialData, dataSize);
		}
	}

	/**
	 * \brief 更新Pass常量缓冲区数据
	 */
	void Dx12CBufferManager::UpdatePcbData(Camera* pCamera)
	{
		size_t dataSize;
		const auto* pPassData = CommonPcbData::FillPcbData(dataSize, pCamera);
		memcpy(m_PassCommonMappedData, pPassData, dataSize);
	}

	/**
	 * \brief 获取Object常量缓冲区描述
	 * \param ocbIndex Object的常量缓冲区描述符索引
	 * \return Object常量缓冲区描述
	 */
	D3D12_GPU_DESCRIPTOR_HANDLE Dx12CBufferManager::GetObjectCbvDescriptor(uint32_t ocbIndex)
	{
		return GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), ocbIndex, CbvDescriptorSize);
	}

	/**
	 * \brief 获取Pass常量缓冲区描述
	 * \return 常量Pass缓冲区描述
	 */
	D3D12_GPU_DESCRIPTOR_HANDLE Dx12CBufferManager::GetPassCbvDescriptor()
	{
		return GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), m_PassCbvDescriptorOffset, CbvDescriptorSize);
	}

	/**
	 * \brief 压入可用的物体常量缓冲区索引
	 * \param index 可用的物体常量缓冲区索引
	 */
	void Dx12CBufferManager::PushObjectCbIndex(uint32_t index)
	{
		if (index < m_ObjectUploadBuffers.size() * ConstantBufferElementCount)
		{
			m_AvailableOubIndices.emplace_back(index);
		}
	}

	/**
	 * \brief 弹出可用的物体常量缓冲区索引
	 * \return 可用的物体常量缓冲区索引
	 */
	uint32_t Dx12CBufferManager::PopObjectCbIndex()
	{
		if (m_AvailableOubIndices.empty())
		{
			const auto startIndex = static_cast<uint32_t>(m_ObjectUploadBuffers.size()) * DoubleConstantBufferElementCount;
			
			//创建新的Object上传缓冲区
			m_ObjectUploadBuffers.emplace_back();
			auto iter = m_ObjectUploadBuffers.rbegin();
			CreateUploadBuffer(m_CbvDescriptorHeap.Get(), iter->CommonUploadBuffer, iter->CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, startIndex);
			CreateUploadBuffer(m_CbvDescriptorHeap.Get(), iter->SpecialUploadBuffer, iter->SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, startIndex + ConstantBufferElementCount);

			//记录可用Object上传缓冲区索引
			m_AvailableOubIndices.reserve(ConstantBufferElementCount);
			for (uint32_t index = 0; index < ConstantBufferElementCount; ++index)
			{
				m_AvailableOubIndices.emplace_back(startIndex + index);
			}
		}

		const auto index = *m_AvailableOubIndices.rbegin();
		m_AvailableOubIndices.pop_back();
		return index;
	}

	/**
	 * \brief 创建常量缓冲区描述符
	 * \param pDevice Dx12设备指针
	 */
	void Dx12CBufferManager::CreateCbvDescriptorHeap(ID3D12Device* pDevice)
	{
		//常量缓冲区描述符/着色器资源描述符/无需访问描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = ConstantBufferDescriptorHeapSize;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//创建常量缓冲区描述符堆
		ThrowIfFailed(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_CbvDescriptorHeap.GetAddressOf())));

		//创建常量缓冲区描述符堆数组
		m_CbvDescriptorHeapPointers = new ID3D12DescriptorHeap*[1];
		m_CbvDescriptorHeapPointers[0] = m_CbvDescriptorHeap.Get();

		//创建Object上传缓冲区
		m_ObjectUploadBuffers.resize(1);
		auto& objectUploadBuffer = m_ObjectUploadBuffers[0];
		CreateUploadBuffer(m_CbvDescriptorHeap.Get(), objectUploadBuffer.CommonUploadBuffer, objectUploadBuffer.CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, 0);
		CreateUploadBuffer(m_CbvDescriptorHeap.Get(), objectUploadBuffer.SpecialUploadBuffer, objectUploadBuffer.SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, ConstantBufferElementCount);

		//创建Pass上传缓冲区
		m_PassCbvDescriptorOffset = ConstantBufferDescriptorHeapSize - 1;
		CreateUploadBuffer(m_CbvDescriptorHeap.Get(), m_PassCommonUploadBuffer, m_PassCommonMappedData, 1, PassCommonCbSize, m_PassCbvDescriptorOffset);
	}
	
	/**
	 * \brief 创建上传缓冲区
	 * \param descriptorHeap 描述符堆
	 * \param uploadBuffer 上传缓冲区指针
	 * \param mappedData 数据指针
	 * \param elementCount 元素数量
	 * \param elementSize 元素大小
	 * \param descriptorHeapOffset 在常量缓冲区描述符堆中的偏移位置
	 */
	void Dx12CBufferManager::CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset)
	{
		//创建上传缓冲区
		D3D12_HEAP_PROPERTIES headProperties
		{
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

		D3D12_RESOURCE_DESC bufferDesc
		{
			D3D12_RESOURCE_DIMENSION_BUFFER,
			0,
			static_cast<uint64_t>(elementCount) * static_cast<uint64_t>(elementSize),
			1,
			1,
			1,
			DXGI_FORMAT_UNKNOWN,
			{1, 0},
			D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			D3D12_RESOURCE_FLAG_NONE
		};

		auto* pDevice = GraphicsManager::GetDx12Device();
		ThrowIfFailed(pDevice->CreateCommittedResource(&headProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer)));
		ThrowIfFailed(uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));

		//创建对应的常量缓冲区描述符
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		for (uint32_t index = 0; index < elementCount; ++index)
		{
			cbvDesc.BufferLocation = uploadBuffer->GetGPUVirtualAddress() + static_cast<uint64_t>(index) * elementSize;
			cbvDesc.SizeInBytes = elementSize;
			pDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(descriptorHeap, descriptorHeapOffset + index, CbvDescriptorSize));
		}
	}
}
