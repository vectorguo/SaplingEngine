#include "Dx12CBufferManager.h"
#include "Dx12GraphicsManager.h"

namespace SaplingEngine
{
	uint32_t											Dx12CBufferManager::CbvDescriptorSize = 0;
	std::map<std::string, ComPtr<ID3D12DescriptorHeap>>	Dx12CBufferManager::m_CbvDescriptorHeaps;
	std::map<std::string, ID3D12DescriptorHeap*>		Dx12CBufferManager::m_CbvDescriptorHeapPointers;
	std::map<std::string, Dx12CBufferManager::ObjectUploadBufferData> Dx12CBufferManager::m_ObjectUploadBuffers;
	std::map<std::string, std::vector<uint32_t>>		Dx12CBufferManager::availableCbvIndices;
	
	/**
	 * \brief	初始化
	 */
	void Dx12CBufferManager::Initialize()
	{		
		//查询描述符大小
		CbvDescriptorSize = GraphicsManager::GetDx12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	/**
	 * \brief	销毁
	 */
	void Dx12CBufferManager::Destroy()
	{
	}

	/**
	 * \brief	压入可用的物体常量缓冲区索引
	 */
	void Dx12CBufferManager::PushCbvIndex(const std::string& shaderName, uint32_t index)
	{
		availableCbvIndices[shaderName].push_back(index);
	}

	/**
	 * \brief	弹出可用的物体常量缓冲区索引
	 */
	uint32_t Dx12CBufferManager::PopCbvIndex(const std::string& shaderName, D3D12_GPU_DESCRIPTOR_HANDLE& commonCbvDescriptor, D3D12_GPU_DESCRIPTOR_HANDLE& specialCbvDescriptor)
	{
		uint32_t index;
		auto iter = availableCbvIndices.find(shaderName);
		if (iter == availableCbvIndices.end())
		{
			CreateCbvDescriptorHeap(shaderName);
			
			auto& indices = availableCbvIndices[shaderName];
			index = *indices.rbegin();
			indices.pop_back();
		}
		else
		{
			index = *iter->second.rbegin();
			iter->second.pop_back();
		}

		commonCbvDescriptor = GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeapPointers[shaderName], index, CbvDescriptorSize);
		specialCbvDescriptor = GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeapPointers[shaderName], index + ConstantBufferElementCount, CbvDescriptorSize);
		return index;
	}

	/**
	 * \brief	创建常量缓冲区描述符
	 */
	void Dx12CBufferManager::CreateCbvDescriptorHeap(const std::string& shaderName)
	{
		//声明常量缓冲区描述符堆
		ComPtr<ID3D12DescriptorHeap> cbvDescriptorHeap;

		//常量缓冲区描述符/着色器资源描述符/无需访问描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = DoubleConstantBufferElementCount + 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//创建常量缓冲区描述符堆
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(cbvDescriptorHeap.GetAddressOf())));
		m_CbvDescriptorHeaps.insert_or_assign(shaderName, cbvDescriptorHeap);
		m_CbvDescriptorHeapPointers.insert_or_assign(shaderName, cbvDescriptorHeap.Get());

		//创建Object上传缓冲区
		auto& objectUploadBuffer = m_ObjectUploadBuffers[shaderName];
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.CommonUploadBuffer, objectUploadBuffer.CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, 0);
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.SpecialUploadBuffer, objectUploadBuffer.SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, ConstantBufferElementCount);
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.PassUploadBuffer, objectUploadBuffer.PassMappedData, 1, PassCommonCbSize, DoubleConstantBufferElementCount);

		//记录可用Object上传缓冲区索引
		auto& objectCbIndices = availableCbvIndices[shaderName];
		objectCbIndices.reserve(ConstantBufferElementCount);
		for (uint32_t index = 0; index < ConstantBufferElementCount; ++index)
		{
			objectCbIndices.emplace_back(index);
		}
	}
	
	/**
	 * \brief	填充物体常量缓冲区数据
	 * \param	index			可用的物体常量缓冲区索引
	 * \param	pCommonData		通用数据
	 * \param	commonDataSize	通用数据大小
	 * \param	pSpecialData	特殊数据
	 * \param	specialDataSize	特殊数据大小
	 */
	void Dx12CBufferManager::FillOcbData(const std::string& shaderName, uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize)
	{
		const auto& uploadBuffer = Dx12CBufferManager::m_ObjectUploadBuffers[shaderName];
		memcpy(uploadBuffer.CommonMappedData + index * static_cast<uint64_t>(Dx12CBufferManager::ObjectCommonCbSize), pCommonData, commonDataSize);
		memcpy(uploadBuffer.SpecialMappedData + index * static_cast<uint64_t>(Dx12CBufferManager::ObjectSpecialCbSize), pSpecialData, specialDataSize);
	}

	/**
	 * \brief	填充Pass常量缓冲区数据
	 * \param	pData			通用数据
	 * \param	dataSize		通用数据大小
	 */
	void Dx12CBufferManager::FillPcbData(const std::string& shaderName, const void* pData, size_t dataSize)
	{
		const auto& uploadBuffer = Dx12CBufferManager::m_ObjectUploadBuffers[shaderName];
		memcpy(uploadBuffer.PassMappedData, pData, dataSize);
	}

	/**
	 * \brief	创建上传缓冲区
	 * \param	descriptorHeap	描述符堆
	 * \param	uploadBuffer	上传缓冲区指针
	 * \param	mappedData		数据指针
	 * \param	elementCount	元素数量
	 * \param	elementSize		元素大小
	 * \param	descriptorHeapOffset 在常量缓冲区描述符堆中的偏移位置
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
