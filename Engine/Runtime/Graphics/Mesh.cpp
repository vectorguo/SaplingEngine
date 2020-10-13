#include "Mesh.h"
#include "Graphics/DirectX12/D3D12GraphicsManager.h"

namespace SaplingEngine
{
	Mesh::~Mesh()
	{
		if (m_pVertexBufferView != nullptr)
		{
			delete m_pVertexBufferView;
			m_pVertexBufferView = nullptr;
		}

		if (m_pIndexBufferView != nullptr)
		{
			delete m_pIndexBufferView;
			m_pIndexBufferView = nullptr;
		}
	}

	/**
	 * \brief 创建顶点缓冲区描述符
	 */
	void Mesh::CreateVertexBufferView()
	{
		const auto vertexSize = static_cast<uint32_t>(m_VertexDatas.size() * sizeof(VertexData));
		m_VertexBufferOnGpu = D3D12GraphicsManager::Instance()->CreateDefaultBuffer(m_VertexDatas.data(), vertexSize, m_VertexBufferUploader);
		m_pVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW{ m_VertexBufferOnGpu->GetGPUVirtualAddress(), vertexSize, sizeof(VertexData) };
	}

	/**
	 * \brief 创建索引缓冲区描述符
	 */
	void Mesh::CreateIndexBufferView()
	{
		const auto indexSize = static_cast<uint32_t>(m_Indices.size() * sizeof(uint16_t));
		m_IndexBufferOnGpu = D3D12GraphicsManager::Instance()->CreateDefaultBuffer(m_Indices.data(), indexSize, m_IndexBufferUploader);
		m_pIndexBufferView = new D3D12_INDEX_BUFFER_VIEW{ m_IndexBufferOnGpu->GetGPUVirtualAddress(), indexSize, DXGI_FORMAT_R16_UINT };
	}
}
