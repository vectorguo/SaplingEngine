#include "Render/Graphics/DirectX12/Dx12CBufferManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/Texture2D.h"

namespace SaplingEngine
{
	uint32_t											Dx12CBufferManager::cbvSrvDescriptorSize = 0;
	std::map<size_t, ComPtr<ID3D12DescriptorHeap>>		Dx12CBufferManager::cbvDescriptorHeaps;
	std::map<size_t, ID3D12DescriptorHeap*>				Dx12CBufferManager::cbvDescriptorHeapPointers;
	ComPtr<ID3D12DescriptorHeap>						Dx12CBufferManager::srvDescriptorHeap;
	ID3D12DescriptorHeap*								Dx12CBufferManager::srvDescriptorHeapPointer;
	std::map<size_t, Dx12CBufferManager::ObjectUploadBufferData> Dx12CBufferManager::objectUploadBuffers;
	std::map<size_t, std::vector<uint32_t>>				Dx12CBufferManager::availableCbvIndices;
	std::vector<uint32_t>								Dx12CBufferManager::availableSrvIndices;
	
	/**
	 * \brief	初始化
	 */
	void Dx12CBufferManager::Initialize()
	{		
		//查询描述符大小
		cbvSrvDescriptorSize = GraphicsManager::GetDx12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//创建SRV描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = SrvCount;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvDescriptorHeap)));
		srvDescriptorHeapPointer = srvDescriptorHeap.Get();

		availableSrvIndices.reserve(SrvCount);
		for (auto i = 0; i < SrvCount; ++i)
		{
			availableSrvIndices.push_back(i);
		}		
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
	void Dx12CBufferManager::PushCbvIndex(size_t shaderHashValue, uint32_t index)
	{
		availableCbvIndices[shaderHashValue].push_back(index);
	}

	/**
	 * \brief	弹出可用的物体常量缓冲区索引
	 */
	uint32_t Dx12CBufferManager::PopCbvIndex(size_t shaderHashValue, D3D12_GPU_DESCRIPTOR_HANDLE& commonCbvDescriptor, D3D12_GPU_DESCRIPTOR_HANDLE& specialCbvDescriptor, D3D12_GPU_VIRTUAL_ADDRESS& commonCbAddress, D3D12_GPU_VIRTUAL_ADDRESS& specialCbAddress)
	{
		uint32_t index;
		auto iter = availableCbvIndices.find(shaderHashValue);
		if (iter == availableCbvIndices.end())
		{
			CreateCbvDescriptorHeap(shaderHashValue);
			
			auto& indices = availableCbvIndices[shaderHashValue];
			index = *indices.rbegin();
			indices.pop_back();
		}
		else
		{
			index = *iter->second.rbegin();
			iter->second.pop_back();
		}

		auto* pCbvDescriptorHeap = cbvDescriptorHeapPointers[shaderHashValue];
		commonCbvDescriptor = GetGPUHandleFromDescriptorHeap(pCbvDescriptorHeap, index, cbvSrvDescriptorSize);
		specialCbvDescriptor = GetGPUHandleFromDescriptorHeap(pCbvDescriptorHeap, index + ConstantBufferElementCount, cbvSrvDescriptorSize);

		auto& objectUploadBuffer = objectUploadBuffers[shaderHashValue];
		commonCbAddress = objectUploadBuffer.CommonUploadBuffer->GetGPUVirtualAddress() + static_cast<uint64_t>(index) * ObjectCommonCbSize;
		specialCbAddress = objectUploadBuffer.SpecialUploadBuffer->GetGPUVirtualAddress() + static_cast<uint64_t>(index) * ObjectSpecialCbSize;

		return index;
	}

	void Dx12CBufferManager::PushSrvIndex(uint32_t index)
	{
		availableSrvIndices.push_back(index);
	}

	void Dx12CBufferManager::PopSrvIndex(Texture2D* pTexture2D)
	{
		if (availableSrvIndices.empty())
		{
			throw Exception("贴图数量太多，无可用Srv描述符堆空间");
		}

		auto index = *availableSrvIndices.rbegin();
		availableSrvIndices.pop_back();

		pTexture2D->m_SrvIndex = index;
		pTexture2D->m_SrvDescriptor = GetGPUHandleFromDescriptorHeap(srvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);

		auto& pTextureResource = pTexture2D->m_ResourcePtr;
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = pTextureResource->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		GraphicsManager::GetDx12Device()->CreateShaderResourceView(pTextureResource.Get(), &srvDesc, GetCPUHandleFromDescriptorHeap(srvDescriptorHeap.Get(), index, cbvSrvDescriptorSize));
	}

	int32_t Dx12CBufferManager::PopSrvIndex(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
	{
		if (availableSrvIndices.empty())
		{
			throw Exception("贴图数量太多，无可用Srv描述符堆空间");
		}

		auto index = *availableSrvIndices.rbegin();
		availableSrvIndices.pop_back();

		cpuHandle = GetCPUHandleFromDescriptorHeap(srvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);
		gpuHandle = GetGPUHandleFromDescriptorHeap(srvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);

		return index;
	}

	/**
	 * \brief	创建常量缓冲区描述符
	 */
	void Dx12CBufferManager::CreateCbvDescriptorHeap(size_t shaderHashValue)
	{
		//声明常量缓冲区描述符堆
		ComPtr<ID3D12DescriptorHeap> cbvDescriptorHeap;

		//常量缓冲区描述符/着色器资源描述符/无需访问描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = DoubleConstantBufferElementCount + 2;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//创建常量缓冲区描述符堆
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(cbvDescriptorHeap.GetAddressOf())));
		cbvDescriptorHeaps.insert_or_assign(shaderHashValue, cbvDescriptorHeap);
		cbvDescriptorHeapPointers.insert_or_assign(shaderHashValue, cbvDescriptorHeap.Get());

		//创建Object上传缓冲区
		auto& objectUploadBuffer = objectUploadBuffers[shaderHashValue];
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.CommonUploadBuffer, objectUploadBuffer.CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, 0);
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.SpecialUploadBuffer, objectUploadBuffer.SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, ConstantBufferElementCount);
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.PassUploadBuffer, objectUploadBuffer.PassMappedData, 2, PassCommonCbSize, DoubleConstantBufferElementCount);

		//记录可用Object上传缓冲区索引
		auto& objectCbIndices = availableCbvIndices[shaderHashValue];
		objectCbIndices.reserve(ConstantBufferElementCount);
		for (uint32_t index = 0; index < ConstantBufferElementCount; ++index)
		{
			objectCbIndices.emplace_back(index);
		}
	}
	
	/**
	 * \brief	填充物体常量缓冲区数据
	 * \param	shaderHashValue	Shader对应的HashValue
	 * \param	index			可用的物体常量缓冲区索引
	 * \param	pCommonData		通用数据
	 * \param	commonDataSize	通用数据大小
	 * \param	pSpecialData	特殊数据
	 * \param	specialDataSize	特殊数据大小
	 */
	void Dx12CBufferManager::FillOcbData(size_t shaderHashValue, uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize)
	{
		const auto& uploadBuffer = Dx12CBufferManager::objectUploadBuffers[shaderHashValue];
		memcpy(uploadBuffer.CommonMappedData + index * static_cast<uint64_t>(Dx12CBufferManager::ObjectCommonCbSize), pCommonData, commonDataSize);
		memcpy(uploadBuffer.SpecialMappedData + index * static_cast<uint64_t>(Dx12CBufferManager::ObjectSpecialCbSize), pSpecialData, specialDataSize);
	}

	/**
	 * \brief	填充Pass常量缓冲区数据
	 * \param	shaderHashValue	Shader对应的HashValue
	 * \param	pData			通用数据
	 * \param	dataSize		通用数据大小
	 */
	void Dx12CBufferManager::FillPcbData(size_t shaderHashValue, const void* pData, size_t dataSize)
	{
		const auto& uploadBuffer = Dx12CBufferManager::objectUploadBuffers[shaderHashValue];
		memcpy(uploadBuffer.PassMappedData, pData, dataSize);
	}

	/**
	 * \brief	填充ShadowPass常量缓冲区数据
	 * \param	shaderHashValue	Shader对应的HashValue
	 * \param	pData			通用数据
	 * \param	dataSize		通用数据大小
	 */
	void Dx12CBufferManager::FillShadowPcbData(size_t shaderHashValue, const void* pData, size_t dataSize)
	{
		const auto& uploadBuffer = Dx12CBufferManager::objectUploadBuffers[shaderHashValue];
		memcpy(uploadBuffer.PassMappedData + static_cast<uint64_t>(Dx12CBufferManager::PassCommonCbSize), pData, dataSize);
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
			pDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(descriptorHeap, descriptorHeapOffset + index, cbvSrvDescriptorSize));
		}
	}
}
