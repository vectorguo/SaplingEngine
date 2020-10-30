#include "Mesh.h"

#include "RenderPipeline/RenderLibrary/DirectX12/Dx12GraphicsManager.h"

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
		auto* pGraphicsManager = dynamic_cast<Dx12GraphicsManager*>(GraphicsManager::Instance());
		for (auto iter = m_NotUploadedMeshes.begin(); iter != m_NotUploadedMeshes.end(); ++iter)
		{
			auto* pMesh = *iter;
			pMesh->m_IsReady = true;
			
			//创建顶点缓冲区描述符
			const auto vertexSize = static_cast<uint32_t>(pMesh->GetVertexCount() * sizeof(VertexData));
			pMesh->m_VertexBufferOnGpu = pGraphicsManager->CreateDefaultBufferAndUploadData(pMesh->GetVertexDatas(), vertexSize);
			pMesh->m_pVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW{ pMesh->m_VertexBufferOnGpu->GetGPUVirtualAddress(), vertexSize, sizeof(VertexData) };
			
			//创建索引缓冲区描述符
			const auto indexSize = static_cast<uint32_t>(pMesh->GetIndexCount() * sizeof(uint16_t));
			pMesh->m_IndexBufferOnGpu = pGraphicsManager->CreateDefaultBufferAndUploadData(pMesh->GetIndices(), indexSize);
			pMesh->m_pIndexBufferView = new D3D12_INDEX_BUFFER_VIEW{ pMesh->m_IndexBufferOnGpu->GetGPUVirtualAddress(), indexSize, DXGI_FORMAT_R16_UINT };
		}
		m_NotUploadedMeshes.clear();
	}
}
