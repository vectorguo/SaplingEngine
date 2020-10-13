#pragma once

#include "SaplingEngineGraphicsPch.h"
#include "Utility/Exception.h"

namespace SaplingEngine
{
	template <typename T>
	class D3D12UploadBuffer
	{
	public:
		D3D12UploadBuffer(ID3D12Device* device, uint32_t elementCount, bool isConstant) : m_ElementSize(isConstant ? (sizeof(T) + 255 & ~255) : sizeof(T))
		{
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
				elementCount * m_ElementSize,
				1,
				1,
				1,
				DXGI_FORMAT_UNKNOWN,
				{1, 0},
				D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
				D3D12_RESOURCE_FLAG_NONE
			};

			ThrowIfFailed(device->CreateCommittedResource(&headProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_UploadBuffer)));
			ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pMappedData)));
		}

		D3D12UploadBuffer(const D3D12UploadBuffer&) = delete;
		D3D12UploadBuffer(D3D12UploadBuffer&&) = delete;
		D3D12UploadBuffer& operator=(const D3D12UploadBuffer&) = delete;
		D3D12UploadBuffer& operator=(D3D12UploadBuffer&&) = delete;

		~D3D12UploadBuffer()
		{
			if (m_UploadBuffer != nullptr)
			{
				m_UploadBuffer->Unmap(0, nullptr);
			}
			m_pMappedData = nullptr;
		}

		ID3D12Resource* GetResource() const
		{
			return m_UploadBuffer.Get();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const
		{
			return m_UploadBuffer->GetGPUVirtualAddress();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(const int index) const
		{
			return m_UploadBuffer->GetGPUVirtualAddress() + index * m_ElementSize;
		}

		uint32_t GetElementSize() const
		{
			return m_ElementSize;
		}

		void CopyData(uint32_t elementIndex, const T& data)
		{
			memcpy(m_pMappedData + (elementIndex * m_ElementSize), &data, sizeof(T));
		}

	private:
		ComPtr<ID3D12Resource> m_UploadBuffer;
		uint32_t m_ElementSize;
		uint8_t* m_pMappedData = nullptr;
	};
}