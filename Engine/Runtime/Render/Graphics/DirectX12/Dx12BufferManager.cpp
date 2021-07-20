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
	 * \brief	��ʼ��
	 */
	void Dx12BufferManager::Initialize()
	{
		//��ѯ��������С
		cbvSrvDescriptorSize = GraphicsManager::GetDx12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		CreateObjectCbvDescriptorHeap();
		CreatePassCbvDescriptorHeap();
		CreateSrvDescriptorHeap();
	}

	/**
	 * \brief	����
	 */
	void Dx12BufferManager::Destroy()
	{
	}

	/**
	 * \brief	ѹ����õ�Object��������������
	 */
	void Dx12BufferManager::PushCbvIndex(uint32_t index)
	{
		availableObjectCbvIndices.push_back(index);
	}

	/**
	 * \brief	�������õ����峣������������
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
	 * \brief	ѹ����õ�SRV����
	 */
	void Dx12BufferManager::PushSrvIndex(uint32_t index)
	{
		availableSrvIndices.push_back(index);
	}

	/**
	 * \brief	�������õ�SRV����
	 */
	uint32_t Dx12BufferManager::PopSrvIndex()
	{
		if (availableSrvIndices.empty())
		{
			throw Exception("��ͼ����̫�࣬�޿���Srv�������ѿռ�");
		}

		auto index = *availableSrvIndices.rbegin();
		availableSrvIndices.pop_back();
		return index;
	}

	/**
	 * \brief	��������������������
	 */
	void Dx12BufferManager::CreateObjectCbvDescriptorHeap()
	{
		//����������������������
		ComPtr<ID3D12DescriptorHeap> cbvDescriptorHeap;

		//����������������/��ɫ����Դ������/���������������
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = ConstantBufferElementCount * 2;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//����������������������
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(cbvDescriptorHeap.GetAddressOf())));
		objectCbvDescriptorHeaps.emplace_back(cbvDescriptorHeap);

		//����Object�ϴ�������
		ObjectUploadBufferData objectUploadBuffer;
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.CommonUploadBuffer, objectUploadBuffer.CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, 0);
		CreateUploadBuffer(cbvDescriptorHeap.Get(), objectUploadBuffer.SpecialUploadBuffer, objectUploadBuffer.SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, ConstantBufferElementCount);
		objectUploadBuffers.emplace_back(std::move(objectUploadBuffer));

		//��¼����Object�ϴ�����������
		auto count = static_cast<uint32_t>(availableObjectCbvIndices.size());
		availableObjectCbvIndices.reserve((count + 1) * ConstantBufferElementCount);
		for (uint32_t index = 0; index < ConstantBufferElementCount; ++index)
		{
			availableObjectCbvIndices.emplace_back(index + count);
		}
	}

	/**
	 * \brief	����Pass������������������
	 */
	void Dx12BufferManager::CreatePassCbvDescriptorHeap()
	{
		//����������������/��ɫ����Դ������/���������������
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = 2;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//����������������������
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(passCbvDescriptorHeap.GetAddressOf())));

		//����Object�ϴ�������
		CreateUploadBuffer(passCbvDescriptorHeap.Get(), passUploadBuffer.PassUploadBuffer, passUploadBuffer.PassMappedData, 2, PassCommonCbSize, 0);
	}

	/**
	 * \brief	����SRV��������
	 */
	void Dx12BufferManager::CreateSrvDescriptorHeap()
	{
		//����SRV��������
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
	 * \brief	������峣������������
	 * \param	index			���õ����峣������������
	 * \param	pCommonData		ͨ������
	 * \param	commonDataSize	ͨ�����ݴ�С
	 * \param	pSpecialData	��������
	 * \param	specialDataSize	�������ݴ�С
	 */
	void Dx12BufferManager::FillOcbData(uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize)
	{
		const auto& uploadBuffer = Dx12BufferManager::objectUploadBuffers[index / ConstantBufferElementCount];

		index %= ConstantBufferElementCount;
		memcpy(uploadBuffer.CommonMappedData + index * static_cast<uint64_t>(Dx12BufferManager::ObjectCommonCbSize), pCommonData, commonDataSize);
		memcpy(uploadBuffer.SpecialMappedData + index * static_cast<uint64_t>(Dx12BufferManager::ObjectSpecialCbSize), pSpecialData, specialDataSize);
	}

	/**
	 * \brief	���Pass��������������
	 * \param	pData			ͨ������
	 * \param	dataSize		ͨ�����ݴ�С
	 */
	void Dx12BufferManager::FillPcbData(const void* pData, size_t dataSize)
	{
		memcpy(passUploadBuffer.PassMappedData, pData, dataSize);
	}

	/**
	 * \brief	���ShadowPass��������������
	 * \param	pData			ͨ������
	 * \param	dataSize		ͨ�����ݴ�С
	 */
	void Dx12BufferManager::FillShadowPcbData(const void* pData, size_t dataSize)
	{
		memcpy(passUploadBuffer.PassMappedData + static_cast<uint64_t>(Dx12BufferManager::PassCommonCbSize), pData, dataSize);
	}

	/**
	 * \brief	�����ϴ�������
	 * \param	descriptorHeap	��������
	 * \param	uploadBuffer	�ϴ�������ָ��
	 * \param	mappedData		����ָ��
	 * \param	elementCount	Ԫ������
	 * \param	elementSize		Ԫ�ش�С
	 * \param	descriptorHeapOffset �ڳ������������������е�ƫ��λ��
	 */
	void Dx12BufferManager::CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset)
	{
		//�����ϴ�������
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

		//������Ӧ�ĳ���������������
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		for (uint32_t index = 0; index < elementCount; ++index)
		{
			cbvDesc.BufferLocation = uploadBuffer->GetGPUVirtualAddress() + static_cast<uint64_t>(index) * elementSize;
			cbvDesc.SizeInBytes = elementSize;
			pDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(descriptorHeap, descriptorHeapOffset + index, cbvSrvDescriptorSize));
		}
	}
}