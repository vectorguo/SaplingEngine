#pragma once

#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/DirectX12/Dx12Utility.h"

namespace SaplingEngine
{
	class Dx12UploadBuffer
	{
	public:
		Dx12UploadBuffer(uint32_t totalSize)
		{
			D3D12_HEAP_PROPERTIES headProperties
			{
				D3D12_HEAP_TYPE_UPLOAD,
				D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
				D3D12_MEMORY_POOL_UNKNOWN,
				1,
				1
			};

			D3D12_RESOURCE_DESC bufferDesc
			{
				D3D12_RESOURCE_DIMENSION_BUFFER,
				0,
				(uint64_t)totalSize,
				1,
				1,
				1,
				DXGI_FORMAT_UNKNOWN,
				{1, 0},
				D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
				D3D12_RESOURCE_FLAG_NONE
			};

			auto* pDevice = GraphicsManager::GetDx12Device();
			ThrowIfFailed(pDevice->CreateCommittedResource(&headProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_UploadBuffer)));
			ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedDataPtr)));
		}

		Dx12UploadBuffer(const Dx12UploadBuffer&) = delete;
		Dx12UploadBuffer(Dx12UploadBuffer&&) = delete;
		Dx12UploadBuffer& operator=(const Dx12UploadBuffer&) = delete;
		Dx12UploadBuffer& operator=(Dx12UploadBuffer&&) = delete;

		~Dx12UploadBuffer()
		{
			if (m_UploadBuffer != nullptr)
			{
				m_UploadBuffer->Unmap(0, nullptr);
			}
			m_MappedDataPtr = nullptr;
		}

		ID3D12Resource* GetResource() const
		{
			return m_UploadBuffer.Get();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const
		{
			return m_UploadBuffer->GetGPUVirtualAddress();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32_t offset) const
		{
			return m_UploadBuffer->GetGPUVirtualAddress() + offset;
		}

		void CopyData(uint32_t offset, const void* pData, uint32_t dataSize)
		{
			memcpy(m_MappedDataPtr + offset, pData, dataSize);
		}

	private:
		ComPtr<ID3D12Resource> m_UploadBuffer;
		uint8_t* m_MappedDataPtr = nullptr;
	};
}