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
	 * \brief ��ʼ��
	 */
	void Dx12CBufferManager::Initialize()
	{
		auto* pDevice = GraphicsManager::GetDx12Device();
		
		//��ѯ��������С
		CbvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//����������������������
		CreateCbvDescriptorHeap(pDevice);

		//��¼����Object�ϴ�����������
		m_AvailableOubIndices.reserve(ConstantBufferElementCount);
		for (uint32_t index = 0; index < ConstantBufferElementCount; ++index)
		{
			m_AvailableOubIndices.emplace_back(index);
		}
	}

	/**
	 * \brief ����
	 */
	void Dx12CBufferManager::Destroy()
	{
		delete[] m_CbvDescriptorHeapPointers;
		m_CbvDescriptorHeapPointers = nullptr;
	}

	/**
	 * \brief ����Object��������������
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
	 * \brief ����Pass��������������
	 */
	void Dx12CBufferManager::UpdatePcbData(Camera* pCamera)
	{
		size_t dataSize;
		const auto* pPassData = CommonPcbData::FillPcbData(dataSize, pCamera);
		memcpy(m_PassCommonMappedData, pPassData, dataSize);
	}

	/**
	 * \brief ��ȡObject��������������
	 * \param ocbIndex Object�ĳ�������������������
	 * \return Object��������������
	 */
	D3D12_GPU_DESCRIPTOR_HANDLE Dx12CBufferManager::GetObjectCbvDescriptor(uint32_t ocbIndex)
	{
		return GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), ocbIndex, CbvDescriptorSize);
	}

	/**
	 * \brief ��ȡPass��������������
	 * \return ����Pass����������
	 */
	D3D12_GPU_DESCRIPTOR_HANDLE Dx12CBufferManager::GetPassCbvDescriptor()
	{
		return GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), m_PassCbvDescriptorOffset, CbvDescriptorSize);
	}

	/**
	 * \brief ѹ����õ����峣������������
	 * \param index ���õ����峣������������
	 */
	void Dx12CBufferManager::PushObjectCbIndex(uint32_t index)
	{
		if (index < m_ObjectUploadBuffers.size() * ConstantBufferElementCount)
		{
			m_AvailableOubIndices.emplace_back(index);
		}
	}

	/**
	 * \brief �������õ����峣������������
	 * \return ���õ����峣������������
	 */
	uint32_t Dx12CBufferManager::PopObjectCbIndex()
	{
		if (m_AvailableOubIndices.empty())
		{
			const auto startIndex = static_cast<uint32_t>(m_ObjectUploadBuffers.size()) * DoubleConstantBufferElementCount;
			
			//�����µ�Object�ϴ�������
			m_ObjectUploadBuffers.emplace_back();
			auto iter = m_ObjectUploadBuffers.rbegin();
			CreateUploadBuffer(m_CbvDescriptorHeap.Get(), iter->CommonUploadBuffer, iter->CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, startIndex);
			CreateUploadBuffer(m_CbvDescriptorHeap.Get(), iter->SpecialUploadBuffer, iter->SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, startIndex + ConstantBufferElementCount);

			//��¼����Object�ϴ�����������
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
	 * \brief ��������������������
	 * \param pDevice Dx12�豸ָ��
	 */
	void Dx12CBufferManager::CreateCbvDescriptorHeap(ID3D12Device* pDevice)
	{
		//����������������/��ɫ����Դ������/���������������
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = ConstantBufferDescriptorHeapSize;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		//����������������������
		ThrowIfFailed(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_CbvDescriptorHeap.GetAddressOf())));

		//��������������������������
		m_CbvDescriptorHeapPointers = new ID3D12DescriptorHeap*[1];
		m_CbvDescriptorHeapPointers[0] = m_CbvDescriptorHeap.Get();

		//����Object�ϴ�������
		m_ObjectUploadBuffers.resize(1);
		auto& objectUploadBuffer = m_ObjectUploadBuffers[0];
		CreateUploadBuffer(m_CbvDescriptorHeap.Get(), objectUploadBuffer.CommonUploadBuffer, objectUploadBuffer.CommonMappedData, ConstantBufferElementCount, ObjectCommonCbSize, 0);
		CreateUploadBuffer(m_CbvDescriptorHeap.Get(), objectUploadBuffer.SpecialUploadBuffer, objectUploadBuffer.SpecialMappedData, ConstantBufferElementCount, ObjectSpecialCbSize, ConstantBufferElementCount);

		//����Pass�ϴ�������
		m_PassCbvDescriptorOffset = ConstantBufferDescriptorHeapSize - 1;
		CreateUploadBuffer(m_CbvDescriptorHeap.Get(), m_PassCommonUploadBuffer, m_PassCommonMappedData, 1, PassCommonCbSize, m_PassCbvDescriptorOffset);
	}
	
	/**
	 * \brief �����ϴ�������
	 * \param descriptorHeap ��������
	 * \param uploadBuffer �ϴ�������ָ��
	 * \param mappedData ����ָ��
	 * \param elementCount Ԫ������
	 * \param elementSize Ԫ�ش�С
	 * \param descriptorHeapOffset �ڳ������������������е�ƫ��λ��
	 */
	void Dx12CBufferManager::CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset)
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
			pDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(descriptorHeap, descriptorHeapOffset + index, CbvDescriptorSize));
		}
	}
}
