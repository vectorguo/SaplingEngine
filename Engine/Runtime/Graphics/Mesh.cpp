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

	/**
	 * \brief 创建基本类型的mesh
	 * \param type mesh类型
	 * \return mesh指针
	 */
	Mesh* Mesh::CreatePrimitive(EMeshPrimitiveType type)
	{
		std::vector<VertexData> vertices =
		{
			VertexData({ XMFLOAT3(-1.0f, -1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::White),		XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(-1.0f, +1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Black),		XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, +1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Red),		XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, -1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Green),		XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(-1.0f, -1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Blue),		XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(-1.0f, +1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Yellow),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, +1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Cyan),		XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, -1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Magenta),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) })
		};

		std::vector<std::uint16_t> indices =
		{
			0, 1, 2,	// front face
			0, 2, 3,
			4, 6, 5,	// back face
			4, 7, 6,
			4, 5, 1,	// left face
			4, 1, 0,
			3, 2, 6,	// right face
			3, 6, 7,
			1, 5, 6,	// top face
			1, 6, 2,
			4, 0, 3,	// bottom face
			4, 3, 7
		};
		
		auto* pMesh = new Mesh();
		pMesh->SetVertexDatas(std::move(vertices));
		pMesh->SetIndices(std::move(indices));
		return pMesh;
	}
}
