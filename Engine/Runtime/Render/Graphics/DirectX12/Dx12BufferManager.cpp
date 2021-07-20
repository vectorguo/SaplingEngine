#include "Render/Graphics/DirectX12/Dx12BufferManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/Texture2D.h"

namespace SaplingEngine
{
	uint32_t												Dx12BufferManager::cbvSrvDescriptorSize = 0;
	std::vector<ComPtr<ID3D12DescriptorHeap>>				Dx12BufferManager::objectCbvDescriptorHeaps;
	std::vector<Dx12BufferManager::ObjectUploadBufferData>	Dx12BufferManager::objectUploadBuffers;
	std::vector<uint32_t>									Dx12BufferManager::availableObjectCbvIndices;
	ComPtr<ID3D12DescriptorHeap>							Dx12BufferManager::passCbvDescriptorHeap;
	Dx12BufferManager::PassUploadBufferData					Dx12BufferManager::passUploadBuffer;
	ComPtr<ID3D12DescriptorHeap>							Dx12BufferManager::srvDescriptorHeap;
	std::vector<uint32_t>									Dx12BufferManager::availableSrvIndices;

	/**
	 * \brief	初始化
	 */
	void Dx12BufferManager::Initialize()
	{
		//查询描述符大小
		cbvSrvDescriptorSize = GraphicsManager::GetDx12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		CreateObjectCbvDescriptorHeap();
		CreatePassCbvDescriptorHeap();
		CreateSrvDescriptorHeap();
	}

	/**
	 * \brief	销毁
	 */
	void Dx12BufferManager::Destroy()
	{
	}

	/**
	 * \brief	压入可用的Object常量缓冲区索引
	 */
	void Dx12BufferManager::PushCbvIndex(uint32_t index)
	{
		availableObjectCbvIndices.push_back(index);
	}

	/**
	 * \brief	弹出可用的物体常量缓冲区索引
	 */
	uint32_t Dx12BufferManager::PopCbvIndex(D3D12_GPU_VIRTUAL_ADDRESS& commonCbAddress, D3D12_GPU_VIRTUAL_ADDRESS& specialCbAddress)
	{
		if (availableObjectCbvIndices.empty())
		{
			CreateObjectCbvDescriptorHeap();
		}

		auto index = *availableObjectCbvIndices.rbegin();
		availableObjectCbvIndices.pop_back();

		auto cbvIndex = index % ConstantBufferElementCount;
		auto& objectUploadBuffer = objectUploadBuffers[index / ConstantBufferElementCount];
		commonCbAddress = objectUploadBuffer.CommonUploadBuffer->GetGPUVirtualAddress() + static_cast<uint64_t>(cbvIndex) * ObjectCommonCbSize;
		specialCbAddress = objectUploadBuffer.SpecialUploadBuffer->GetGPUVirtualAddress() + static_cast<uint64_t>(cbvIndex) * ObjectSpecialCbSize;

		return index;
	}

	/**
	 * \brief	压入可用的SRV索引
	 */
	void Dx12BufferManager::PushSrvIndex(uint32_t index)
	{
		availableSrvIndices.push_back(index);
	}

	/**
	 * \brief	弹出可用的SRV索引
	 */
	uint32_t Dx12BufferManager::PopSrvIndex()
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
	 * \brief	创建常量缓冲区描述符
	 */
	void Dx12BufferManager::CreateObjectCbvDescriptorHeap()
	{
		//声明常量缓冲区描述符堆
		ComPtr<ID3D12DescriptorHeap> cbvDescriptorHeap;

		//常量缓冲区描述符/着色器资源描述符/无需访问描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = ConstantBufferElementCount * 2;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//创建常量缓冲区描述符堆
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(cbvDescriptorHeap.GetAddressOf())));
		objectCbvDescriptorHeaps.emplace_back(cbvDescriptorHeap);

		//创建Object上传缓冲区
		ObjectUploadBufferData objectUploadBuffer;
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.CommonUploadBuffer, objectUploadBuffer.CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, 0);
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.SpecialUploadBuffer, objectUploadBuffer.SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, ConstantBufferElementCount);
		objectUploadBuffers.emplace_back(std::move(objectUploadBuffer));

		//记录可用Object上传缓冲区索引
		auto count = static_cast<uint32_t>(availableObjectCbvIndices.size());
		availableObjectCbvIndices.reserve((count + 1) * ConstantBufferElementCount);
		for (uint32_t index = 0; index < ConstantBufferElementCount; ++index)
		{
			availableObjectCbvIndices.emplace_back(index + count);
		}
	}

	/**
	 * \brief	创建Pass常量缓冲区描述符堆
	 */
	void Dx12BufferManager::CreatePassCbvDescriptorHeap()
	{
		//常量缓冲区描述符/着色器资源描述符/无需访问描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = 2;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//创建常量缓冲区描述符堆
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(passCbvDescriptorHeap.GetAddressOf())));

		//创建Object上传缓冲区
		CreateUploadBuffer(passCbvDescriptorHeap.Get(), passUploadBuffer.PassUploadBuffer, passUploadBuffer.PassMappedData, 2, PassCommonCbSize, 0);
	}

	/**
	 * \brief	创建SRV描述符堆
	 */
	void Dx12BufferManager::CreateSrvDescriptorHeap()
	{
		//创建SRV描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = SrvCount;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvDescriptorHeap)));

		availableSrvIndices.reserve(SrvCount);
		for (auto i = 0; i < SrvCount; ++i)
		{
			availableSrvIndices.push_back(i);
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
	void Dx12BufferManager::FillOcbData(uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize)
	{
		const auto& uploadBuffer = Dx12BufferManager::objectUploadBuffers[index / ConstantBufferElementCount];

		index %= ConstantBufferElementCount;
		memcpy(uploadBuffer.CommonMappedData + index * static_cast<uint64_t>(Dx12BufferManager::ObjectCommonCbSize), pCommonData, commonDataSize);
		memcpy(uploadBuffer.SpecialMappedData + index * static_cast<uint64_t>(Dx12BufferManager::ObjectSpecialCbSize), pSpecialData, specialDataSize);
	}

	/**
	 * \brief	填充Pass常量缓冲区数据
	 * \param	pData			通用数据
	 * \param	dataSize		通用数据大小
	 */
	void Dx12BufferManager::FillPcbData(const void* pData, size_t dataSize)
	{
		memcpy(passUploadBuffer.PassMappedData, pData, dataSize);
	}

	/**
	 * \brief	填充ShadowPass常量缓冲区数据
	 * \param	pData			通用数据
	 * \param	dataSize		通用数据大小
	 */
	void Dx12BufferManager::FillShadowPcbData(const void* pData, size_t dataSize)
	{
		memcpy(passUploadBuffer.PassMappedData + static_cast<uint64_t>(Dx12BufferManager::PassCommonCbSize), pData, dataSize);
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
	void Dx12BufferManager::CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset)
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