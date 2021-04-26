#include "DirectX12/Dx12GraphicsManager.h"

#include "Mesh.h"
#include "MeshFactory.h"

namespace SaplingEngine
{
	std::vector<Mesh*>	MeshFactory::unreadyMeshes;
	
	/**
	 * \brief	����Mesh
	 * \param	path		Mesh����Դ·��
	 * \return	ָ�򴴽��õ�Mesh��ָ��
	 */
	Mesh* MeshFactory::CreateMesh(const std::string& path)
	{
		auto* pMesh = new Mesh();
		pMesh->Load(path);

		unreadyMeshes.emplace_back(pMesh);

		return pMesh;
	}
	
	/**
	 * \brief	ɾ��Mesh
	 * \param	pMesh		Ҫ��ɾ����Mesh��ָ��
	 */
	void MeshFactory::DestroyMesh(Mesh* pMesh)
	{
		if (!pMesh->IsReady())
		{
			unreadyMeshes.erase(std::find(unreadyMeshes.begin(), unreadyMeshes.end(), pMesh));
		}

		delete pMesh;
	}

	/**
	 * \brief �ϴ�Mesh���ݵ�GPU
	 */
	void MeshFactory::UploadMeshDatas()
	{
		for (auto iter = unreadyMeshes.begin(); iter != unreadyMeshes.end(); ++iter)
		{
			auto* pMesh = *iter;
			pMesh->m_IsReady = true;

			//�������㻺����������
			const auto vertexSize = static_cast<uint32_t>(pMesh->GetVertexCount() * sizeof(VertexData));
			pMesh->m_VertexBufferOnGpu = GraphicsManager::CreateDefaultBufferAndUploadData(pMesh->GetVertexDatas(), vertexSize);
			pMesh->m_pVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW{ pMesh->m_VertexBufferOnGpu->GetGPUVirtualAddress(), vertexSize, sizeof(VertexData) };

			//��������������������
			const auto indexSize = static_cast<uint32_t>(pMesh->GetIndexCount() * sizeof(uint16_t));
			pMesh->m_IndexBufferOnGpu = GraphicsManager::CreateDefaultBufferAndUploadData(pMesh->GetIndices(), indexSize);
			pMesh->m_pIndexBufferView = new D3D12_INDEX_BUFFER_VIEW{ pMesh->m_IndexBufferOnGpu->GetGPUVirtualAddress(), indexSize, DXGI_FORMAT_R16_UINT };
		}
		unreadyMeshes.clear();
	}
}