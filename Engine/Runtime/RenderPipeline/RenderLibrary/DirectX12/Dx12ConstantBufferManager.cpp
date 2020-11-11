#include "Dx12ConstantBufferManager.h"

namespace SaplingEngine
{
	Dx12ConstantBufferManager::Dx12ConstantBufferManager(ID3D12Device* pDevice, ID3D12DescriptorHeap* pCbvDescriptorHeap, uint32_t cbvDescriptorSize)
		: m_pD3DDevice(pDevice), m_pCbvDescriptorHeap(pCbvDescriptorHeap), m_CbvDescriptorSize(cbvDescriptorSize)
	{
		//����Pass�ϴ�������
		CreateUploadBuffer(m_PassCommonUploadBuffer, m_PassCommonMappedData, 1, PassCommonCbSize, 0);

		//����Object�ϴ�������
		m_ObjectUploadBuffers.emplace_back();
		auto iter = m_ObjectUploadBuffers.rbegin();
		CreateUploadBuffer(iter->CommonUploadBuffer, iter->CommonMappedData, CbElementCount, ObjectCommonCbSize, 1);
		CreateUploadBuffer(iter->SpecialUploadBuffer, iter->SpecialMappedData, CbElementCount, ObjectSpecialCbSize, 1 + CbElementCount);

		//��¼����Object�ϴ�����������
		m_AvailableOubIndices.reserve(CbElementCount);
		for (uint32_t index = 1; index < CbElementCount; ++index)
		{
			m_AvailableOubIndices.emplace_back(index);
		}
	}

	/**
	 * \brief ѹ����õ����峣������������
	 * \param index ���õ����峣������������
	 */
	void Dx12ConstantBufferManager::PushObjectCbIndex(uint32_t index)
	{
		if (index <= m_ObjectUploadBuffers.size() * CbElementCount)
		{
			m_AvailableOubIndices.emplace_back(index);
		}
	}

	/**
	 * \brief �������õ����峣������������
	 * \return ���õ����峣������������
	 */
	uint32_t Dx12ConstantBufferManager::PopObjectCbIndex()
	{
		if (m_AvailableOubIndices.empty())
		{
			const auto startIndex = static_cast<uint32_t>(m_ObjectUploadBuffers.size()) * DoubleCbElementCount + 1;
			
			//�Ѿ�û�ж���ĳ�������������Ҫ�����µĳ���������
			m_ObjectUploadBuffers.emplace_back();
			auto iter = m_ObjectUploadBuffers.rbegin();
			CreateUploadBuffer(iter->CommonUploadBuffer, iter->CommonMappedData, CbElementCount, ObjectCommonCbSize, startIndex);
			CreateUploadBuffer(iter->SpecialUploadBuffer, iter->SpecialMappedData, CbElementCount, ObjectSpecialCbSize, startIndex + CbElementCount);

			//��¼����Object�ϴ�����������
			m_AvailableOubIndices.reserve(CbElementCount);
			for (uint32_t index = 0; index < CbElementCount; ++index)
			{
				m_AvailableOubIndices.emplace_back(startIndex + index);
			}
		}

		const auto index = *m_AvailableOubIndices.rbegin();
		m_AvailableOubIndices.pop_back();
		return index;
	}

	/**
	 * \brief �������ݵ�����������
	 * \param index ����ĳ�������������
	 * \param pData ����ָ��
	 * \param dataSize ���ݴ�С
	 * \param isSpecial �Ƿ������ⳣ������������
	 */
	void Dx12ConstantBufferManager::CopyObjectCbData(uint32_t index, const void* pData, size_t dataSize, bool isSpecial)
	{
		index -= 1;
		const auto elementIndex = index % DoubleCbElementCount;
		const auto oubIndex = index / DoubleCbElementCount;
		if (isSpecial)
		{
			memcpy(m_ObjectUploadBuffers[oubIndex].SpecialMappedData + static_cast<uint64_t>(elementIndex) * static_cast<uint64_t>(ObjectSpecialCbSize), pData, dataSize);
		}
		else
		{
			memcpy(m_ObjectUploadBuffers[oubIndex].CommonMappedData + static_cast<uint64_t>(elementIndex) * static_cast<uint64_t>(ObjectCommonCbSize), pData, dataSize);
		}
	}

	void Dx12ConstantBufferManager::CopyPassCbData(const void* pData, size_t dataSize)
	{
		memcpy(m_PassCommonMappedData, pData, dataSize);
	}

	/**
	 * \brief �����ϴ�������
	 * \param uploadBuffer �ϴ�������ָ��
	 * \param mappedData ����ָ��
	 * \param elementCount Ԫ������
	 * \param elementSize Ԫ�ش�С
	 * \param descriptorHeapOffset �ڳ������������������е�ƫ��λ��
	 */
	void Dx12ConstantBufferManager::CreateUploadBuffer(ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset) const
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

		ThrowIfFailed(m_pD3DDevice->CreateCommittedResource(&headProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer)));
		ThrowIfFailed(uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));

		//������Ӧ�ĳ���������������
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		for (uint32_t index = 0; index < elementCount; ++index)
		{
			cbvDesc.BufferLocation = uploadBuffer->GetGPUVirtualAddress() + static_cast<uint64_t>(index) * elementSize;
			cbvDesc.SizeInBytes = elementSize;
			m_pD3DDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(m_pCbvDescriptorHeap, descriptorHeapOffset + index, m_CbvDescriptorSize));
		}
	}
}
