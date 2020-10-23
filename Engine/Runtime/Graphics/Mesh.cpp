#include "Mesh.h"

#include "RenderPipeline/RenderPipeline.h"

namespace SaplingEngine
{
	std::vector<Mesh*> Mesh::m_NotUploadedMeshes;
	
	Mesh::Mesh() : m_IsReady(false)
	{
		m_NotUploadedMeshes.push_back(this);
	}

	Mesh::~Mesh()
	{
		delete m_pVertexBufferView;
		delete m_pIndexBufferView;

		if (!m_IsReady)
		{
			m_NotUploadedMeshes.erase(std::find(m_NotUploadedMeshes.begin(), m_NotUploadedMeshes.end(), this));
		}
	}

	/**
	 * \brief 上传Mesh数据到GPU
	 */
	void Mesh::UploadMeshDatas()
	{
		auto& graphicsManager = RenderPipeline::Instance()->GetGraphicsManager();
		for (auto iter = m_NotUploadedMeshes.begin(); iter != m_NotUploadedMeshes.end(); ++iter)
		{
			auto* pMesh = *iter;
			pMesh->m_IsReady = true;
			
			//创建顶点缓冲区描述符
			const auto vertexSize = static_cast<uint32_t>(pMesh->m_VertexDatas.size() * sizeof(VertexData));
			pMesh->m_VertexBufferOnGpu = graphicsManager.CreateDefaultBufferAndUploadData(pMesh->m_VertexDatas.data(), vertexSize);
			pMesh->m_pVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW{ pMesh->m_VertexBufferOnGpu->GetGPUVirtualAddress(), vertexSize, sizeof(VertexData) };
			
			//创建索引缓冲区描述符
			const auto indexSize = static_cast<uint32_t>(pMesh->m_Indices.size() * sizeof(uint16_t));
			pMesh->m_IndexBufferOnGpu = graphicsManager.CreateDefaultBufferAndUploadData(pMesh->m_Indices.data(), indexSize);
			pMesh->m_pIndexBufferView = new D3D12_INDEX_BUFFER_VIEW{ pMesh->m_IndexBufferOnGpu->GetGPUVirtualAddress(), indexSize, DXGI_FORMAT_R16_UINT };
		}
		m_NotUploadedMeshes.clear();
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
