#include "Mesh.h"
#include "Application/DirectX12/D3D12Application.h"

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
		auto* pApp = dynamic_cast<D3D12Application*>(GameApplication::Instance());
		const auto vertexSize = static_cast<uint32_t>(m_VertexDatas.size() * sizeof(VertexData));
		m_VertexBufferOnGpu = pApp->CreateDefaultBuffer(m_VertexDatas.data(), vertexSize, m_VertexBufferUploader);
		m_pVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW{ m_VertexBufferOnGpu->GetGPUVirtualAddress(), vertexSize, sizeof(VertexData) };
	}

	/**
	 * \brief 创建索引缓冲区描述符
	 */
	void Mesh::CreateIndexBufferView()
	{
		auto* pApp = dynamic_cast<D3D12Application*>(GameApplication::Instance());
		const auto indexSize = static_cast<uint32_t>(m_Indices.size() * sizeof(uint16_t));
		m_IndexBufferOnGpu = pApp->CreateDefaultBuffer(m_Indices.data(), indexSize, m_IndexBufferUploader);
		m_pIndexBufferView = new D3D12_INDEX_BUFFER_VIEW{ m_IndexBufferOnGpu->GetGPUVirtualAddress(), indexSize, DXGI_FORMAT_R16_UINT };
	}
}
